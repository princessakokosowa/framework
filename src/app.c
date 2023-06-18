#include "allocator_tests.h"
#include "array_tests.h"
#include "haversine_tests.h"

core_function void entryPoint(void) {
    AllocatorTests_test();
    ArrayTests_test();
    HaversineTests_test();
}
