#ifndef INCLUDE_ARRAY_H
#define INCLUDE_ARRAY_H

#include "foundation.h"
#include "allocator.h"

#define ARRAY_DEFAULT_CAPACITY 128

typedef struct {
    isize count;
    isize capacity;
} ArrayImpl;

#define Array_getImpl(array)     (cast(ArrayImpl*, (array)) - 1)

#define ArrayImpl_getArray(impl) (                 (impl)   + 1)

void *Array_maybeGrow(void *array, unsigned int size_of_backing_type, unsigned int values_count) {
    ArrayImpl *impl = null;
    if (array == null) {
        impl  = alloc(sizeof(ArrayImpl) + size_of_backing_type * ARRAY_DEFAULT_CAPACITY);
        *impl = (ArrayImpl) {
            .capacity = ARRAY_DEFAULT_CAPACITY,
        };
    } else {
        impl = Array_getImpl(array);

        bool should_resize = impl->count + values_count >= impl->capacity;
        if (should_resize == true) {
            isize new_capacity = impl->capacity * 3 / 8;
            impl = resize(impl, sizeof(ArrayImpl) + size_of_backing_type * new_capacity);
            if (impl == null) assert(true);

            *impl = (ArrayImpl) {
                .count    = impl->count,
                .capacity = new_capacity,
            };
        }
    }

    return ArrayImpl_getArray(impl);
}

#define Array_add(array, value)                                                             \
    (array)                                = Array_maybeGrow((array), sizeof(*(array)), 1); \
    (array)[Array_getImpl((array))->count] = (value);                                       \
    Array_getImpl((array))->count          += 1

#define Array_count(array)    Array_getImpl((array))->count

#define Array_capacity(array) Array_getImpl((array))->capacity

void Array_free(void *array) {
    if (array != null) free(Array_getImpl(array));
}

#endif // INCLUDE_ARRAY_H
