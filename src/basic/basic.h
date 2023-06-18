#ifndef INCLUDE_BASIC_H
#define INCLUDE_BASIC_H

#include "stdio.h"
#include "stdarg.h"
#include "stdlib.h"

// Move that to the build script.
#define BUILD_CORE_LINK_MODE BUILD_LINK_MODE_SOURCE

////////////////////////////////
// Basic headers

#include "build.h"
#include "types.h"

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
attribute_cold
attribute_noreturn
attribute_printf(1, 2)
void prv_panic(char const *format, ...) {
    va_list ap;
    va_start(ap, format);

    vfprintf(stderr, format, ap);
    fflush(stderr);

    va_end(ap);
    abort();
}

void prv_assert(bool condition, const char *file, int line, const char *func) {
    if (!condition) prv_panic("Assertion failed at %s:%d in %s.\n", file, line, func);
}

#undef ensure
#undef staticAssert

#define ensure(condition)       prv_assert(    condition, __FILE__, __LINE__, __func__)
#define staticAssert(condition) _Static_assert(condition, "Static assertion failed at %s:%d in %s.\n", __FILE__, __LINE__, __func__)

#define unreachable() prv_panic("Unreachable at %s:%d in %s.\n", __FILE__, __LINE__, __func__)
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#include "os/os.h"

#include "allocator.h"
#include "context.h"
#include "temporary_storage.h"
#include "memory.h"
#include "array.h"
#include "math.h" // Windows-provided, @TODO make self.
#include "thread.h"

#include "os/os_entry_point.h"

#endif // INCLUDE_BASIC_H
