#if !defined(AllocatorImpl_reserve)
    #error AllocatorImpl_reserve must be defined to use base memory.
#endif

#if !defined(AllocatorImpl_release)
    #error AllocatorImpl_release must be defined to use base memory.
#endif

#if !defined(AllocatorImpl_commit)
    #error AllocatorImpl_commit must be defined to use base memory.
#endif

#if !defined(AllocatorImpl_decommit)
    #error AllocatorImpl_decommit must be defined to use base memory.
#endif

////////////////////////////////
// Allocator procedures

core_function void Allocator_allocHeap(Allocator *allocator) {
    void *maybe_ptr = allocator->procedure(AllocatorMode_AllocateHeap, &(AllocatorDescription){
        .impl = allocator->impl,
    });

    assert(maybe_ptr != null);
}

core_function void Allocator_releaseHeap(Allocator *allocator) {
    void *maybe_ptr = allocator->procedure(AllocatorMode_ReleaseHeap, &(AllocatorDescription){
        .impl = allocator->impl,
    });

    assert(maybe_ptr != null);
}

core_function void *Allocator_alloc(isize type_size_times_count, Allocator *allocator) {
    void *maybe_ptr = allocator->procedure(AllocatorMode_Allocate, &(AllocatorDescription){
        .size_to_be_allocated_or_resized = type_size_times_count,
        .impl                            = allocator->impl,
    });

    assert(maybe_ptr != null);

    return maybe_ptr;
}

core_function void *Allocator_resize(void *ptr, isize type_size_times_count, Allocator *allocator) {
    assert(ptr != null);

    void *maybe_ptr = allocator->procedure(AllocatorMode_Resize, &(AllocatorDescription){
        .ptr_to_be_resized_or_freed      = ptr,
        .size_to_be_allocated_or_resized = type_size_times_count,
        .impl                            = allocator->impl,
    });

    assert(maybe_ptr != null);

    return maybe_ptr;
}

core_function void Allocator_release(void *ptr, Allocator *allocator) {
    assert(ptr != null);

    void *result_but_ptr = allocator->procedure(AllocatorMode_Release, &(AllocatorDescription) {
        .ptr_to_be_resized_or_freed = ptr,
        .impl                       = allocator->impl,
    });

    assert(result_but_ptr != null);

    unusedVariable(result_but_ptr);
}

////////////////////////////////
// Pool procedures

core_function void *Pool_procedure(AllocatorMode mode, AllocatorDescription *description) {
    assert(description->impl != null);

    Pool *pool = cast(Pool *, description->impl);

    switch (mode) {
        case AllocatorMode_StartUp: through
        case AllocatorMode_ShutDown: through

        case AllocatorMode_ThreadStart: through
        case AllocatorMode_ThreadStop: through

        case AllocatorMode_AllocateHeap: through
        case AllocatorMode_ReleaseHeap: through

        case AllocatorMode_IsThisYours: through
        case AllocatorMode_Capabilities: through

        case AllocatorMode_Release: {
            // @NOTE
            //
            // Mental shortcut: reserving, commiting and releasing that memory is easy,
            // decommiting, not in the least.
            //
            // Decommiting means: fragmentation of memory blocks (or maybe not, but then
            // you would have to come up with a mechanism that flags the pools used and
            // unused and e.g. differentate between large, medium and small memory
            // blocks.
            //
            // So we will stay with the current stategy i.e. only commit, but never
            // decommit for now, and distribute memory from here to other allocators that
            // may be able to manage this memory better.
            //
            // This is the sanest way to handle memory without unnecessary fuckery.
            //
            // I am thinking of converging `Arena` with this allocator and using `Arena`
            // as a global allocator instead. The current system offers more flexibility,
            // but do I really need that flexibility?
            //
            // Maybe that is ssaanneerr.
            //
            // `AllocatorImpl_decommit(pool_block->ptr, pool_block->size)`
            //     ~ princessakokosowa, 22 June 2023

            if (pool->ptr == null) return null;

            AllocatorImpl_release(pool->ptr, pool->size);

            return cast(void *, cast(i64, true));
        } break;
        case AllocatorMode_Resize: through
        case AllocatorMode_Allocate: {
            if (pool->ptr == null) {
                isize size = pool->size;
                isize size_roundup_granularity = megabytes(64);
                isize aligned_size = align(size, size_roundup_granularity);

                pool->ptr      = AllocatorImpl_reserve(aligned_size);
                pool->commited = pool->commit_granularity;

                AllocatorImpl_commit(pool->ptr, pool->commit_granularity);
            }

            void  *ptr           = description->ptr_to_be_resized_or_freed;
            bool  is_that_resize = ptr != null;
            isize size           = description->size_to_be_allocated_or_resized;
            isize aligned_size   = align(size, pool->alignment);

            if (pool->ptr == null) return null;

            assert(pool->occupied + aligned_size <= pool->size);

            u8 *chunk = pool->ptr + pool->occupied;

            pool->occupied += aligned_size;

            if (pool->commited < pool->occupied) {
                isize size_to_commit         = pool->occupied - pool->commited;
                isize aligned_size_to_commit = align(size_to_commit, Pool_DEFAULT_COMMIT_GRANULARITY);

                AllocatorImpl_commit(pool->ptr + pool->commited, aligned_size_to_commit);

                pool->commited += aligned_size_to_commit;
            }

            if (is_that_resize == true) return ptr;

            return chunk;
        } break;
    }

    unreachable();
}

core_function Pool Pool_init(PoolDescription *description) {
    return (Pool) {
        .alignment          = description->alignment,
        .size               = description->size,
        .commit_granularity = description->commit_granularity,
        .decommit_threshold = description->decommit_threshold,
    };
}

core_function void Pool_release(Pool *pool) {
    *pool = (Pool) {
        0,
    };
}

core_function void *Pool_alloc(Pool *pool, isize type_size_times_count) {
    return Pool_procedure(AllocatorMode_Allocate, &(AllocatorDescription) {
        .size_to_be_allocated_or_resized = type_size_times_count,
        .impl                            = cast(void *, pool),
    });
}

core_function Allocator Pool_getAllocator(Pool *pool) {
    return (Allocator) {
        .procedure = Pool_procedure,
        .impl      = cast(void *, pool),
    };
}

////////////////////////////////
// Context globals

#if BUILD_ROOT
    Context context = { 0, };
#endif

////////////////////////////////
// Context procedures

core_function void Context_init(void) {
    context = (Context) {
        .pool            = Pool_init(&(PoolDescription) {
            .alignment          = Pool_DEFAULT_ALIGNMENT,
            .size               = Pool_DEFAULT_SIZE,
            .commit_granularity = Pool_DEFAULT_COMMIT_GRANULARITY,
            .decommit_threshold = Pool_DEFAULT_DECOMMIT_THRESHOLD,
        }),
        ._pool_allocator = Pool_getAllocator(&context.pool),
        .allocator       = &context._pool_allocator,
    };
}

core_function void Context_release(void) {
    Pool_release(&context.pool);

    context = (Context) {
        0,
    };
}

core_function void Context_rememberAllocators(void) {
    assert(MAX_REMEMBERED_LIST_COUNT != context.remembered_count);

    context.remembered_list[context.remembered_count] = context.allocator;
    context.remembered_count                          += 1;
}

core_function void Context_pushAllocators(Allocator *allocator) {
    Context_rememberAllocators();

    context.allocator = allocator;
}

core_function void Context_popAllocators(void) {
    assert(context.remembered_count != 0);

    context.remembered_count                          -= 1;
    context.allocator                                 = context.remembered_list[context.remembered_count];
    context.remembered_list[context.remembered_count] = null;
}

core_function Allocator *Context_getAllocator(void) {
    return context.allocator;
}

core_function Allocator *Context_getDefaultAllocator(void) {
    return &context._pool_allocator;
}

