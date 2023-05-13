#ifndef INCLUDE_TEMPORARY_STORAGE_H
#define INCLUDE_TEMPORARY_STORAGE_H

#include "foundation.h"
#include "allocator.h"

#define OVERFLOW_PAGE_COUNT 32768
typedef struct {
    u8 *ptr_to_heap;
} OverflowPage;

#define TEMPORARY_STORAGE_DEFAULT_SIZE      32768
#define TEMPORARY_STORAGE_DEFAULT_ALIGNMENT 8

typedef struct {
    isize alignment;

    isize size;
    isize occupied;
    isize last;
    u8    *ptr;

    Allocator *backing_allocator;
} TemporaryStorage;

TemporaryStorage temporary_storage;
Allocator        temporary_allocator;

void TemporaryStorage_setAllocators(Allocator *allocator) {
    temporary_storage.backing_allocator = allocator;
}

void *TemporaryStorage_allocatorProcedure(AllocatorMode mode, AllocatorDescription *description) {
    switch (mode) {
        case ALLOCATOR_MODE_FREE: {
            return cast(void*, cast(isize, true));
        } break;
        case ALLOCATOR_MODE_RESIZE: {
            assert(description->ptr_to_be_resized_or_freed != null);

            bool is_this_the_previous_allocation = temporary_storage.ptr + temporary_storage.last == cast(u8*, description->ptr_to_be_resized_or_freed);
            if (is_this_the_previous_allocation == true) {
                isize previous_allocation_size = temporary_storage.occupied - temporary_storage.last;
                isize allocation_size          = description->size_to_be_allocated_or_resized - previous_allocation_size;
                isize aligned_allocation_size  = align(allocation_size, ALLOCATOR_ALIGNMENT);

                assert(temporary_storage.occupied + aligned_allocation_size <= temporary_storage.size);

                temporary_storage.occupied += aligned_allocation_size;

                return description->ptr_to_be_resized_or_freed;
            }

            assert(temporary_storage.occupied + description->size_to_be_allocated_or_resized <= temporary_storage.size);

            isize aligned_allocation_size = align(description->size_to_be_allocated_or_resized, ALLOCATOR_ALIGNMENT);
            u8    *chunk                  = temporary_storage.ptr + temporary_storage.occupied;

            temporary_storage.occupied += aligned_allocation_size;

            for (isize i = 0; i < description->size_to_be_allocated_or_resized; i += 1) {
                chunk[i] = cast(u8*, description->ptr_to_be_resized_or_freed)[i];
            }

            return chunk;
        } break;
        case ALLOCATOR_MODE_ALLOCATE: {
            assert(temporary_storage.occupied + description->size_to_be_allocated_or_resized <= temporary_storage.size);

            if (temporary_storage.ptr == null) temporary_storage.ptr = allocUsingAllocator(temporary_storage.size, temporary_storage.backing_allocator);

            isize aligned_allocation_size = align(description->size_to_be_allocated_or_resized, ALLOCATOR_ALIGNMENT);
            u8    *chunk                  = temporary_storage.ptr + temporary_storage.occupied;

            temporary_storage.last     =  temporary_storage.occupied;
            temporary_storage.occupied += aligned_allocation_size;

            return chunk;
        } break;
    }

    unreachable();
}

void TemporaryStorage_create(void) {
    temporary_storage = (TemporaryStorage) {
        .alignment = TEMPORARY_STORAGE_DEFAULT_ALIGNMENT,
        .size      = TEMPORARY_STORAGE_DEFAULT_SIZE,
    };

    temporary_allocator = (Allocator) {
        .procedure = &TemporaryStorage_allocatorProcedure,
        // .impl      = cast(void*, &temporary_storage),
    };
}

void TemporaryStorage_destroy(void) {
    temporary_allocator = (Allocator) {
        0,
    };

    freeUsingAllocator(temporary_storage.ptr, temporary_storage.backing_allocator);

    temporary_storage = (TemporaryStorage) {
        0,
    };
}

void TemporaryStorage_reset(void) {
    temporary_storage.last     = 0;
    temporary_storage.occupied = 0;
}

#endif // INCLUDE_TEMPORARY_STORAGE_H
