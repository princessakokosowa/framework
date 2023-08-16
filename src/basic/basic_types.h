#ifndef BASIC_TYPES_H
#define BASIC_TYPES_H

#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "math.h"
#include "time.h"

#if COMPILER_MSVC
    #include "intrin.h"
#endif

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

// assignments

#define true               1
#define false              0
#define null               0

// utility

#define multilineMacroBegin do {
#define multilineMacroEnd   } while (false)

// compiler helpers

#define unusedVariable(variable) cast(void, (variable))

// integer/pointer/array/type manipulations

#define cast(Type, value) ((Type)(value))

#define arrayCount(array) (sizeof(array) / sizeof((array)[0]))

#define intFromPtr(ptr)     cast(usize , cast(u8 *, ptr) - 0)
#define ptrFromInt(integer) cast(void *, cast(u8 *, 0  ) + i)

#define member(Type, member_name)              cast(Type *, 0)->member_name
#define offsetOf(Type, member_name)            intFromPtr(&member(Type, member_name))
#define baseFromMember(Type, member_name, ptr) cast(Type *, cast(u8 *, ptr) - offsetOf(Type, member_name))

#define bytes(value)      (           (value) /* <<  0 */)
#define kilobytes(value)  (           (value)    << 10   )
#define megabytes(value)  (           (value)    << 20   )
#define gigabytes(value)  (cast(usize, value)    << 30   )
#define terabytes(value)  (cast(usize, value)    << 40   )

#define thousand(value) ((value)) * 1000        )
#define million(value)  ((value)) * 1000000     )
#define billion(value)  ((value)) * 1000000000ll)

#define swap(Type, a, b)         \
    multilineMacroBegin          \
        Type _Swap_buffer = (a); \
                                 \
        (a) = (b);               \
        (b) = _Swap_buffer;      \
    multilineMacroEnd

// linked list helpers

#define Queue_pushSetNext(first, last, node, next) \
    multilineMacroBegin                            \
        if ((first) == null) {                     \
            (first) = (node);                      \
            (last)  = (node);                      \
        } else {                                   \
            (last)->next = (node);                 \
            (last)       = (node);                 \
        }                                          \
                                                   \
        (node)->next = null;                       \
    multilineMacroEnd

#define Queue_pushFrontSetNext(first, last, node, next) \
    multilineMacroBegin                                 \
        if ((first) == null) {                          \
            (first)      = (node);                      \
            (last)       = (node);                      \
            (node)->next = null;                        \
        } else {                                        \
            (node )->next = (first);                    \
            (first)       = (node );                    \
        }                                               \
    multilineMacroEnd

#define Queue_popSetNext(first, last, next) \
    multilineMacroBegin                     \
        if ((first) == (last)) {            \
            (first) = null;                 \
            (last ) = null;                 \
        } else {                            \
            (first) = (first)->next;        \
        }                                   \
    multilineMacroEnd

#define Queue_push(first, last, node)      Queue_pushSetNext(first, last, node, next)
#define Queue_pushFront(first, last, node) Queue_pushFrontSetNext(first, last, node, next)
#define Queue_pop(first, last)             Queue_popSetNext(first, last, next)

#define Stack_pushSetNext(first, node, next) \
    multilineMacroBegin                      \
        (node )->next = (first);             \
        (first)       = (node );             \
    multilineMacroEnd

#define Stack_popSetNext(first, next)                 \
    multilineMacroBegin                               \
        if ((first) == null) (first) = null;          \
        else                 (first) = (first)->next; \
    multilineMacroEnd

#define Stack_push(first, node) Stack_pushSetNext(first, node, next)
#define Stack_pop(first)        Stack_popSetNext(first, next)

#define Dll_insertSetNextPrevious(first, last, p, node, next, prev) \
    multilineMacroBegin                                          \
        if ((first) == null) {                                   \
            (first)       = (node);                              \
            (last)        = (node);                              \
            (node)->next) = null;                                \
            (node)->prev) = null;                                \
        } else if ((p) == null) {                                \
            (node)->prev = null;                                 \
            (node)->next = (first);                              \
                                                                 \
            if ((first) != null) (first)->prev = (node);         \
                                                                 \
            (first) = (node);                                    \
        } else {                                                 \
            if ((p) != null) (p)->next->prev = (node);           \
                                                                 \
            (node)->next = (p)->next;                            \
            (node)->prev = (p);                                  \
            (p)->next    = (node);                               \
                                                                 \
            if ((p) == (last)) (last) = (node);                  \
        }                                                        \
    multilineMacroEnd

#define Dll_pushSetNextPrevious(first, last, node, next, prev) Dll_insertSetNextPrevious(first, last, last, node, next, prev)

#define Dll_removeSetNextPrevious(first, last, node, next, prev)              \
    multilineMacroBegin                                                    \
        if ((first) == (node)) {                                           \
            (first) = (first)->next;                                       \
                                                                           \
            if ((first) == null) (last)         = null;                    \
            else                 (first)->prev) = null;                    \
        } else if ((last) == (node)) {                                     \
            (last) = (last)->prev;                                         \
                                                                           \
            if ((last) == null) (first)      = null;                       \
            else                (last)->next = null;                       \
        } else {                                                           \
            if ((node)->next != null) ((node)->next->prev = (node)->prev); \
            if ((node)->prev != null) ((node)->prev->next = (node)->next); \
        }                                                                  \
    multilineMacroEnd

#define Dll_push(first, last, node)      Dll_pushSetNextPrevious(first, last, node, next, prev)
#define Dll_pushFront(first, last, node) Dll_pushSetNextPrevious(last, first, node, prev, next)
#define Dll_pushAt(first, last, p, node) Dll_insertSetNextPrevious(first, last, p, node, next, prev)
#define Dll_remove(first, last, node)    Dll_removeSetNextPrevious(first, last, node, next, prev)

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
#define forRange(from, to)                               \
    for (isize index = (from); index < (to); index += 1)

#define forRangeStep(from, to, step)                          \
    for (isize index = (from); index < (to); index += (step))

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

////////////////////////////////
// Limits

read_only global i8 U8_MAX = 0xFF;
read_only global i8 U8_MIN = 0;

read_only global i16 U16_MAX = 0xFFFF;
read_only global i16 U16_MIN = 0;

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
// Base Enums

typedef enum {
    Side_Invalid = -1,
    Side_Min         ,
    Side_Max         ,

    // ...

    Side_Count       ,
} Side;

typedef enum {
    Axis2_Invalid = -1,
    Axis2_X           ,
    Axis2_Y           ,

    // ...

    Axis2_COUNT       ,
} Axis2;

#define Axis2_flip(a) ((Axis2)(!(a)))

typedef enum {
    Axis3_Invalid = -1,
    Axis3_X           ,
    Axis3_Y           ,
    Axis3_Z           ,

    // ...

    Axis3_Count       ,
} Axis3;

typedef enum {
    Axis4_Invalid = -1,
    Axis4_X           ,
    Axis4_Y           ,
    Axis4_Z           ,
    Axis4_W           ,

    // ...

    Axis4_COUNT       ,
} Axis4;

typedef enum {
    Corner_Invalid = -1,
    Corner_00          ,
    Corner_01          ,
    Corner_10          ,
    Corner_11          ,

    // ...

    Corner_COUNT       ,
} Corner;

typedef enum {
    Dir2_Left  = 0,
    Dir2_Up       ,
    Dir2_Right    ,
    Dir2_Down     ,

    // ...

    Dir2_Count    ,
} Dir2;

typedef enum {
    Comparison_Null                 = 0,
    Comparison_EqualTo                 ,
    Comparison_NotEqualTo              ,
    Comparison_LessThan                ,
    Comparison_LessThanOrEqualTo       ,
    Comparison_GreaterThan             ,
    Comparison_GreaterThanOrEqualTo    ,

    // ...

    Comparison_Count                   ,
} Comparison;

////////////////////////////////
// Member offset helper

typedef struct {
    usize v;
} MemberOffset;

#define memberOff(StructName, memb)             (MemberOffset){ offsetOf(StructName, memb), }
#define memberFromOff(Type, ptr, member_offset) *cast(Type *, cast(u8 *, ptr + member_offset.v))

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
#define assert(condition)          if (!(condition)) debugBreak()

#if BUILD_ROOT
    #define staticAssert(c, label) i8 static_assert_##label[(c)?(1):(-1)]
#else
    #define staticAssert(c, label)
#endif

#define unreachable(void)    assert(!"Unreachable")
#define notImplemented(void) assert(!"Not Implemented")
#define invalidPath(void)    assert(!"Invalid Path")

// memory copy/set operations

inline_function isize align(isize size, isize alignment) {
    isize mask = ~(alignment - 1);
    isize _    = size + (alignment - 1);

    return _ & mask;
}

// inline_function isize alignPtr(u8 *ptr, isize alignment) {
//     isize mask   = alignment - 1;
//     isize offset = cast(isize, ptr) & mask;
//     isize _      = alignment - offset;
// 
//     return _ & mask;
//  }

inline_function void *copy(void *destination, void *source, usize count_to_be_copied) {
    // Cache coherency.
    u8 *d = cast(u8 *, destination);
    u8 *s = cast(u8 *, source);

    // Copy blocks of size of `u32` bytes.
    //
    // @NOTE
    // Couldn't we try copying chunks of size of `u64`?
    //     ~ princessakokosowa, 18 June 2023
    for (usize index = 0; index < count_to_be_copied / sizeof(u32); index += 1) {
        *cast(u32 *,d) = *cast(u32 *, s);

        d += sizeof(u32);
        s += sizeof(u32);
    }

    // Copy remaining bytes individually.
    for (usize index = 0; index < count_to_be_copied % sizeof(u32); index += 1) {
        *d = *s;

        d += 1;
        s += 1;
    }

    return destination;
}

inline_function void *move(void *destination, void *source, usize count_to_be_copied) {
    // Cache coherency.
    u8 *d = cast(u8 *, destination);
    u8 *s = cast(u8 *, source);

    // Nothing to copy.
    if (d == s)
        return d;

    // Non-overlapping memory blocks, simply copy the memory.
    if (s + count_to_be_copied <= d || d + count_to_be_copied <= s) {
        copy(destination, source, count_to_be_copied);
    }

    if (d < s) {
        // Copy from start to end.
        while (count_to_be_copied > 0) {
            *d = *s;

            d += 1;
            s += 1;

            count_to_be_copied -= 1;
        }
    } else {
        // Copy from end to start.
        d += count_to_be_copied - 1;
        s += count_to_be_copied - 1;
        while (count_to_be_copied > 0) {
            *d = *s;

            d -= 1;
            s -= 1;

            count_to_be_copied -= 1;
        }
    }

    return destination;
}

inline_function void *set(void *destination, u8 value_to_set, usize count_to_be_set) {
    // Cache coherency.
    u8 *d = cast(u8 *, destination);

    for (usize index = 0; index < count_to_be_set; index += 1) {
        *d = value_to_set;
        d += 1;
    }

    return destination;
}

#endif // BASIC_TYPES_H
