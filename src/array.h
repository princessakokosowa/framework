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
#include "memory.h"

#define ARRAY_DEFAULT_CAPACITY 4

typedef struct {
    isize count;
    isize capacity;
} ArrayImpl;

#define forEach(array)                                                                                                 \
    for (isize i = 0; i < Array_count((array)); i += 1) \

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

#define Array_reserve(array, capacity_to_be_set)                                   \
    (array) = Array_maybeGrow((array), sizeof(*(array)), 0 , (capacity_to_be_set))

#define Array_resize(array, count_to_be_set)                                                                                     \
    do {                                                                                                                         \
        (array) = Array_maybeGrow((array), sizeof(*(array)), (count_to_be_set) - Array_count((array)), 0);                       \
        if (Array_count((array)) < (count_to_be_set)) Array_getImpl((array))->count += (count_to_be_set) - Array_count((array)); \
    } while (false)

#define Array_addAt(array, value, index)                                                  \
    do {                                                                                  \
        assert((index) >= 0);                                                             \
        assert((index) <= Array_count((array)));                                          \
                                                                                          \
        (array)                       = Array_maybeGrow((array), sizeof(*(array)), 1, 0); \
        Array_getImpl((array))->count += 1;                                               \
                                                                                          \
        Memory_move(                                                                      \
            (array) + (index) + 1,                                                        \
            (array) + (index),                                                            \
            (Array_count((array)) - (index)) * sizeof(*(array))                           \
        );                                                                                \
                                                                                          \
        (array)[(index)] = (value);                                                       \
    } while (false)

#define Array_add(array, value)                                                                    \
    do {                                                                                           \
        (array)                                = Array_maybeGrow((array), sizeof(*(array)), 1, 0); \
        (array)[Array_getImpl((array))->count] = (value);                                          \
        Array_getImpl((array))->count          += 1;                                               \
    } while (false)

#define Array_removeAtIndex(array, index)                                       \
    do {                                                                        \
        assert((index) >= 0);                                                   \
        assert((index) < Array_count((array)));                                 \
                                                                                \
        Array_getImpl((array))->count -= 1;                                     \
        (array)[(index)]              = (array)[Array_getImpl((array))->count]; \
    } while (false)

#define Array_removeAtIndexOrdered(array, index)                \
    do {                                                        \
        assert((index) >= 0);                                   \
        assert((index) < Array_count((array)));                 \
                                                                \
        Memory_move(                                            \
            (array) + (index),                                  \
            (array) + (index) + 1,                              \
            (Array_count((array)) - (index)) * sizeof(*(array)) \
        );                                                      \
                                                                \
        Array_getImpl((array))->count -= 1;                     \
    } while (false)

#define Array_removeByValue(array, value)                                                      \
    do {                                                                                       \
        for (isize _Array_index = 0; _Array_index < Array_count((array)); _Array_index += 1) { \
            if ((value) == (array)[_Array_index]) {                                            \
                Array_removeAtIndex((array), _Array_index);                                    \
                break;                                                                         \
            }                                                                                  \
        }                                                                                      \
    } while (false)

#define Array_removeByValueOrdered(array, value)                                               \
    do {                                                                                       \
        for (isize _Array_index = 0; _Array_index < Array_count((array)); _Array_index += 1) { \
            if ((value) == (array)[_Array_index]) {                                            \
                Array_removeAtIndexOrdered((array), _Array_index);                             \
                break;                                                                         \
            }                                                                                  \
        }                                                                                      \
    } while (false)

#define Array_removeAllByValue(array, value)                                                  \
    do {                                                                                      \
        isize _Array_index = Array_count((array)) - 1;                                        \
        while (_Array_index >= 0) {                                                           \
            if ((value) == (array)[_Array_index]) Array_removeAtIndex((array), _Array_index); \
            _Array_index -= 1;                                                                \
        }                                                                                     \
    } while (false)

#define Array_removeAllByValueOrdered(array, value)                                                  \
    do {                                                                                             \
        isize _Array_index = Array_count((array)) - 1;                                               \
        while (_Array_index >= 0) {                                                                  \
            if ((value) == (array)[_Array_index]) Array_removeAtIndexOrdered((array), _Array_index); \
            _Array_index -= 1;                                                                       \
        }                                                                                            \
    } while (false)

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
