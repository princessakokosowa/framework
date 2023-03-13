#ifndef INCLUDE_CONTEXT_H
#define INCLUDE_CONTEXT_H

#include "foundation.h"
#include "allocator.h"
#include "temporary_storage.h"

typedef struct {
    AllocatorProcedure* allocator;
    AllocatorProcedure* remembered_allocator;
} Context;

Context context;

static inline void* defaultAllocatorProcedure(AllocatorMode mode, AllocatorDescription* description) {
    if      (mode == ALLOCATOR_MODE_ALLOCATE) return cast(void*,           HeapAlloc(  GetProcessHeap(), 0,                                                        description->size_to_be_allocated_or_resized));
    else if (mode == ALLOCATOR_MODE_RESIZE)   return cast(void*,           HeapReAlloc(GetProcessHeap(), 0, cast(LPVOID, description->ptr_to_be_resized_or_freed), description->size_to_be_allocated_or_resized));
    else if (mode == ALLOCATOR_MODE_FREE)     return cast(void*, cast(i64, HeapFree(   GetProcessHeap(), 0, cast(LPVOID, description->ptr_to_be_resized_or_freed)                                             )));

    _unreachable();
}

static inline void contextCreate() {
    context = (Context) {
        .allocator = &defaultAllocatorProcedure,
    };

    temporaryStorageCreate();
}

static inline void contextRememberAllocators() {
    context.remembered_allocator = context.allocator;
}

static inline void contextSetAllocators(AllocatorProcedure* allocator) {
    contextRememberAllocators();

    context.allocator = allocator;
}

static inline void contextRemindAllocators() {
    context.allocator            = context.remembered_allocator;
    context.remembered_allocator = null;
}

static inline int contextDestroy() {
    temporaryStorageDestroy();

    context = (Context) {
        0,
    };

    return 0;
}

static inline void* _alloc(isize type_size_times_count) {
    void* maybe_ptr = context.allocator(ALLOCATOR_MODE_ALLOCATE, &(AllocatorDescription){
        .size_to_be_allocated_or_resized = type_size_times_count,
    });

    assert(maybe_ptr != null);

    return maybe_ptr;
}

static inline void* _resize(void* ptr, isize type_size_times_count) {
    assert(ptr != null);

    void* maybe_ptr = context.allocator(ALLOCATOR_MODE_RESIZE, &(AllocatorDescription){
        .ptr_to_be_resized_or_freed      = ptr,
        .size_to_be_allocated_or_resized = type_size_times_count,
    });

    assert(maybe_ptr != null);

    return maybe_ptr;
}

static inline void _free(void* ptr) {
    assert(ptr != null);

    void* result_but_ptr = context.allocator(ALLOCATOR_MODE_FREE, &(AllocatorDescription) {
        .ptr_to_be_resized_or_freed = ptr,
    });

    assert(result_but_ptr != null);
}

#endif // INCLUDE_CONTEXT_H
