#ifndef BASIC_CONTEXT_H
#define BASIC_CONTEXT_H

enum {
    HEAP_DEFAULT_ALIGNMENT          = 8,
    HEAP_DEFAULT_SIZE               = megabytes(256),
    HEAP_DEFAULT_COMMIT_GRANULARITY = kilobytes(4),
    HEAP_DEFAULT_DECOMMIT_THRESHOLD = megabytes(64),
};

typedef struct {
    isize alignment;
    isize size;
    isize commit_granularity;
    isize decommit_threshold;
} HeapDescription;

typedef struct {
    isize alignment;
    isize size;
    isize commited;
    isize occupied;
    isize commit_granularity;
    isize decommit_threshold;

    u8 *ptr;
} Heap;

core_function void      *Heap_allocatorProcedure(AllocatorMode mode, AllocatorDescription *description);
core_function Heap      Heap_create(HeapDescription *description);
core_function void      Heap_destroy(Heap *heap);
core_function void      *Heap_get(Heap *heap, isize type_size_times_count);
core_function Allocator Heap_getAllocator(Heap *heap);

enum {
    MAX_REMEMBERED_LIST_COUNT = 128,
};

typedef struct {
    isize     remembered_count;
    Allocator *remembered_list[MAX_REMEMBERED_LIST_COUNT];

    Heap      heap;
    Allocator heap_allocator;

    Allocator *allocator;
} Context;

core_global Context context;

core_function void Context_create(void);
core_function void Context_destroy(void);
core_function void Context_rememberAllocators(void);
core_function void Context_setAllocators(Allocator *allocator);
core_function void Context_remindAllocators(void);

core_function void *Context_alloc(isize type_size_times_count);
core_function void *Context_resize(void *ptr, isize type_size_times_count);
core_function void Context_free(void *ptr);

#define alloc  Context_alloc
#define resize Context_resize
#define free   Context_free

/*
#if !defined(alloc)
    #define alloc Context_alloc
#endif

#if !defined(resize)
    #define resize Context_resize
#endif

#if !defined(free)
    #define free Context_free
#endif
*/

#endif // BASIC_CONTEXT_H
