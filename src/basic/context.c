#if BUILD_ROOT
    per_thread Context   context;
    per_thread Allocator default_allocator;
#endif

core_function void *Default_allocatorProcedure(AllocatorMode mode, AllocatorDescription *description) {
    if      (mode == ALLOCATOR_MODE_ALLOCATE) return cast(void *,           HeapAlloc(  GetProcessHeap(), 0,                                                        description->size_to_be_allocated_or_resized));
    else if (mode == ALLOCATOR_MODE_RESIZE)   return cast(void *,           HeapReAlloc(GetProcessHeap(), 0, cast(LPVOID, description->ptr_to_be_resized_or_freed), description->size_to_be_allocated_or_resized));
    else if (mode == ALLOCATOR_MODE_FREE)     return cast(void *, cast(i64, HeapFree(   GetProcessHeap(), 0, cast(LPVOID, description->ptr_to_be_resized_or_freed)                                             )));

    unreachable();
}

core_function void Context_create(void) {
    context = (Context) {
        .allocator = &default_allocator,
    };
}

core_function void Context_destroy(void) {
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
