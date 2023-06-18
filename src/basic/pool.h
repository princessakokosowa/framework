#ifndef POOL_H
#define POOL_H

#include "basic.h"

typedef struct Block Block;
struct Block {
    Block *next;
    void  *ptr;
};

enum {
    POOL_DEFAULT_BUCKET_SIZE                      = 65536,
    POOL_DEFAULT_SINGLE_ALLOCATION_IN_BUCKET_SIZE = 6554,
    POOL_DEFAULT_ALIGNMENT                        = 8,

    POOL_STAMP                                    = 0xCD,
};

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

    Allocator *backing_allocator;
} Pool;

core_function void Pool_setAllocators(Pool *pool, Allocator *allocator);
core_function void Pool_makeAndSwapBlocks(Pool *pool);
core_function void *Pool_allocatorProcedure(AllocatorMode mode, AllocatorDescription *description);
core_function Pool Pool_create(PoolDescription *description);
core_function void Pool_destroy(Pool *pool);
core_function void *Pool_get(Pool *pool, isize type_size_times_count);
core_function Allocator Pool_getAllocator(Pool *pool);

#endif // POOL_H
