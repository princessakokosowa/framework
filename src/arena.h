#ifndef INCLUDE_ARENA_ALLOCATOR_H
#define INCLUDE_ARENA_ALLOCATOR_H

#include "foundation.h"
#include "allocator.h"

#define AREA_STORAGE_DEFAULT_COUNT 128

typedef struct {
    isize count;
    void* ptr_to_heap;
} ArenaDescription;

typedef struct {
    bool  has_user_provided_heap;
    isize count;
    isize occupied;
    isize last;

    u8* ptr_to_heap;
} Arena;

void* arenaAllocatorProcedure(AllocatorMode mode, AllocatorDescription *description) {
    Arena *arena = cast(Arena*, description->impl);

    if (mode == ALLOCATOR_MODE_ALLOCATE) {
        assert(arena->occupied + description->size_to_be_allocated_or_resized <= arena->count);

        isize const aligned_allocation_size = allocatorAlign(description->size_to_be_allocated_or_resized, ALLOCATOR_ALIGNMENT);
        u8          *chunk                  = arena->ptr_to_heap + arena->occupied;

        arena->last     =  arena->occupied;
        arena->occupied += aligned_allocation_size;

        return cast(void*, chunk);
    } else if (mode == ALLOCATOR_MODE_RESIZE) {
        assert(description->ptr_to_be_resized_or_freed != null);

        bool const is_this_the_previous_allocation = arena->ptr_to_heap + arena->last == cast(u8*, description->ptr_to_be_resized_or_freed);

        if (is_this_the_previous_allocation == true) {
            isize const previous_allocation_size = arena->occupied - arena->last;
            isize const allocation_size          = description->size_to_be_allocated_or_resized - previous_allocation_size;
            isize const aligned_allocation_size  = allocatorAlign(allocation_size, ALLOCATOR_ALIGNMENT);

            assert(arena->occupied + aligned_allocation_size <= arena->count);

            arena->occupied += aligned_allocation_size;
        
            return description->ptr_to_be_resized_or_freed;
        }

        assert(arena->occupied + description->size_to_be_allocated_or_resized <= arena->count);

        isize const aligned_allocation_size = allocatorAlign(description->size_to_be_allocated_or_resized, ALLOCATOR_ALIGNMENT);
        u8          *chunk                  = arena->ptr_to_heap + arena->occupied;

        arena->occupied += aligned_allocation_size;

        for (isize i = 0; i < description->size_to_be_allocated_or_resized; i += 1) {
            chunk[i] = cast(u8*, description->ptr_to_be_resized_or_freed)[i];
        }

        return cast(void*, chunk);
    } else {
        return cast(void*, cast(i64, true));
    }

    unreachable();
}

Arena arenaCreate(ArenaDescription *description) {
    u8   *ptr_to_heap           = null;
    bool has_user_provided_heap = description->ptr_to_heap != null;
    if (!has_user_provided_heap) ptr_to_heap = cast(u8*, alloc(AREA_STORAGE_DEFAULT_COUNT));
    else                         ptr_to_heap = description->ptr_to_heap;
    
    return (Arena) {
        .has_user_provided_heap = has_user_provided_heap,
        .count                  = has_user_provided_heap ? description->count : AREA_STORAGE_DEFAULT_COUNT,
        .ptr_to_heap            = ptr_to_heap,
    };
}

void arenaDestroy(Arena *arena) {
    if (!arena->has_user_provided_heap) free(arena->ptr_to_heap);

    *arena = (Arena) {
        0,
    };
}

Allocator arenaGetAllocator(Arena *arena) {
    return (Allocator) {
        .procedure = arenaAllocatorProcedure,
        .impl      = cast(void*, arena),
    };
}

#endif // INCLUDE_ARENA_ALLOCATOR_H
