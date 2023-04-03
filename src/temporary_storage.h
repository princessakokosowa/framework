#ifndef INCLUDE_TEMPORARY_STORAGE_H
#define INCLUDE_TEMPORARY_STORAGE_H

#include "foundation.h"
#include "allocator.h"

#define OVERFLOW_PAGE_COUNT 16384
typedef struct {
    u8 *ptr_to_heap;
} OverflowPage;

#define TEMPORARY_STORAGE_COUNT 16384
typedef struct {
    i16 occupied;
    i16 last;
    u8  *ptr_to_heap;
} TemporaryStorage;

TemporaryStorage temporary_storage;
Allocator        temporary_allocator;

void* temporaryStorageAllocatorProcedure(AllocatorMode mode, AllocatorDescription *description) {
    assert(description->ptr_to_heap == null);

    if (mode == ALLOCATOR_MODE_ALLOCATE) {
        assert(temporary_storage.occupied + description->size_to_be_allocated_or_resized <= TEMPORARY_STORAGE_COUNT);

        isize const aligned_allocation_size = allocatorAlign(description->size_to_be_allocated_or_resized, ALLOCATOR_ALIGNMENT);
        u8          *chunk                  = temporary_storage.ptr_to_heap + temporary_storage.occupied;

        temporary_storage.last     =  temporary_storage.occupied;
        temporary_storage.occupied += aligned_allocation_size;

        return cast(void*, chunk);
    } else if (mode == ALLOCATOR_MODE_RESIZE) {
        assert(description->ptr_to_be_resized_or_freed != null);

        bool const is_this_the_previous_allocation = temporary_storage.ptr_to_heap + temporary_storage.last == cast(u8*, description->ptr_to_be_resized_or_freed);

        if (is_this_the_previous_allocation == true) {
            isize const previous_allocation_size = temporary_storage.occupied - temporary_storage.last;
            isize const allocation_size          = description->size_to_be_allocated_or_resized - previous_allocation_size;
            isize const aligned_allocation_size  = allocatorAlign(allocation_size, ALLOCATOR_ALIGNMENT);

            assert(temporary_storage.occupied + aligned_allocation_size <= TEMPORARY_STORAGE_COUNT);
            
            temporary_storage.occupied += aligned_allocation_size;

            return description->ptr_to_be_resized_or_freed;
        }

        assert(temporary_storage.occupied + description->size_to_be_allocated_or_resized <= TEMPORARY_STORAGE_COUNT);

        isize const aligned_allocation_size = allocatorAlign(description->size_to_be_allocated_or_resized, ALLOCATOR_ALIGNMENT);
        u8          *chunk                  = temporary_storage.ptr_to_heap + temporary_storage.occupied;

        temporary_storage.occupied += aligned_allocation_size;

        for (isize i = 0; i < description->size_to_be_allocated_or_resized; i += 1) {
            chunk[i] = cast(u8*, description->ptr_to_be_resized_or_freed)[i];
        }

        return cast(void*, chunk);
    } else {
        return cast(void*, cast(i64, true));
    }

    unreachable();
}

void temporaryStorageCreate(void) {
    temporary_storage = (TemporaryStorage) {
        .ptr_to_heap = alloc(TEMPORARY_STORAGE_COUNT),
    };

    temporary_allocator = (Allocator) {
        .procedure = &temporaryStorageAllocatorProcedure,
    };
}

void temporaryStorageDestroy(void) {
    temporary_allocator = (Allocator) {
        0,
    };

    free(temporary_storage.ptr_to_heap);

    temporary_storage = (TemporaryStorage) {
        0,
    };
}

void temporaryStorageReset(void) {
    temporary_storage.last     = 0;
    temporary_storage.occupied = 0;
}

#endif // INCLUDE_TEMPORARY_STORAGE_H
