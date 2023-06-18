#ifndef INCLUDE_CONTEXT_H
#define INCLUDE_CONTEXT_H

// Do we actually want to do it that way? I mean, our call stack _probably_ won't be
// deeper than 32 procedure calls, or maybe it will? I don't know, man, but let's go that
// way and see whether it works for that use case.
//     ~ princessakokosowa, 3rd of April 2023

enum {
    MAX_REMEMBERED_LIST_COUNT = 128,
};

typedef struct {
    isize     remembered_count;
    Allocator *remembered_list[MAX_REMEMBERED_LIST_COUNT];

    Allocator *allocator;
} Context;

per_thread Context context;

core_function
void *Default_allocatorProcedure(AllocatorMode mode, AllocatorDescription *description) {
    if      (mode == ALLOCATOR_MODE_ALLOCATE) return cast(void *,           HeapAlloc(  GetProcessHeap(), 0,                                                        description->size_to_be_allocated_or_resized));
    else if (mode == ALLOCATOR_MODE_RESIZE)   return cast(void *,           HeapReAlloc(GetProcessHeap(), 0, cast(LPVOID, description->ptr_to_be_resized_or_freed), description->size_to_be_allocated_or_resized));
    else if (mode == ALLOCATOR_MODE_FREE)     return cast(void *, cast(i64, HeapFree(   GetProcessHeap(), 0, cast(LPVOID, description->ptr_to_be_resized_or_freed)                                             )));

    unreachable();
}

Allocator default_allocator = (Allocator) {
    .procedure = &Default_allocatorProcedure,
};

core_function
void Context_create(void) {
    context = (Context) {
        .allocator = &default_allocator,
    };

    TemporaryStorage_create();
    TemporaryStorage_setAllocators(&default_allocator);
}

core_function
int Context_destroy(void) {
    TemporaryStorage_destroy();

    context = (Context) {
        0,
    };

    return 0;
}

core_function
void Context_rememberAllocators(void) {
    ensure(MAX_REMEMBERED_LIST_COUNT != context.remembered_count);

    context.remembered_list[context.remembered_count] = context.allocator;
    context.remembered_count                          += 1;
}

core_function
void Context_setAllocators(Allocator *allocator) {
    Context_rememberAllocators();

    context.allocator = allocator;
}

core_function
void Context_remindAllocators(void) {
    ensure(context.remembered_count != 0);

    context.remembered_count                          -= 1;
    context.allocator                                 = context.remembered_list[context.remembered_count];
    context.remembered_list[context.remembered_count] = null;
}

core_function
void *Context_alloc(isize type_size_times_count) {
    void *maybe_ptr = context.allocator->procedure(ALLOCATOR_MODE_ALLOCATE, &(AllocatorDescription){
        .size_to_be_allocated_or_resized = type_size_times_count,
        .impl                            = context.allocator->impl,
    });

    ensure(maybe_ptr != null);

    return maybe_ptr;
}

core_function
void *Context_resize(void *ptr, isize type_size_times_count) {
    ensure(ptr != null);

    void *maybe_ptr = context.allocator->procedure(ALLOCATOR_MODE_RESIZE, &(AllocatorDescription){
        .ptr_to_be_resized_or_freed      = ptr,
        .size_to_be_allocated_or_resized = type_size_times_count,
        .impl                            = context.allocator->impl,
    });

    ensure(maybe_ptr != null);

    return maybe_ptr;
}

core_function
void Context_free(void *ptr) {
    ensure(ptr != null);

    void *result_but_ptr = context.allocator->procedure(ALLOCATOR_MODE_FREE, &(AllocatorDescription) {
        .ptr_to_be_resized_or_freed = ptr,
        .impl                       = context.allocator->impl,
    });

    ensure(result_but_ptr != null);

    (void) result_but_ptr;
}

core_function
void *Context_allocUsingAllocator(isize type_size_times_count, Allocator *allocator) {
    void *maybe_ptr = allocator->procedure(ALLOCATOR_MODE_ALLOCATE, &(AllocatorDescription){
        .size_to_be_allocated_or_resized = type_size_times_count,
        .impl                            = allocator->impl,
    });

    ensure(maybe_ptr != null);

    return maybe_ptr;
}

core_function
void *Context_resizeUsingAllocator(void *ptr, isize type_size_times_count, Allocator *allocator) {
    ensure(ptr != null);

    void *maybe_ptr = allocator->procedure(ALLOCATOR_MODE_RESIZE, &(AllocatorDescription){
        .ptr_to_be_resized_or_freed      = ptr,
        .size_to_be_allocated_or_resized = type_size_times_count,
        .impl                            = allocator->impl,
    });

    ensure(maybe_ptr != null);

    return maybe_ptr;
}

core_function
void Context_freeUsingAllocator(void *ptr, Allocator *allocator) {
    ensure(ptr != null);

    void *result_but_ptr = allocator->procedure(ALLOCATOR_MODE_FREE, &(AllocatorDescription) {
        .ptr_to_be_resized_or_freed = ptr,
        .impl                       = allocator->impl,
    });

    ensure(result_but_ptr != null);

    (void) result_but_ptr;
}

#endif // INCLUDE_CONTEXT_H
