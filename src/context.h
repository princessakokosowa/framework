#ifndef INCLUDE_CONTEXT_H
#define INCLUDE_CONTEXT_H

#include "foundation.h"
#include "allocator.h"
#include "temporary_storage.h"

typedef struct {
    Allocator *allocator;
    Allocator *remembered_allocator;
} Context;

Context context;

void* defaultAllocatorProcedure(AllocatorMode mode, AllocatorDescription *description) {
    assert(description->ptr_to_heap == null);

    if      (mode == ALLOCATOR_MODE_ALLOCATE) return cast(void*,           HeapAlloc(  GetProcessHeap(), 0,                                                        description->size_to_be_allocated_or_resized));
    else if (mode == ALLOCATOR_MODE_RESIZE)   return cast(void*,           HeapReAlloc(GetProcessHeap(), 0, cast(LPVOID, description->ptr_to_be_resized_or_freed), description->size_to_be_allocated_or_resized));
    else if (mode == ALLOCATOR_MODE_FREE)     return cast(void*, cast(i64, HeapFree(   GetProcessHeap(), 0, cast(LPVOID, description->ptr_to_be_resized_or_freed)                                             )));

    _unreachable();
}

Allocator default_allocator = (Allocator) {
    .procedure   = &defaultAllocatorProcedure,
    .ptr_to_heap = null,
};

void contextCreate(void) {
    context = (Context) {
        .allocator = &default_allocator,
    };

    temporaryStorageCreate();
}

void contextRememberAllocators(void) {
    context.remembered_allocator = context.allocator;
}

void contextSetAllocators(Allocator *allocator) {
    contextRememberAllocators();

    context.allocator = allocator;
}

void contextRemindAllocators(void) {
    context.allocator            = context.remembered_allocator;
    context.remembered_allocator = null;
}

int contextDestroy(void) {
    temporaryStorageDestroy();

    context = (Context) {
        0,
    };

    return 0;
}

void* _alloc(isize type_size_times_count) {
    void *maybe_ptr = context.allocator->procedure(ALLOCATOR_MODE_ALLOCATE, &(AllocatorDescription){
        .size_to_be_allocated_or_resized = type_size_times_count,
        .ptr_to_heap                     = context.allocator->ptr_to_heap,
    });

    assert(maybe_ptr != null);

    return maybe_ptr;
}

void* _resize(void *ptr, isize type_size_times_count) {
    assert(ptr != null);

    void *maybe_ptr = context.allocator->procedure(ALLOCATOR_MODE_RESIZE, &(AllocatorDescription){
        .ptr_to_be_resized_or_freed      = ptr,
        .size_to_be_allocated_or_resized = type_size_times_count,
        .ptr_to_heap                     = context.allocator->ptr_to_heap,
    });

    assert(maybe_ptr != null);

    return maybe_ptr;
}

void _free(void *ptr) {
    assert(ptr != null);

    void *result_but_ptr = context.allocator->procedure(ALLOCATOR_MODE_FREE, &(AllocatorDescription) {
        .ptr_to_be_resized_or_freed = ptr,
        .ptr_to_heap                = context.allocator->ptr_to_heap,
    });

    assert(result_but_ptr != null);

    (void) result_but_ptr;
}

#endif // INCLUDE_CONTEXT_H
