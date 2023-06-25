////////////////////////////////
// Globals

#if BUILD_ROOT
    Context context = { 0, };
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

        case ALLOCATOR_MODE_CREATE_HEAP: {
            if (heap->ptr != null) return null;

            isize size                     = heap->size;
            isize size_roundup_granularity = megabytes(64);
            isize aligned_size             = Allocator_align(size, size_roundup_granularity);

            heap->ptr      = Allocator_reserve(aligned_size);
            heap->commited = heap->commit_granularity;

            Allocator_commit(heap->ptr, heap->commit_granularity);

            return cast(void *, cast(i64, true));
        } break;
        case ALLOCATOR_MODE_DESTROY_HEAP: {
            if (heap->ptr == null) return null;

            Allocator_release(heap->ptr, heap->size);

            return cast(void *, cast(i64, true));
        } break;

        case ALLOCATOR_MODE_IS_THIS_YOURS: through
        case ALLOCATOR_MODE_CAPS: through

        case ALLOCATOR_MODE_FREE: {
            // @NOTE
            //
            // Mental shortcut: reserving, commiting and releasing that memory is easy,
            // decommiting, not in the least.
            //
            // Decommiting means: fragmentation of memory blocks (or maybe not, but then
            // you would have to come up with a mechanism that flags the heaps used and
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
            // `Allocator_decommit(heap_block->ptr, heap_block->size)`
            //     ~ princessakokosowa, 22 June 2023

            return cast(void *, cast(i64, true));
        } break;
        case ALLOCATOR_MODE_RESIZE: through
        case ALLOCATOR_MODE_ALLOCATE: {
            void  *ptr           = description->ptr_to_be_resized_or_freed;
            bool  is_that_resize = ptr != null;
            isize size           = description->size_to_be_allocated_or_resized;
            isize aligned_size   = Allocator_align(size, heap->alignment);

            if (heap->ptr == null) return null;

            ensure(heap->occupied + aligned_size <= heap->size);

            u8 *chunk = heap->ptr + heap->occupied;

            heap->occupied += aligned_size;

            if (heap->commited < heap->occupied) {
                isize size_to_commit         = heap->occupied - heap->commited;
                isize aligned_size_to_commit = Allocator_align(size_to_commit, HEAP_DEFAULT_COMMIT_GRANULARITY);

                Allocator_commit(heap->ptr + heap->commited, aligned_size_to_commit);

                heap->commited += aligned_size_to_commit;
            }

            if (is_that_resize == true) return ptr;

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

    Allocator_createHeap(&context.heap_allocator);
}

core_function void Context_destroy(void) {
    Allocator_destroyHeap(&context.heap_allocator);

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
