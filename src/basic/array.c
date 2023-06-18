core_function void *Array_grow(void *array, isize size_of_backing_type, isize count_to_be_added, isize capacity_to_be_set) {
    if (Array_count(array) + count_to_be_added > capacity_to_be_set) capacity_to_be_set = Array_count(array) + count_to_be_added;
    if (Array_capacity(array) >= capacity_to_be_set)                 return array;

    if      (ARRAY_DEFAULT_GROWTH_FACTOR * Array_capacity(array) > capacity_to_be_set) capacity_to_be_set = ARRAY_DEFAULT_GROWTH_FACTOR * Array_capacity(array);
    else if (ARRAY_DEFAULT_CAPACITY > capacity_to_be_set)                              capacity_to_be_set = ARRAY_DEFAULT_CAPACITY;

    // Cached, as `resize` may mov the contents somewhere else.
    isize count = Array_count(array);

    Allocator *allocator = Array_allocator(array);
    if (allocator == null) allocator = context.allocator;

    ArrayPreamble *preamble = null;
    if (array == null) preamble = allocWithAllocator(sizeof(ArrayPreamble) + size_of_backing_type * capacity_to_be_set, allocator);
    else               preamble = resizeWithAllocator(Array_castToPreamble(array), sizeof(ArrayPreamble) + size_of_backing_type * capacity_to_be_set, allocator);

    *preamble = (ArrayPreamble) {
        .count     = count,
        .capacity  = capacity_to_be_set,
        .allocator = allocator,
    };

    return Preamble_castToArray(preamble);
}

core_function void *Array_initialiseWithAllocators(void *array, Allocator *allocator) {
    ensure(array == null);
    ensure(allocator != null);

    isize size_of_backing_type = sizeof(isize);

    ArrayPreamble *preamble = allocWithAllocator(sizeof(ArrayPreamble) + size_of_backing_type * ARRAY_DEFAULT_CAPACITY, allocator);

    *preamble = (ArrayPreamble) {
        .capacity  = ARRAY_DEFAULT_CAPACITY,
        .allocator = allocator,
    };

    return Preamble_castToArray(preamble);
}

core_function void Array_free(void *array) {
    if (array != null) freeWithAllocator(Array_castToPreamble(array), Array_allocator(array));
}
