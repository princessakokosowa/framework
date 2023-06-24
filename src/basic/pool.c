core_function void Pool_setAllocators(Pool *pool, Allocator *allocator) {
    pool->backing_allocator = allocator;
}

core_function void Pool_makeAndSwapBlocks(Pool *pool) {
    Block *new_block;
    if (pool->unused_blocks != null) {
        new_block           = pool->unused_blocks;
        pool->unused_blocks = pool->unused_blocks->next;
    } else {
        new_block = allocWithAllocator(pool->bucket_size, pool->backing_allocator);

        // if (pool->should_overwrite_blocks == true && new_block != null) // Overwrite...
    }

    new_block->next   = pool->used_blocks;
    pool->used_blocks = new_block;
    if (pool->first_used_memblock == null) pool->first_used_memblock = new_block;

    pool->left          = pool->bucket_size - sizeof(Block);
    pool->current_block = new_block;
    pool->current_ptr   = cast(u8 *, pool->current_block);
}

core_function void *Pool_allocatorProcedure(AllocatorMode mode, AllocatorDescription *description) {
    ensure(description->impl != null);

    Pool *pool = cast(Pool*, description->impl);

    switch (mode) {
        case ALLOCATOR_MODE_STARTUP: through
        case ALLOCATOR_MODE_SHUTDOWN: through

        case ALLOCATOR_MODE_THREAD_START: through
        case ALLOCATOR_MODE_THREAD_STOP: through

        case ALLOCATOR_MODE_CREATE_HEAP: through
        case ALLOCATOR_MODE_DESTROY_HEAP: through

        case ALLOCATOR_MODE_IS_THIS_YOURS: through
        case ALLOCATOR_MODE_CAPS: through

        case ALLOCATOR_MODE_FREE: {
            return cast(void*, cast(isize, true));
        } break;
        case ALLOCATOR_MODE_RESIZE: through
        case ALLOCATOR_MODE_ALLOCATE: {
            ensure(description->size_to_be_allocated_or_resized > 0);

            // @TODO
            // Detect whether a user makes a mistake and sets the backing allocator to
            // `Context`'s allocator, which may be set to this pool.
            if (pool->backing_allocator == null) {
                bool are_we_already_set_in_context = context.allocator->impl == pool;
                if (are_we_already_set_in_context == true) Pool_setAllocators(pool, &context.heap_allocator);
                else                                       Pool_setAllocators(pool, context.allocator);
            }

            bool is_that_allocation_too_big = description->size_to_be_allocated_or_resized >= pool->single_allocation_in_bucket_size;
            if (is_that_allocation_too_big == true) {
                Block *block = cast(Block *, allocWithAllocator(description->size_to_be_allocated_or_resized, &context.heap_allocator));
                if (block == null) return null;

                block->next                     = pool->out_of_bounds_allocations;
                pool->out_of_bounds_allocations = block;
                
                u8 *chunk = cast(u8 *, block);

                return chunk;
            }

            isize aligned_ptr_position                = Allocator_alignPtr(pool->current_ptr, pool->alignment);
            bool  is_there_enough_space_left_in_block = pool->left > description->size_to_be_allocated_or_resized + sizeof(Block) + aligned_ptr_position;
            if (is_there_enough_space_left_in_block == false) {
                Pool_makeAndSwapBlocks(pool);

                if (pool->current_block == null) return null;
                aligned_ptr_position = Allocator_alignPtr(pool->current_ptr, pool->alignment);
            }

            u8 *chunk         =  pool->current_ptr + aligned_ptr_position;
            pool->current_ptr += description->size_to_be_allocated_or_resized + aligned_ptr_position;
            pool->left        -= description->size_to_be_allocated_or_resized + aligned_ptr_position;

            // if (pool->should_overwrite_blocks) // Stamp...
            
            return chunk; 
        } break;
    }

    unreachable();
}

core_function Pool Pool_create(PoolDescription *description) {
    return (Pool) {
        .bucket_size                      = POOL_DEFAULT_BUCKET_SIZE,
        .single_allocation_in_bucket_size = POOL_DEFAULT_SINGLE_ALLOCATION_IN_BUCKET_SIZE,
        .alignment                        = POOL_DEFAULT_ALIGNMENT,
        .should_overwrite_blocks          = false,
        .should_free_blocks_on_reset      = false,
    };
}

core_function void Pool_destroy(Pool *pool) {
    *pool = (Pool) {
        0,
    };
}

core_function void *Pool_get(Pool *pool, isize type_size_times_count) {
    return Pool_allocatorProcedure(ALLOCATOR_MODE_ALLOCATE, &(AllocatorDescription) {
        .size_to_be_allocated_or_resized = type_size_times_count,
        .impl                            = cast(void *, pool),
    });
}

core_function Allocator Pool_getAllocator(Pool *pool) {
    return (Allocator) {
        .procedure = Pool_allocatorProcedure,
        .impl      = cast(void *, pool),
    };
}
