// Unlike many other _modules_ in this framework, arrays and hash maps will be done a
// little differently, hiding implementation details from the user in favor of greater
// convenience of use.
//
// While in the case of allocators, or wrappers for API functions (where we often use
// structs, which are so-called parameter bags (which we're trying to balance so that we
// don't end up like OpenGL, as one extreme, or the Vulkan API, as the other), we assume
// that arrays as the main workhorse of this framework must feel like they would be
// native to the C language. This is probably the closest to what we can actually have.
// I wonder what to do with static arrays and how to unify Array_count so that it handles
// both static and dynamic (resizable?) arrays.

#ifndef INCLUDE_ARRAY_H
#define INCLUDE_ARRAY_H

#include "foundation.h"
#include "allocator.h"

#define ARRAY_DEFAULT_CAPACITY 4

typedef struct {
    isize count;
    isize capacity;
} ArrayImpl;

#define Array_getImpl(array)     (cast(ArrayImpl*, (array)) - 1)
#define ArrayImpl_getArray(impl) (                 (impl)   + 1)

#define Array_count(array)                  \
    (                                       \
        ((array) != null)                   \
            ? Array_getImpl((array))->count \
            : 0                             \
    )

#define Array_capacity(array)                  \
    (                                          \
        ((array) != null)                      \
            ? Array_getImpl((array))->capacity \
            : 0                                \
    )

static inline void *Array_maybeGrow(void *array, isize size_of_backing_type, isize count_added, isize capacity_to_be_set) {
    if (Array_count(array) + count_added > capacity_to_be_set) capacity_to_be_set = Array_count(array) + count_added;
    if (Array_capacity(array) >= capacity_to_be_set)           return array;
    //                        ^^
    // That's fine, @TODO explain why.

    if      (2 * Array_capacity(array) > capacity_to_be_set) capacity_to_be_set = 2 * Array_capacity(array);
    else if (ARRAY_DEFAULT_CAPACITY > capacity_to_be_set)    capacity_to_be_set = ARRAY_DEFAULT_CAPACITY;

    ArrayImpl *impl = null;
    if (array == null) {
        impl           = alloc(sizeof(ArrayImpl) + size_of_backing_type * capacity_to_be_set);
        impl->count    = 0;
        impl->capacity = capacity_to_be_set;
    } else {
        impl           = resize(Array_getImpl(array), sizeof(ArrayImpl) + size_of_backing_type * capacity_to_be_set);
        impl->capacity = capacity_to_be_set;
    }

    return ArrayImpl_getArray(impl);
}

#define Array_addAt(array, value, index)                                              \
    assert((index) >= 0);                                                             \
    assert((index) <= Array_count((array)));                                          \
                                                                                      \
    (array)                       = Array_maybeGrow((array), sizeof(*(array)), 1, 0); \
    Array_getImpl((array))->count += 1;                                               \
                                                                                      \
    {                                                                                 \
        isize __index = Array_count((array));                                         \
        while (__index > (index)) {                                                   \
            (array)[__index] = (array)[__index - 1];                                  \
            __index          -= 1;                                                    \
        }                                                                             \
    }                                                                                 \
                                                                                      \
    (array)[(index)] = (value)

#define Array_add(array, value)                                                                \
    (array)                                = Array_maybeGrow((array), sizeof(*(array)), 1, 0); \
    (array)[Array_getImpl((array))->count] = (value);                                          \
    Array_getImpl((array))->count          += 1

#define Array_removeAt(array, index)                                        \
    Array_getImpl((array))->count -= 1;                                     \
    (array)[(index)]              = (array)[Array_getImpl((array))->count];

// @TODO
// Replace some of its parts with `Array_removeAt`.
#define Array_remove(array, value)                                                  \
    for (isize __index = 0; __index < Array_count((array)); __index += 1) {         \
        if ((value) == (array)[__index]) {                                          \
            Array_getImpl((array))->count -= 1;                                     \
            (array)[__index]              = (array)[Array_getImpl((array))->count]; \
        }                                                                           \
    }

#define Array_push(array, value) \
    Array_add((array), (value))

#define Array_pop(array)                       \
    (                                          \
        Array_getImpl((array))->count -= 1,    \
        (array)[Array_getImpl((array))->count] \
    )

static inline void Array_free(void *array) {
    if (array != null) free(Array_getImpl(array));
}

#endif // INCLUDE_ARRAY_H
