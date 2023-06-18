////////////////////////////////
// Includes, unity build

// headers
#include "basic/basic.h"
#include "tests/tests.h"

// sources
#include "basic/basic.c"
#include "tests/tests.c"

core_function void entryPoint(void) {
    AllocatorTests_test();
    ArrayTests_test();
    HaversineTests_test();
}
