core_function void Arena_setAllocators(Arena *arena, Allocator *allocator) {
    arena->backing_allocator = allocator;
}

core_function void *Arena_allocatorProcedure(AllocatorMode mode, AllocatorDescription *description) {
    ensure(description->impl != null);

    Arena *arena = cast(Arena *, description->impl);

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
            bool is_that_resize                  = description->ptr_to_be_resized_or_freed != null;
            bool is_this_the_previous_allocation = arena->ptr + arena->last == cast(u8 *, description->ptr_to_be_resized_or_freed);
            if (is_that_resize == true && is_this_the_previous_allocation == true) {
                isize previous_allocation_size = arena->occupied - arena->last;
                isize allocation_size          = description->size_to_be_allocated_or_resized - previous_allocation_size;
                isize aligned_allocation_size  = Allocator_align(allocation_size, ALLOCATOR_ALIGNMENT);

                ensure(arena->occupied + aligned_allocation_size <= arena->size);

                arena->occupied += aligned_allocation_size;

                return description->ptr_to_be_resized_or_freed;
            }

            ensure(arena->occupied + description->size_to_be_allocated_or_resized <= arena->size);

            if (arena->backing_allocator == null) {
                bool are_we_already_set_in_context = context.allocator->impl == arena;
                if (are_we_already_set_in_context == true) Arena_setAllocators(arena, &default_allocator);
                else                                       Arena_setAllocators(arena, context.allocator);
            }

            if (arena->ptr == null) arena->ptr = allocWithAllocator(arena->size, arena->backing_allocator);

            isize aligned_allocation_size = Allocator_align(description->size_to_be_allocated_or_resized, ALLOCATOR_ALIGNMENT);
            u8    *chunk                  = arena->ptr + arena->occupied;

            if (is_that_resize == false) arena->last = arena->occupied;

            arena->occupied += aligned_allocation_size;

            return chunk;
        } break;
    }

    unreachable();
}

core_function Arena Arena_create(ArenaDescription *description) {
    return (Arena) {
        .alignment         = description->alignment ? description->alignment : ARENA_DEFAULT_ALIGNMENT,
        .size              = description->size      ? description->size      : ARENA_DEFAULT_SIZE,
        .should_overwrite  = description->should_overwrite,
        .backing_allocator = description->backing_allocator,
    };
}

core_function void Arena_destroy(Arena *arena) {
    if (arena->ptr != null) freeWithAllocator(arena->ptr, arena->backing_allocator);

    *arena = (Arena) {
        0,
    };
}

core_function void *Arena_get(Arena *arena, isize type_size_times_count) {
    return Arena_allocatorProcedure(ALLOCATOR_MODE_ALLOCATE, &(AllocatorDescription) {
        .size_to_be_allocated_or_resized = type_size_times_count,
        .impl                            = cast(void *, arena),
    });
}

core_function Allocator Arena_getAllocator(Arena *arena) {
    return (Allocator) {
        .procedure = Arena_allocatorProcedure,
        .impl      = cast(void *, arena),
    };
}
