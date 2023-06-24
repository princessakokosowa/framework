////////////////////////////////
// Globals

#if BUILD_ROOT
    Context   context           = { 0, };
#endif

#if !defined(Allocator_reserve)
    #error Allocator_reserve must be defined to use base memory.
#endif

#if !defined(Allocator_release)
    #error Allocator_release must be defined to use base memory.
#endif

#if !defined(Allocator_commit)
    #error Allocator_commit must be defined to use base memory.
#endif

#if !defined(Allocator_decommit)
    #error Allocator_decommit must be defined to use base memory.
#endif

core_function void *Heap_allocatorProcedure(AllocatorMode mode, AllocatorDescription *description) {
    ensure(description->impl != null);

    Heap *heap = cast(Heap *, description->impl);

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
            bool success = true; // Allocator_release(description->ptr_to_be_resized_or_freed, 0);

            return cast(void *, cast(i64, success));
        } break;
        case ALLOCATOR_MODE_RESIZE: through
        case ALLOCATOR_MODE_ALLOCATE: {
            // isize size                     = description->size_to_be_allocated_or_resized;
            // isize size_roundup_granularity = megabytes(64);
            //
            // size += size_roundup_granularity - 1;
            // size -= size % size_roundup_granularity;

            if (heap->ptr == null) {
                heap->ptr = Allocator_reserve(heap->size);

                Allocator_commit(heap->ptr, heap->commit_granularity);

                heap->size_per_commit = heap->commit_granularity;
            }

            ensure(heap->occupied + description->size_to_be_allocated_or_resized <= heap->size);

            isize aligned_allocation_size = Allocator_align(description->size_to_be_allocated_or_resized, heap->alignment);
            u8    *chunk                  = heap->ptr + heap->occupied;

            // if (is_that_resize == false) heap->last = heap->occupied;

            heap->occupied += aligned_allocation_size;

            if (heap->size_per_commit < heap->occupied) {
                isize size_to_commit = heap->occupied - heap->size_per_commit;

                size_to_commit += HEAP_DEFAULT_COMMIT_GRANULARITY - 1;
                size_to_commit -= size_to_commit % HEAP_DEFAULT_COMMIT_GRANULARITY;

                Allocator_commit(heap->ptr + heap->size_per_commit, size_to_commit);

                heap->size_per_commit += size_to_commit;
            }

            return chunk;
        } break;
    }

    unreachable();
}

core_function Heap Heap_create(HeapDescription *description) {
    return (Heap) {
        .alignment          = description->alignment,
        .size               = description->size,
        .commit_granularity = description->commit_granularity,
        .decommit_threshold = description->decommit_threshold,
    };
}

core_function void Heap_destroy(Heap *heap) {
    if (heap->ptr != null) Allocator_release(heap->ptr, heap->size);

    *heap = (Heap) {
        0,
    };
}

core_function void *Heap_get(Heap *heap, isize type_size_times_count) {
    return Heap_allocatorProcedure(ALLOCATOR_MODE_ALLOCATE, &(AllocatorDescription) {
        .size_to_be_allocated_or_resized = type_size_times_count,
        .impl                            = cast(void *, heap),
    });
}

core_function Allocator Heap_getAllocator(Heap *heap) {
    return (Allocator) {
        .procedure = Heap_allocatorProcedure,
        .impl      = cast(void *, heap),
    };
}

core_function void Context_create(void) {
    context = (Context) {
        .heap           = Heap_create(&(HeapDescription) {
            .alignment          = HEAP_DEFAULT_ALIGNMENT,
            .size               = HEAP_DEFAULT_SIZE,
            .commit_granularity = HEAP_DEFAULT_COMMIT_GRANULARITY,
            .decommit_threshold = HEAP_DEFAULT_DECOMMIT_THRESHOLD,
        }),
        .heap_allocator = Heap_getAllocator(&context.heap),
        .allocator      = &context.heap_allocator,
    };
}

core_function void Context_destroy(void) {
    Heap_destroy(&context.heap);

    context = (Context) {
        0,
    };
}

core_function void Context_rememberAllocators(void) {
    ensure(MAX_REMEMBERED_LIST_COUNT != context.remembered_count);

    context.remembered_list[context.remembered_count] = context.allocator;
    context.remembered_count                          += 1;
}

core_function void Context_setAllocators(Allocator *allocator) {
    Context_rememberAllocators();

    context.allocator = allocator;
}

core_function void Context_remindAllocators(void) {
    ensure(context.remembered_count != 0);

    context.remembered_count                          -= 1;
    context.allocator                                 = context.remembered_list[context.remembered_count];
    context.remembered_list[context.remembered_count] = null;
}

core_function void *Context_alloc(isize type_size_times_count) {
    return Allocator_allocWithAllocator(type_size_times_count, context.allocator);
}

core_function void *Context_resize(void *ptr, isize type_size_times_count) {
    return Allocator_resizeWithAllocator(ptr, type_size_times_count, context.allocator);
}

core_function void Context_free(void *ptr) {
    Allocator_freeWithAllocator(ptr, context.allocator);
}
