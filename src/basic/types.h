#ifndef BASIC_TYPES_H
#define BASIC_TYPES_H

////////////////////////////////
// Macros

// linking keywords
#if LANG_CPP
    #define no_name_mangle extern "C"
#else
    #define no_name_mangle
#endif

#if BUILD_CORE_LINK_MODE == BUILD_LINK_MODE_SOURCE
    #define core_global       no_name_mangle
    #define core_function     function
#elif BUILD_CORE_LINK_MODE == BUILD_LINK_MODE_STATIC
    #define core_global       extern
    #define core_function     extern no_name_mangle
#elif BUILD_CORE_LINK_MODE == BUILD_LINK_MODE_DYNAMIC
    #if BUILD_ROOT
        #define core_global   exported extern
        #define core_function exported
    #else
        #define core_global   imported
        #define core_function imported
    #endif
#endif

#if OS_WINDOWS
    #define exported no_name_mangle __declspec(dllexport)
#else
    #define exported no_name_mangle
#endif

#if OS_WINDOWS
    #define imported no_name_mangle __declspec(dllimport)
#else
    #define imported no_name_mangle
#endif

// keywords

#define global         static
#define function       static
#define local_persist  static

#define main_thread_only
#define through

#if COMPILER_MSVC
    #define per_thread __declspec(thread)
#elif COMPILER_CLANG
    #define per_thread __thread
#elif COMPILER_GCC
    #define per_thread __thread
#endif

#if COMPILER_MSVC && COMPILER_MSVC_YEAR < 2015
    #define inline_function static
#else
    #define inline_function inline static
#endif

#if COMPILER_MSVC && COMPILER_MSVC_YEAR < 2015
    #define this_function_name "unknown"
#else
    #define this_function_name __func__
#endif

#if COMPILER_MSVC && COMPILER_MSVC_YEAR < 2015
    #define snprintf _snprintf
#endif

#if COMPILER_MSVC
    #pragma section(".roglob", read)
    #define read_only __declspec(allocate(".roglob"))
#elif COMPILER_CLANG
    #define read_only // __attribute__((section(".roglob"), read))
#elif COMPILER_GCC
    #define read_only // __attribute__((section(".roglob"), read))
#endif

#ifdef COMPILER_MSVC
    #define attribute_cold            __declspec(noinline)
    #define attribute_printf(a, b)
    #define attribute_returns_noalias __declspec(restrict)
    #define attribute_noreturn        __declspec(noreturn)
    #define attribute_must_use
#elif COMPILER_CLANG
    #define attribute_cold            __attribute__((cold))
    #define attribute_printf(a, b)    __attribute__((format(printf, a, b)))
    #define attribute_returns_noalias __attribute__((__malloc__))
    #define attribute_noreturn        __attribute__((noreturn))
    #define attribute_must_use        __attribute__((warn_unused_result))
#elif COMPILER_GCC
    #define attribute_cold            __attribute__((cold))
    #define attribute_printf(a, b)    __attribute__((format(printf, a, b)))
    #define attribute_returns_noalias __attribute__((__malloc__))
    #define attribute_noreturn        __attribute__((noreturn))
    #define attribute_must_use        __attribute__((warn_unused_result))
#endif

// atomics

#if OS_WINDOWS
    #define atomicIncrementEval64(ptr) InterlockedIncrement64((ptr))
    #define atomicDecrementEval64(ptr) InterlockedDecrement64((ptr))
    #define atomicSetEval64(ptr, val)  InterlockedExchange64((ptr), (val))
#else
    #error Atomics not implemented.
#endif

// compiler helpers

#define unusedVariable(variable) cast(void, (variable))

// integer/pointer/array/type manipulations

#define cast(Type, value) ((Type)(value))

#define arrayCount(array) (sizeof(array) / sizeof((array)[0]))

#define intFromPtr(ptr)     cast(usize , cast(u8 *, ptr) - 0)
#define ptrFromInt(integer) cast(void *, cast(u8 *, 0  ) + i)

#define bytes(value)      (           (value) /* <<  0 */)
#define kilobytes(value)  (           (value)    << 10   )
#define megabytes(value)  (           (value)    << 20   )
#define gigabytes(value)  (cast(usize, value)    << 30   )
#define terabytes(value)  (cast(usize, value)    << 40   )

#define thousand(value) ((value)) * 1000        )
#define million(value)  ((value)) * 1000000     )
#define billion(value)  ((value)) * 1000000000ll)

// clamps, mins, maxes

#define min(a, b)   \
    (               \
        ((a) < (b)) \
            ? (a)   \
            : (b)   \
    )

#define max(a, b)   \
    (               \
        ((a) > (b)) \
            ? (a)   \
            : (b)   \
    )

#define clampCeil(x, a) min(x, a)
#define clampFloor(a, x)   max(a, x)

#define clamp(a, x, b)    \
    (                     \
        ((a) > (x))       \
            ? (a)         \
            : ((b) < (x)) \
                ? (b)     \
                : (x)     \
    )

// loop helpers

// @TODO
// Add types to these macros, maybe?
//     ~ princessakokosowa, 18 June 2023
#define forInRange(from, to)                             \
    for (isize index = (from); index < (to); index += 1)

#define forInRangeStep(from, to, step)                        \
    for (isize index = (from); index < (to); index += (step))

// utility

#define multilineMacroBegin do {
#define multilineMacroEnd   } while (false)

////////////////////////////////
// Base types

typedef _Bool              bool;

typedef signed char        i8;
typedef short              i16;
typedef int                i32;
typedef long long          i64;

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

typedef i16                f16;
typedef float              f32;
typedef double             f64;

////////////////////////////////
// @NOTE
// The convention here is that we ALWAYS use either `isize` or `usize` when we want to
// indicate that we DO NOT care about the size of our variables at the moment we actually
// write the code, and that it is still a subject for optimisation, e.g. once we know how
// big our numbers are.
//     ~ princessakokosowa, 7 March 2023
typedef u64                usize;
typedef i64                isize;

#define true               1
#define false              0
#define null               0

////////////////////////////////
// Limits

read_only global i8 i8_MAX = 0xFF;
read_only global i8 i8_MIN = 0;

read_only global i16 i16_MAX = 0xFFFF;
read_only global i16 i16_MIN = 0;

read_only global u32 U32_MAX = 0xFFFFFFFF;
read_only global u32 U32_MIN = 0;

read_only global u64 U64_MAX = 0xFFFFFFFFFFFFFFFF;
read_only global u64 U64_MIN = 0;

read_only global i8 I8_MAX = 0x7F;
read_only global i8 I8_MIN = -1 - 0x7F;

read_only global i16 I16_MAX = 0x7FFF;
read_only global i16 I16_MIN = -1 - 0x7FFF;

read_only global i32 I32_MAX = 0x7FFFFFFF;
read_only global i32 I32_MIN = -1 - 0x7FFFFFFF;

read_only global i64 I64_MAX = 0x7FFFFFFFFFFFFFFF;
read_only global i64 I64_MIN = -1 - 0x7FFFFFFFFFFFFFFF;

read_only global f32 F32_MAX = 3.4028234664e+38;
read_only global f32 F32_MIN = -3.4028234664e+38;

// @TODO
//     ~ princessakokosowa, 18 June 2023
// read_only global f64 F64_MAX = 0.0;
// read_only global f64 F64_MIN = 0.0;

////////////////////////////////
// Constants

read_only global u32 F32_SIGN     = 0x80000000;
read_only global u32 F32_EXPONENT = 0x7F800000;
read_only global u32 F32_MANTISSA = 0x7FFFFF;

read_only global f32 F32_SMALLEST_POSITIVE = 1.1754943508e-38;
read_only global f32 F32_EPSILON           = 5.96046448e-8;

read_only global u64 F64_SIGN     = 0x8000000000000000ull;
read_only global u64 F64_EXPONENT = 0x7FF0000000000000ull;
read_only global u64 F64_MANTISSA = 0xFFFFFFFFFFFFFull;

// @TODO
//     ~ princessakokosowa, 18 June 2023
// read_only global f64 F64_SMALLEST_POSITIVE = 0.0;
// read_only global f64 F64_EPSILON           = 0.0;

////////////////////////////////
// Date
typedef struct {
    i16 year;
    i8  month;           // from 1 to  12
    i8  week;            // from 0 to  52
    i8  day_of_week;     // from 0 to   6
    i8  day;             // from 1 to  31
    i8  hour;            // from 0 to  23
    i8  minute;          // from 0 to  59
    i8  second;          // from 0 to  59
    i16 milliseconds;    // from 0 to 999
} Date;

////////////////////////////////
// Assertions

#if COMPILER_MSVC
    #define debugBreak() __debugbreak()
#elif COMPILER_CLANG
    #define debugBreak() __builtin_debugtrap()
#elif COMPILER_GCC
    #define debugBreak() __builtin_trap()
#else
    #include <signal.h>
    #define debugBreak() raise(SIGTRAP)
#endif

#undef assert
#define assert(condition)               \
    multilineMacroBegin                 \
        if (!(condition)) debugBreak(); \
    multilineMacroEnd

#if BUILD_ROOT
    #define staticAssert(c, label) \
        i8 static_assert_##label[(c)?(1):(-1)]
#else
    #define staticAssert(c, label)
#endif

#define notImplemented assert(!"Not Implemented")
#define invalidPath    assert(!"Invalid Path")

#endif // BASIC_TYPES_H
