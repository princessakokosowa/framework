#ifndef INCLUDE_FOUNDATION_H
#define INCLUDE_FOUNDATION_H

/////////////////////////////////////////////////////////////////////////////////////////
// The system dependencies.
/////////////////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32

    #include "win32.h"

#endif // _WIN32

/////////////////////////////////////////////////////////////////////////////////////////
// Typedefs for a boolean type and its values.
/////////////////////////////////////////////////////////////////////////////////////////
typedef _Bool              bool;

#define true               1
#define false              0

/////////////////////////////////////////////////////////////////////////////////////////
// Typedefs for a integer, both signed and unsigned, type and its values.
/////////////////////////////////////////////////////////////////////////////////////////
typedef signed char        i8;
typedef short              i16;
typedef int                i32;
typedef long long          i64;

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

typedef u16                f16;
typedef float              f32;
typedef double             f64;

typedef u64                usize;
typedef i64                isize;

// The convention here is that we ALWAYS use either `isize` or `usize` when we want to
// indicate that we DO NOT care about the size of our variables at the moment we actually
// write the code, and that it is still a subject for optimisation, e.g. once we know how
// big our numbers are.
//     ~ princessakokosowa, 7th of March 2023

/////////////////////////////////////////////////////////////////////////////////////////
// Various macros to help you express your intentions regarding various operations on
// data.
/////////////////////////////////////////////////////////////////////////////////////////
#define cast(Type, value) ((Type)(value))

#define arrayCount(value) ((isize)((sizeof((value))) / (sizeof((value[0])))))

#define null 0

#ifdef _MSC_VER

    #define ATTRIBUTE_COLD            __declspec(noinline)
    #define ATTRIBUTE_PRINTF(a, b)
    #define ATTRIBUTE_RETURNS_NOALIAS __declspec(restrict)
    #define ATTRIBUTE_NORETURN        __declspec(noreturn)
    #define ATTRIBUTE_MUST_USE

    #define BREAKPOINT __debugbreak()

#else

    #define ATTRIBUTE_COLD            __attribute__((cold))
    #define ATTRIBUTE_PRINTF(a, b)    __attribute__((format(printf, a, b)))
    #define ATTRIBUTE_RETURNS_NOALIAS __attribute__((__malloc__))
    #define ATTRIBUTE_NORETURN        __attribute__((noreturn))
    #define ATTRIBUTE_MUST_USE        __attribute__((warn_unused_result))

    #if defined(__MINGW32__) || defined(__MINGW64__)

        #define BREAKPOINT __debugbreak()

    #elif defined(__i386__) || defined(__x86_64__)

        #define BREAKPOINT __asm__ volatile("int $0x03");

    #elif defined(__clang__)

        #define BREAKPOINT __builtin_debugtrap()

    #elif defined(__GNUC__)

        #define BREAKPOINT __builtin_trap()

    #else

        #include <signal.h>
        #define BREAKPOINT raise(SIGTRAP)

    #endif // defined(__MINGW32__) || defined(__MINGW64__) || defined(__i386__) || defined(__x86_64__) || defined(__clang__) || defined(__GNUC__)

#endif // _MSC_VER

// @TODO
// Make up for these includes.
#include <stdio.h>
#include <stdarg.h>

ATTRIBUTE_COLD
ATTRIBUTE_NORETURN
ATTRIBUTE_PRINTF(1, 2)
void _panic(const char *format, ...) {
    va_list ap;
    va_start(ap, format);

    vfprintf(stderr, format, ap);
    fflush(stderr);

    va_end(ap);
    abort();
}

static inline void _assert(bool ok, const char *file, int line, const char *func) {
    if (!ok) {
        _panic("Assertion failed at %s:%d in %s.", file, line, func);
    }
}

#ifdef _WIN32

    #define __func__ __FUNCTION__

#endif // _WIN32

#define _unreachable() _panic("Unreachable at %s:%d in %s. This is a bug in the Zig compiler.", __FILE__, __LINE__, __func__)

#undef assert
#define assert(ok) _assert(ok, __FILE__, __LINE__, __func__)

#define staticAssert(ok) _Static_assert(ok, "Static assertion failed at %s:%d in %s.", __FILE__, __LINE__, __func__)

#endif // INCLUDE_FOUNDATION_H
