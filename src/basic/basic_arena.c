core_function void Arena_setAllocators(Arena *arena, Allocator *allocator) {
    arena->backing_allocator = allocator;
}

core_function void *Arena_allocatorProcedure(AllocatorMode mode, AllocatorDescription *description) {
    assert(description->impl != null);

    Arena *arena = cast(Arena *, description->impl);

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
            return cast(void*, cast(isize, true));
        } break;
        case AllocatorMode_Resize: through
        case AllocatorMode_Allocate: {
            bool is_that_resize                  = description->ptr_to_be_resized_or_freed != null;
            bool is_this_the_previous_allocation = arena->ptr + arena->last == cast(u8 *, description->ptr_to_be_resized_or_freed);
            if (is_that_resize == true && is_this_the_previous_allocation == true) {
                isize previous_allocation_size = arena->occupied - arena->last;
                isize allocation_size          = description->size_to_be_allocated_or_resized - previous_allocation_size;
                isize aligned_allocation_size  = align(allocation_size, Allocator_Alignment);

                assert(arena->occupied + aligned_allocation_size <= arena->size);

                arena->occupied += aligned_allocation_size;

                return description->ptr_to_be_resized_or_freed;
            }

            assert(arena->occupied + description->size_to_be_allocated_or_resized <= arena->size);

            if (arena->backing_allocator == null) {
                bool are_we_already_set_in_context = Context_getAllocator()->impl == arena;
                if (are_we_already_set_in_context == true) Arena_setAllocators(arena, Context_getDefaultAllocator());
                else                                       Arena_setAllocators(arena, Context_getAllocator());
            }

            if (arena->ptr == null) arena->ptr = Allocator_alloc(arena->size, arena->backing_allocator);

            isize aligned_allocation_size = align(description->size_to_be_allocated_or_resized, Allocator_Alignment);
            u8    *chunk                  = arena->ptr + arena->occupied;

            if (is_that_resize == false) arena->last = arena->occupied;

            arena->occupied += aligned_allocation_size;

            return chunk;
        } break;
    }

    unreachable();
}

core_function Arena Arena_init(ArenaDescription *description) {
    return (Arena) {
        .alignment         = description->alignment ? description->alignment : Arena_DEFAULT_ALIGNMENT,
        .size              = description->size      ? description->size      : Arena_DEFAULT_SIZE,
        .should_overwrite  = description->should_overwrite,
        .backing_allocator = description->backing_allocator,
    };
}

core_function void Arena_deinit(Arena *arena) {
    if (arena->ptr != null) Allocator_release(arena->ptr, arena->backing_allocator);

    *arena = (Arena) {
        0,
    };
}

core_function void *Arena_alloc(Arena *arena, isize type_size_times_count) {
    return Arena_allocatorProcedure(AllocatorMode_Allocate, &(AllocatorDescription) {
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
