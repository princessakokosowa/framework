core_function isize Allocator_align(isize size, isize alignment) {
    isize mask = ~(alignment - 1);
    isize _    = size + (alignment - 1);

    return _ & mask;
}

core_function isize Allocator_alignPtr(u8 *ptr, isize alignment) {
    isize mask   = alignment - 1;
    isize offset = cast(isize, ptr) & mask;
    isize _      = alignment - offset;

    return _ & mask;
}

core_function void *Allocator_allocWithAllocator(isize type_size_times_count, Allocator *allocator) {
    void *maybe_ptr = allocator->procedure(ALLOCATOR_MODE_ALLOCATE, &(AllocatorDescription){
        .size_to_be_allocated_or_resized = type_size_times_count,
        .impl                            = allocator->impl,
    });

    ensure(maybe_ptr != null);

    return maybe_ptr;
}

core_function void *Allocator_resizeWithAllocator(void *ptr, isize type_size_times_count, Allocator *allocator) {
    ensure(ptr != null);

    void *maybe_ptr = allocator->procedure(ALLOCATOR_MODE_RESIZE, &(AllocatorDescription){
        .ptr_to_be_resized_or_freed      = ptr,
        .size_to_be_allocated_or_resized = type_size_times_count,
        .impl                            = allocator->impl,
    });

    ensure(maybe_ptr != null);

    return maybe_ptr;
}

core_function void Allocator_freeWithAllocator(void *ptr, Allocator *allocator) {
    ensure(ptr != null);

    void *result_but_ptr = allocator->procedure(ALLOCATOR_MODE_FREE, &(AllocatorDescription) {
        .ptr_to_be_resized_or_freed = ptr,
        .impl                       = allocator->impl,
    });

    ensure(result_but_ptr != null);

    (void) result_but_ptr;
}
