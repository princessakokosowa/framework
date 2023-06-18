#ifndef CONTEXT_H
#define CONTEXT_H

enum {
    MAX_REMEMBERED_LIST_COUNT = 128,
};

typedef struct {
    isize     remembered_count;
    Allocator *remembered_list[MAX_REMEMBERED_LIST_COUNT];

    Allocator *allocator;
} Context;

core_function void *Default_allocatorProcedure(AllocatorMode mode, AllocatorDescription *description);

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

#endif // CONTEXT_H
