#include "build.c"
#include "types.c"

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

#include "memory.c"
#include "allocator.c"
#include "context.c"
#include "arena.c"
#include "pool.c"
#include "temporary_storage.c"
#include "array.c"
#include "thread.c"
