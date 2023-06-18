#include "allocator_tests.h"
#include "array_tests.h"
#include "haversine_tests.h"

int main(void) {
    AllocatorTests_test();
    ArrayTests_test();
    HaversineTests_test();

    return 0;
}
