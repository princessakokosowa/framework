#ifndef INCLUDE_DYNAMIC_ARRAY_H
#define INCLUDE_DYNAMIC_ARRAY_H

#include "foundation.h"
#include "allocator.h"

typedef struct {
    isize count;
    isize capacity;
} DynamicArray;

#define header(t)       ((DynamicArray*)(t) - 1)
#define darrCount(a)    ((a) ? header(a)->count : 0 )
#define darrCapacity(a) ((a) ? header(a)->capacity : 0)

void* darrGrowProcedure(void *maybe_ptr, isize type_size, isize count, isize minimal_capacity) {
    // Force debugging, a nice trick from stb_ds.h.
    {
        DynamicArray dynamic_array_internals = (DynamicArray){
            0,
        };

        cast(void, sizeof(dynamic_array_internals));
    }

    isize minimal_count = darrCount(maybe_ptr) + count;

    // Either adjust the capacity of the array or leave if there is nothing to adjust.
    // An early exit, usually when the array is already created.
    {
        bool const should_adjust_capacity = minimal_count > minimal_capacity;
        if (should_adjust_capacity == true) minimal_capacity = minimal_count;

        bool const should_simply_leave_as_there_is_nothing_to_adjust = darrCapacity(maybe_ptr) >= minimal_capacity;
        if (should_simply_leave_as_there_is_nothing_to_adjust == true) return maybe_ptr;
    }

    // Kindly borrowed from stb_ds.h, it is for increasing the needed capacity to
    // guarantee O(1) being amortized.
    {
        bool const should_increase_capacity_to_amortise_for_performance = minimal_capacity < 2 * darrCapacity(maybe_ptr);
        bool const make_it_hold_at_least_four_elements                  = minimal_capacity < 4;
        if      (should_increase_capacity_to_amortise_for_performance) minimal_capacity = 2 * darrCapacity(maybe_ptr);
        else if (make_it_hold_at_least_four_elements)                  minimal_capacity = 4;
    }

    isize const size  = type_size * minimal_capacity;
    isize const extra = sizeof(DynamicArray);
    u8          *ptr  = null;
    if (maybe_ptr == null) ptr = cast(u8*, alloc(                    size + extra));
    else                   ptr = cast(u8*, resize(header(maybe_ptr), size + extra));

    // This is ALWAYS OK!
    ptr += extra;

    if (maybe_ptr == null) header(ptr)->count = 0;

    header(ptr)->capacity = minimal_capacity;

    return ptr;
}

#define darrGrow(a, b, c)   ((a) = darrGrowProcedure((a), sizeof *(a), (b), (c)))
#define darrMaybeGrow(a, n) ((!(a) || header(a)->count + (n) > header(a)->capacity) ? (darrGrow(a, n, 0), 0) : 0)
#define darrAppend(a, v)    (darrMaybeGrow(a, 1), (a)[header(a)->count++] = (v))
#define darrFree(a)         ((void) ((a) ? free(header(a)) : (void)0), (a) = null)

#endif // INCLUDE_DYNAMIC_ARRAY_H
