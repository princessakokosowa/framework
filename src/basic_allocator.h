#ifndef INCLUDE_ALLOCATOR_H
#define INCLUDE_ALLOCATOR_H

enum {
    ALLOCATOR_ALIGNMENT = 8,
};

core_function isize align(isize size, isize alignment) {
    isize mask = ~(alignment - 1);
    isize _    = size + (alignment - 1);

    return _ & mask;
}

/*
core_function
isize alignPtr(u8 *ptr, isize alignment) {
    isize mask   = alignment - 1;
    isize offset = cast(isize, ptr) & mask;
    isize _      = alignment - offset;

    return _ & mask;
}
*/

// @TODO
// Allocators, in addition, of course, to such basic modes, viz:
//     * ALLOCATOR_MODE_ALLOCATE
//     * ALLOCATOR_MODE_RESIZE
//     * ALLOCATOR_MODE_FREE
//
// There should be a bit more status here that, among other things, tells you whether the
// allocator is active, whether it has yet to be activated, etc. etc. and I can think of
// modes such as:
//    * ALLOCATOR_MODE_STARTUP
//    * ALLOCATOR_MODE_SHUTDOWN
//
//    * ALLOCATOR_MODE_THREAD_START
//    * ALLOCATOR_MODE_THREAD_STOP
//
//    * ALLOCATOR_MODE_CREATE_HEAP
//    * ALLOCATOR_MODE_DESTROY_HEAP
//
//    * ALLOCATOR_MODE_IS_THIS_YOURS
//    * ALLOCATOR_MODE_CAPS
//
// In addition to this, allocator capabilities should probably be added, but I do not
// even want to think at the moment about what they would be and GENERALLY how to define
// and stitch it all together.
//     ~ princessakokosowa, 10th of March 2023
typedef enum {
    ALLOCATOR_MODE_ALLOCATE = 0,
    ALLOCATOR_MODE_RESIZE   = 1,
    ALLOCATOR_MODE_FREE     = 2,
} AllocatorMode;

typedef struct {
    // In case of ALLOCATOR_MODE_ALLOCATE this is the new size that needs to be allocated.
    // In case of ALLOCATOR_MODE_RESIZE this is the new size that the buffer should have.
    // In case of ALLOCATOR_MODE_FREE this argument can be ignored.
    isize size_to_be_allocated_or_resized;

    // In case of ALLOCATOR_MODE_ALLOCATE this argument can be ignored.
    // In case of ALLOCATOR_MODE_RESIZE this is the current size of the buffer being reallocated. If the pointer is not null and the current_allocation_size is 0 then the allocator does not provide this information.
    // In case of ALLOCATOR_MODE_FREE this argument can be ignored.
    isize current_allocation_size;

    // In case of ALLOCATOR_MODE_ALLOCATE this argument can be ignored.
    // In case of ALLOCATOR_MODE_RESIZE this argument is the pointer to the buffer that must be reallocated.
    // In case of ALLOCATOR_MODE_FREE this argument is the pointer that needs to be freed.
    void *ptr_to_be_resized_or_freed;

    // @TODO
    // Write a description for that.
    void *impl;
} AllocatorDescription;

typedef void *(AllocatorProcedure)(AllocatorMode mode, AllocatorDescription *description);

typedef struct {
    AllocatorProcedure *procedure;
    void               *impl;
} Allocator;


// @TODO
// Consider moving this to context.h.
//     ~ princessakokosowa, 17 June 2023
core_function void *Context_alloc(isize type_size_times_count);
core_function void *Context_resize(void *ptr, isize type_size_times_count);
core_function void  Context_free(void *ptr);

core_function void *Context_allocUsingAllocator(isize type_size_times_count, Allocator *allocator);
core_function void *Context_resizeUsingAllocator(void *ptr, isize type_size_times_count, Allocator *allocator);
core_function void  Context_freeUsingAllocator(void *ptr, Allocator *allocator);

#define alloc                Context_alloc
#define resize               Context_resize
#define free                 Context_free

#define allocUsingAllocator  Context_allocUsingAllocator
#define resizeUsingAllocator Context_resizeUsingAllocator
#define freeUsingAllocator   Context_freeUsingAllocator

#endif // INCLUDE_ALLOCATOR_H