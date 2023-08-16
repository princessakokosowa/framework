core_function void *Array_initialiseOrGrow(void *array, isize size_of_backing_type, isize count_to_be_added, isize capacity_to_be_set) {
    if (Array_count(array) + count_to_be_added > capacity_to_be_set) capacity_to_be_set = Array_count(array) + count_to_be_added;
    if (Array_capacity(array) >= capacity_to_be_set)                 return array;

    if      (Array_GrowthFactor * Array_capacity(array) > capacity_to_be_set) capacity_to_be_set = Array_GrowthFactor * Array_capacity(array);
    else if (Array_Capacity > capacity_to_be_set)                              capacity_to_be_set = Array_Capacity;

    // Cached, as `resize` may mov the contents somewhere else.
    isize count = Array_count(array);

    Allocator *allocator = Array_allocator(array);
    if (allocator == null) allocator = Context_getAllocator();

    ArrayPreamble *preamble = null;
    if (array == null) preamble = Allocator_alloc(sizeof(ArrayPreamble) + size_of_backing_type * capacity_to_be_set, allocator);
    else               preamble = Allocator_resize(Array_castToPreamble(array), sizeof(ArrayPreamble) + size_of_backing_type * capacity_to_be_set, allocator);

    *preamble = (ArrayPreamble) {
        .count     = count,
        .capacity  = capacity_to_be_set,
        .allocator = allocator,
    };

    return Preamble_castToArray(preamble);
}

core_function void *Array_justInitialiseButSetAllocator(void *array, Allocator *allocator) {
    assert(array == null);
    assert(allocator != null);

    isize size_of_backing_type = sizeof(isize);

    ArrayPreamble *preamble = Allocator_alloc(sizeof(ArrayPreamble) + size_of_backing_type * Array_Capacity, allocator);

    *preamble = (ArrayPreamble) {
        .capacity  = Array_Capacity,
        .allocator = allocator,
    };

    return Preamble_castToArray(preamble);
}

core_function void Array_release(void *array) {
    if (array != null) Allocator_release(Array_castToPreamble(array), Array_allocator(array));
}
