#ifndef INCLUDE_POOL_H
#define INCLUDE_POOL_H

#include "foundation.h"
#include "allocator.h"

typedef struct Block Block;
struct Block {
    Block *next;
    void  *ptr;
};

#define POOL_DEFAULT_BUCKET_SIZE                      65536
#define POOL_DEFAULT_SINGLE_ALLOCATION_IN_BUCKET_SIZE 6554
#define POOL_DEFAULT_ALIGNMENT                        8

#define POOL_STAMP                                    0xCD

typedef struct {
    isize bucket_size;
    isize single_allocation_in_bucket_size;
    isize alignment;

    bool should_overwrite_blocks;
    bool should_free_blocks_on_reset;
} PoolDescription;

typedef struct {
    isize bucket_size;
    isize single_allocation_in_bucket_size;
    isize alignment;

    isize left;
    Block *current_block;
    u8    *current_ptr;

    Block *unused_blocks;
    Block *used_blocks;
    Block *first_used_memblock;
    Block *out_of_bounds_allocations;

    bool should_overwrite_blocks;
    bool should_free_blocks_on_reset;

    Allocator backing_allocator;
} Pool;

void _Pool_makeAndSwapBlocks(Pool *pool) {
    if (pool->backing_allocator.procedure == null) pool->backing_allocator = default_allocator;

    Block *new_block;
    if (pool->unused_blocks != null) {
        new_block           = pool->unused_blocks;
        pool->unused_blocks = pool->unused_blocks->next;
    } else {
        new_block = default_allocator.procedure(ALLOCATOR_MODE_ALLOCATE, &(AllocatorDescription) {
            .size_to_be_allocated_or_resized = pool->bucket_size,
        });

        // if (pool->should_overwrite_blocks == true && new_block != null) // Overwrite...
    }

    new_block->next   = pool->used_blocks;
    pool->used_blocks = new_block;
    if (pool->first_used_memblock == null) pool->first_used_memblock = new_block;

    pool->left          = pool->bucket_size - sizeof(Block);
    pool->current_block = new_block;
    pool->current_ptr   = pool->current_block->ptr;
}

void *Pool_allocatorProcedure(AllocatorMode mode, AllocatorDescription *description) {
    assert(description->impl != null);
    Pool *pool = cast(Pool*, description->impl);

    switch (mode) {
        case ALLOCATOR_MODE_FREE: {
            return cast(void*, cast(isize, true));
        } break;
        case ALLOCATOR_MODE_RESIZE: // through
        case ALLOCATOR_MODE_ALLOCATE: {
            assert(description->size_to_be_allocated_or_resized > 0);

            // @TODO
            // Detect whether a user makes a mistake and sets the backing allocator to
            // `Context`'s allocator, which may be set to this pool.
            if (pool->backing_allocator.procedure == null) pool->backing_allocator = default_allocator;

            bool is_that_allocation_too_big = description->size_to_be_allocated_or_resized >= pool->single_allocation_in_bucket_size;
            if (is_that_allocation_too_big == true) {
                Block *block = cast(Block*, alloc(description->size_to_be_allocated_or_resized));
                if (block == null) return null;

                block->next                     = pool->out_of_bounds_allocations;
                pool->out_of_bounds_allocations = block;
                
                u8 *chunk = block->ptr;

                return chunk;
            }

            isize aligned_ptr_position                = alignPtr(pool->current_ptr, pool->alignment);
            bool  is_there_enough_space_left_in_block = pool->left > description->size_to_be_allocated_or_resized + sizeof(Block) + aligned_ptr_position;
            if (is_there_enough_space_left_in_block == false) {
                _Pool_makeAndSwapBlocks(pool);

                if (pool->current_block == null) return null;
                aligned_ptr_position = alignPtr(pool->current_ptr, pool->alignment);
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

Pool Pool_create(PoolDescription *description) {
    return (Pool) {
        .bucket_size                      = POOL_DEFAULT_BUCKET_SIZE,
        .single_allocation_in_bucket_size = POOL_DEFAULT_SINGLE_ALLOCATION_IN_BUCKET_SIZE,
        .alignment                        = POOL_DEFAULT_ALIGNMENT,
        .should_overwrite_blocks          = false,
        .should_free_blocks_on_reset      = false,
    };
}

void Pool_destroy(Pool *pool) {
    *pool = (Pool) {
        0,
    };
}

void *Pool_get(Pool *pool, isize type_size_times_count) {
    return Pool_allocatorProcedure(ALLOCATOR_MODE_ALLOCATE, &(AllocatorDescription) {
        .size_to_be_allocated_or_resized = type_size_times_count,
        .impl                            = cast(void*, pool),
    });
}


Allocator Pool_getAllocator(Pool *pool) {
    return (Allocator) {
        .procedure = Pool_allocatorProcedure,
        .impl      = cast(void*, pool),
    };
}

#endif // INCLUDE_POOL_H
