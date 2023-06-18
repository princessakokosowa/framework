#ifndef INCLUDE_BASIC_H
#define INCLUDE_BASIC_H

#include <stdio.h>
#include <stdarg.h>

////////////////////////////////
// Basic headers

#include "build.h"
#include "types.h"

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>

////////////////////////////////
// OS-specific headers; @NOTE This probably shouldn not be here

#if OS_WINDOWS
    #include "os_win.h"
#else
    #error   OS layer not implemented.
#endif

////////////////////////////////
// Glue

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

#include "allocator.h"
#include "temporary_storage.h"
#include "context.h"
#include "memory.h"
#include "array.h"
#include "math.h" // Windows-provided, @TODO make self.

#endif // INCLUDE_BASIC_H
