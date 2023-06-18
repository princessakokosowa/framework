#if BUILD_ROOT
    per_thread TemporaryStorage temporary_storage;
    per_thread Allocator        temporary_storage_allocator;
#endif

core_function void *TemporaryStorage_allocatorProcedure(AllocatorMode mode, AllocatorDescription *description) {
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
            return cast(void *, cast(isize, true));
        } break;

        case ALLOCATOR_MODE_RESIZE: through
        case ALLOCATOR_MODE_ALLOCATE: {
            bool is_that_resize                  = description->ptr_to_be_resized_or_freed != null;
            bool is_this_the_previous_allocation = temporary_storage.ptr + temporary_storage.last == cast(u8*, description->ptr_to_be_resized_or_freed);
            if (is_that_resize == true && is_this_the_previous_allocation == true) {
                isize previous_allocation_size = temporary_storage.occupied - temporary_storage.last;
                isize allocation_size          = description->size_to_be_allocated_or_resized - previous_allocation_size;
                isize aligned_allocation_size  = Allocator_align(allocation_size, ALLOCATOR_ALIGNMENT);

                ensure(temporary_storage.occupied + aligned_allocation_size <= temporary_storage.size);

                temporary_storage.occupied += aligned_allocation_size;

                return description->ptr_to_be_resized_or_freed;
            }

            ensure(temporary_storage.occupied + description->size_to_be_allocated_or_resized <= temporary_storage.size);

            if (temporary_storage.ptr == null) temporary_storage.ptr = allocWithAllocator(temporary_storage.size, temporary_storage.backing_allocator);

            isize aligned_allocation_size = Allocator_align(description->size_to_be_allocated_or_resized, ALLOCATOR_ALIGNMENT);
            u8    *chunk                  = temporary_storage.ptr + temporary_storage.occupied;

            if (is_that_resize == false) temporary_storage.last = temporary_storage.occupied;

            temporary_storage.occupied += aligned_allocation_size;

            return chunk;
        } break;
    }

    unreachable();
}

core_function void TemporaryStorage_create(void) {
    temporary_storage = (TemporaryStorage) {
        .alignment         = TEMPORARY_STORAGE_DEFAULT_ALIGNMENT,
        .size              = TEMPORARY_STORAGE_DEFAULT_SIZE,
        .backing_allocator = &default_allocator,
    };

    temporary_storage_allocator = (Allocator) {
        .procedure = &TemporaryStorage_allocatorProcedure,
        // .impl      = cast(void *, &temporary_storage),
    };
}

core_function void TemporaryStorage_destroy(void) {
    temporary_storage_allocator = (Allocator) {
        0,
    };

    if (temporary_storage.ptr) freeWithAllocator(temporary_storage.ptr, temporary_storage.backing_allocator);

    temporary_storage = (TemporaryStorage) {
        0,
    };
}

core_function void TemporaryStorage_reset(void) {
    temporary_storage.last     = 0;
    temporary_storage.occupied = 0;
}
