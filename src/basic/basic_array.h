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

#ifndef BASIC_ARRAY_H
#define BASIC_ARRAY_H

enum {
    Array_GrowthFactor = 2,
    Array_Capacity     = 8,
};

typedef struct {
    isize count;
    isize capacity;

    Allocator *allocator;
} ArrayPreamble;

#define Array(Type) Type *

#define Array_castToPreamble(array)                  (cast(ArrayPreamble *, (array   )) - 1)
#define Preamble_castToArray(preamble) (cast(void *, (                       preamble   + 1)))

#define Array_count(array)                         \
    (                                              \
        ((array) != null)                          \
            ? Array_castToPreamble((array))->count \
            : 0                                    \
    )

#define Array_capacity(array)                         \
    (                                                 \
        ((array) != null)                             \
            ? Array_castToPreamble((array))->capacity \
            : 0                                       \
    )

#define Array_allocator(array)                         \
    (                                                  \
        ((array) != null)                              \
            ? Array_castToPreamble((array))->allocator \
            : null                                     \
    )

#define Array_first(array) (array + 0)
#define Array_last(array)  (array + Array_count(array) - 1)

#define forEachByIndex(array) for (isize index = 0; index < Array_count((array)); index += 1)

#define forEachByValue(array) for (__typeof((array)) value = (array); value != (array) + Array_count((array)); value += 1)

// @NOTE
// Alright, so this works as follows:
//     * Create a standard for loop that iterates through all elements of the array using
//       an index-based approach
//     * Introduce a variable called `flip` to control the termination of the subsequent
//       for loop after a single iteration; `flip == 0`
//     * Inside the second for loop, initialize a variable `value` to hold the value
//       corresponding to the current index; `continue` contition is `flip != 1`
//     * As soon as the second for loop starts its seconds iteraton due to the fact that
//       `flip` is flipped so that `flip == 1`, it breaks and the first for loop bumps
//       `index` and flips `flip` back to 0
//     * Consequently, within that _double_ for loop body, both the index and the
//       corresponding value are available for each iteration
//
// I have no clue how much such thing may hurt performance in practice, we will see.
//     ~ princessakokosowa, 18 June 2023
#define forEach(array)                                                                      \
    for (isize flip = 0, index = 0; index < Array_count((array)); flip = !flip, index += 1) \
        for (__typeof((array)) value = (array) + index; flip != 1; flip = !flip)

core_function void *Array_initialiseOrGrow(void *array, isize size_of_backing_type, isize count_to_be_added, isize capacity_to_be_set);
core_function void *Array_justInitialiseButSetAllocator(void *array, Allocator *allocator);

#define Array_initSetAllocators(array, allocator) (array) = Array_justInitialiseButSetAllocator((array), (allocator))

#define Array_reserve(array, capacity_to_be_set)  (array) = Array_initialiseOrGrow((array), sizeof(*(array)), 0, (capacity_to_be_set))

#define Array_resize(array, count_to_be_set)                                                                                            \
    multilineMacroBegin                                                                                                                 \
        (array) = Array_initialiseOrGrow((array), sizeof(*(array)), (count_to_be_set) - Array_count((array)), 0);                                   \
        if (Array_count((array)) < (count_to_be_set)) Array_castToPreamble((array))->count += (count_to_be_set) - Array_count((array)); \
    multilineMacroEnd

#define Array_addAt(array, value, index)                                                                \
    multilineMacroBegin                                                                                 \
        assert((index) >= 0);                                                                           \
        assert((index) <= Array_count((array)));                                                        \
                                                                                                        \
        (array)                              = Array_initialiseOrGrow((array), sizeof(*(array)), 1, 0); \
        Array_castToPreamble((array))->count += 1;                                                      \
                                                                                                        \
        move(                                                                                           \
            (array) + (index) + 1,                                                                      \
            (array) + (index),                                                                          \
            (Array_count((array)) - (index)) * sizeof(*(array))                                         \
        );                                                                                              \
                                                                                                        \
        (array)[(index)] = (value);                                                                     \
    multilineMacroEnd

#define Array_add(array, value)                                                                         \
    multilineMacroBegin                                                                                 \
        (array)                              = Array_initialiseOrGrow((array), sizeof(*(array)), 1, 0); \
        (array)[Array_count((array))]        = (value);                                                 \
        Array_castToPreamble((array))->count += 1;                                                      \
    multilineMacroEnd

#define Array_removeAtIndex(array, index)                                     \
    multilineMacroBegin                                                       \
        assert((index) >= 0);                                                 \
        assert((index) < Array_count((array)));                               \
                                                                              \
        Array_castToPreamble((array))->count -= 1;                            \
        (array)[(index)]                     = (array)[Array_count((array))]; \
    multilineMacroEnd

#define Array_removeAtIndexOrdered(array, index)                \
    multilineMacroBegin                                         \
        assert((index) >= 0);                                   \
        assert((index) < Array_count((array)));                 \
                                                                \
        move(                                                   \
            (array) + (index),                                  \
            (array) + (index) + 1,                              \
            (Array_count((array)) - (index)) * sizeof(*(array)) \
        );                                                      \
                                                                \
        Array_castToPreamble((array))->count -= 1;              \
    multilineMacroEnd

#define Array_removeByValue(array, value)                                                      \
    multilineMacroBegin                                                                        \
        for (isize _Array_index = 0; _Array_index < Array_count((array)); _Array_index += 1) { \
            if ((value) == (array)[_Array_index]) {                                            \
                Array_removeAtIndex((array), _Array_index);                                    \
                break;                                                                         \
            }                                                                                  \
        }                                                                                      \
    multilineMacroEnd

#define Array_removeByValueOrdered(array, value)                                               \
    multilineMacroBegin                                                                        \
        for (isize _Array_index = 0; _Array_index < Array_count((array)); _Array_index += 1) { \
            if ((value) == (array)[_Array_index]) {                                            \
                Array_removeAtIndexOrdered((array), _Array_index);                             \
                break;                                                                         \
            }                                                                                  \
        }                                                                                      \
    multilineMacroEnd

#define Array_removeAllByValue(array, value)                                                  \
    multilineMacroBegin                                                                       \
        isize _Array_index = Array_count((array)) - 1;                                        \
        while (_Array_index >= 0) {                                                           \
            if ((value) == (array)[_Array_index]) Array_removeAtIndex((array), _Array_index); \
            _Array_index -= 1;                                                                \
        }                                                                                     \
    multilineMacroEnd

#define Array_removeAllByValueOrdered(array, value)                                                  \
    multilineMacroBegin                                                                              \
        isize _Array_index = Array_count((array)) - 1;                                               \
        while (_Array_index >= 0) {                                                                  \
            if ((value) == (array)[_Array_index]) Array_removeAtIndexOrdered((array), _Array_index); \
            _Array_index -= 1;                                                                       \
        }                                                                                            \
    multilineMacroEnd

#define Array_push(array, value) Array_add((array), (value))

#define Array_pop(array)                              \
    (                                                 \
        Array_castToPreamble((array))->count -= 1,    \
        (array)[Array_castToPreamble((array))->count] \
    )

core_function void Array_release(void *array);

#endif // BASIC_ARRAY_H
