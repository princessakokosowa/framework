#include "allocator_tests.h"
#include "array_tests.h"
#include "gpu_tests.h"

int main(void) {
    AllocatorTests_test();
    ArrayTests_test();
    GpuTests_test();

    return 0;
}
