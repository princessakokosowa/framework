#ifndef BASIC_ALLOCATOR_H
#define BASIC_ALLOCATOR_H

////////////////////////////////
// Allocator constants

#if !defined(ALLOCATOR_COMMIT_GRANULARITY)
    #define ALLOCATOR_COMMIT_GRANULARITY kilobytes(4)
#endif

#if !defined(ALLOCATOR_DECOMMIT_THRESHOLD)
    #define ALLOCATOR_DECOMMIT_THRESHOLD megabytes(64)
#endif

////////////////////////////////
// Allocator types

enum {
    Allocator_Alignment = 8,
};

typedef enum {
    AllocatorMode_Allocate     = 0 ,
    AllocatorMode_Resize       = 1 ,
    AllocatorMode_Release      = 2 ,

    AllocatorMode_StartUp      = 4 ,
    AllocatorMode_ShutDown     = 5 ,

    AllocatorMode_ThreadStart  = 6 ,
    AllocatorMode_ThreadStop   = 7 ,

    AllocatorMode_AllocateHeap = 8 ,
    AllocatorMode_ReleaseHeap  = 9 ,

    AllocatorMode_IsThisYours  = 10,
    AllocatorMode_Capabilities = 11,
} AllocatorMode;

typedef struct {
    isize size_to_be_allocated_or_resized;
    isize current_allocation_size;
    void  *ptr_to_be_resized_or_freed;

    void *impl;
} AllocatorDescription;

////////////////////////////////
// Allocator procedures

typedef void *(AllocatorProcedure)(AllocatorMode mode, AllocatorDescription *description);

typedef struct {
    AllocatorProcedure *procedure;
    void               *impl;
} Allocator;

core_function void Allocator_allocHeap(Allocator *allocator);
core_function void Allocator_releaseHeap(Allocator *allocator);

core_function void *Allocator_alloc(isize type_size_times_count, Allocator *allocator);
core_function void *Allocator_resize(void *ptr, isize type_size_times_count, Allocator *allocator);
core_function void Allocator_release(void *ptr, Allocator *allocator);

#undef alloc
#undef resize
#undef release

////////////////////////////////
// Pool constants

enum {
    Pool_DEFAULT_ALIGNMENT          = 8,
    Pool_DEFAULT_SIZE               = megabytes(256),
    Pool_DEFAULT_COMMIT_GRANULARITY = kilobytes(4),
    Pool_DEFAULT_DECOMMIT_THRESHOLD = megabytes(64),
};

////////////////////////////////
// Pool types

typedef struct {
    isize alignment;
    isize size;
    isize commit_granularity;
    isize decommit_threshold;
} PoolDescription;

typedef struct {
    isize alignment;
    isize size;
    isize commited;
    isize occupied;
    isize commit_granularity;
    isize decommit_threshold;

    u8 *ptr;
} Pool;

////////////////////////////////
// Pool procedures

core_function void      *Pool_procedure(AllocatorMode mode, AllocatorDescription *description);
core_function Pool      Pool_init(PoolDescription *description);
core_function void      Pool_release(Pool *pool);
core_function void      *Pool_alloc(Pool *pool, isize type_size_times_count);
core_function Allocator Pool_getAllocator(Pool *pool);

////////////////////////////////
// Context constants

enum {
    MAX_REMEMBERED_LIST_COUNT = 128,
};

////////////////////////////////
// Context types

typedef struct {
    isize     remembered_count;
    Allocator *remembered_list[MAX_REMEMBERED_LIST_COUNT];

    Pool      pool;
    Allocator _pool_allocator;

    Allocator *allocator;
} Context;

////////////////////////////////
// Context globals

core_global Context context;

////////////////////////////////
// Context procedures

core_function void      Context_init(void);
core_function void      Context_release(void);
core_function void      Context_rememberAllocators(void);
core_function void      Context_pushAllocators(Allocator *allocator);
core_function void      Context_popAllocators(void);
core_function Allocator *Context_getAllocator(void);
core_function Allocator *Context_getDefaultAllocator(void);

#define alloc(      type_size_times_count) Allocator_alloc(         (type_size_times_count), (context.allocator))
#define resize(ptr, type_size_times_count) Allocator_resize( (ptr), (type_size_times_count), (context.allocator))
#define release(  ptr                    ) Allocator_release((ptr),                          (context.allocator));

#endif // BASIC_ALLOCATOR_H
