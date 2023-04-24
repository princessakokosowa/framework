#ifndef INCLUDE_CONTEXT_H
#define INCLUDE_CONTEXT_H

#include "foundation.h"
#include "allocator.h"
#include "temporary_storage.h"

// Do we actually want to do it that way? I mean, our call stack _probably_ won't be
// deeper than 32 procedure calls, or maybe it will? I don't know, man, but let's go that
// way and see whether it works for that use case.
//     ~ princessakokosowa, 3rd of April 2023
#define MAX_REMEMBERED_LIST_COUNT 32
typedef struct {
    isize     remembered_count;
    Allocator *remembered_list[MAX_REMEMBERED_LIST_COUNT];

    Allocator *allocator;
} Context;

Context context;

void *defaultAllocatorProcedure(AllocatorMode mode, AllocatorDescription *description) {
    if      (mode == ALLOCATOR_MODE_ALLOCATE) return cast(void*,           HeapAlloc(  GetProcessHeap(), 0,                                                        description->size_to_be_allocated_or_resized));
    else if (mode == ALLOCATOR_MODE_RESIZE)   return cast(void*,           HeapReAlloc(GetProcessHeap(), 0, cast(LPVOID, description->ptr_to_be_resized_or_freed), description->size_to_be_allocated_or_resized));
    else if (mode == ALLOCATOR_MODE_FREE)     return cast(void*, cast(i64, HeapFree(   GetProcessHeap(), 0, cast(LPVOID, description->ptr_to_be_resized_or_freed)                                             )));

    unreachable();
}

Allocator default_allocator = (Allocator) {
    .procedure = &defaultAllocatorProcedure,
};

void contextCreate(void) {
    context = (Context) {
        .allocator = &default_allocator,
    };

    temporaryStorageCreate();
}

void contextRememberAllocators(void) {
    assert(MAX_REMEMBERED_LIST_COUNT != context.remembered_count);

    context.remembered_list[context.remembered_count] = context.allocator;
    context.remembered_count                          += 1;
}

void contextSetAllocators(Allocator *allocator) {
    contextRememberAllocators();

    context.allocator = allocator;
}

void contextRemindAllocators(void) {
    context.remembered_count                          -= 1;
    context.allocator                                 = context.remembered_list[context.remembered_count];
    context.remembered_list[context.remembered_count] = null;
}

void contextDestroy(void) {
    temporaryStorageDestroy();

    context = (Context) {
        0,
    };
}

// @NOTE
// This could actually be called at the very beginning and end of the `main' procedure,
// but this way you avoid accidentally shooting yourself in the foot, e.g. by calling the
// procedures of this framework before initialising it. This could actually be written
// somewhere in this file, but eh, no. We're using _clang_ and the so-called GNU ABI here,
// and so I feel somehow okay about using such compiler extensions to simplify how you
// interact with their `main' procedure.
//
// If this turns out to be annoying, it will be imminently and immediately removed.
//     ~ princessakokosowa, 24th of April 2023
void __attribute__ ((constructor)) preload(void) {
    contextCreate();
}

void __attribute__ ((destructor)) tidyUp(void) {
    contextDestroy();
}

void *_alloc(isize type_size_times_count) {
    void *maybe_ptr = context.allocator->procedure(ALLOCATOR_MODE_ALLOCATE, &(AllocatorDescription){
        .size_to_be_allocated_or_resized = type_size_times_count,
        .impl                            = context.allocator->impl,
    });

    assert(maybe_ptr != null);

    return maybe_ptr;
}

void *_resize(void *ptr, isize type_size_times_count) {
    assert(ptr != null);

    void *maybe_ptr = context.allocator->procedure(ALLOCATOR_MODE_RESIZE, &(AllocatorDescription){
        .ptr_to_be_resized_or_freed      = ptr,
        .size_to_be_allocated_or_resized = type_size_times_count,
        .impl                            = context.allocator->impl,
    });

    assert(maybe_ptr != null);

    return maybe_ptr;
}

void _free(void *ptr) {
    assert(ptr != null);

    void *result_but_ptr = context.allocator->procedure(ALLOCATOR_MODE_FREE, &(AllocatorDescription) {
        .ptr_to_be_resized_or_freed = ptr,
        .impl                       = context.allocator->impl,
    });

    assert(result_but_ptr != null);

    (void) result_but_ptr;
}

#endif // INCLUDE_CONTEXT_H
