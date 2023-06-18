#ifndef ALLOCATOR_H
#define ALLOCATOR_H

enum {
    ALLOCATOR_ALIGNMENT = 8,
};

core_function isize Allocator_align(isize size, isize alignment);
core_function isize Allocator_alignPtr(u8 *ptr, isize alignment);

typedef enum {
    ALLOCATOR_MODE_ALLOCATE      = 0,
    ALLOCATOR_MODE_RESIZE        = 1,
    ALLOCATOR_MODE_FREE          = 2,

    ALLOCATOR_MODE_STARTUP       = 4,
    ALLOCATOR_MODE_SHUTDOWN      = 5,

    ALLOCATOR_MODE_THREAD_START  = 6,
    ALLOCATOR_MODE_THREAD_STOP   = 7,

    ALLOCATOR_MODE_CREATE_HEAP   = 8,
    ALLOCATOR_MODE_DESTROY_HEAP  = 9,

    ALLOCATOR_MODE_IS_THIS_YOURS = 10,
    ALLOCATOR_MODE_CAPS          = 11,
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

    // A pointer to the implementation of an allocator.
    void *impl;
} AllocatorDescription;

typedef void *(AllocatorProcedure)(AllocatorMode mode, AllocatorDescription *description);

typedef struct {
    AllocatorProcedure *procedure;
    void               *impl;
} Allocator;

core_function void *Allocator_allocWithAllocator(isize type_size_times_count, Allocator *allocator);
core_function void *Allocator_resizeWithAllocator(void *ptr, isize type_size_times_count, Allocator *allocator);
core_function void Allocator_freeWithAllocator(void *ptr, Allocator *allocator);

#define allocWithAllocator  Allocator_allocWithAllocator
#define resizeWithAllocator Allocator_resizeWithAllocator
#define freeWithAllocator   Allocator_freeWithAllocator

#endif // ALLOCATOR_H
