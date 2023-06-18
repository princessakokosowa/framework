#ifndef BASIC_BUILD_H
#define BASIC_BUILD_H

////////////////////////////////
// MSVC Extraction

#if defined(_MSC_VER)
    #define COMPILER_MSVC 1

    #if defined(_WIN32)
        #define OS_WINDOWS 1
    #else
        #error             _MSC_VER is defined, but _WIN32 is not. This setup is not supported.
    #endif // 

    #if defined(_M_AMD64)
        #define ARCH_X64   1
    #elif defined(_M_IX86)
        #define ARCH_X86   1
    #elif defined(_M_ARM64)
        #define ARCH_ARM64 1
    #elif defined(_M_ARM)
        #define ARCH_ARM32 1
    #else
        #error             Target architecture is not supported. _MSC_VER is defined, but one of {_M_AMD64, _M_IX86, _M_ARM64, _M_ARM} is not.
    #endif // 
    
    #if _MSC_VER >= 1920
        #define COMPILER_MSVC_YEAR 2019
    #elif _MSC_VER >= 1910
        #define COMPILER_MSVC_YEAR 2017
    #elif _MSC_VER >= 1900
        #define COMPILER_MSVC_YEAR 2015
    #elif _MSC_VER >= 1800
        #define COMPILER_MSVC_YEAR 2013
    #elif _MSC_VER >= 1700
        #define COMPILER_MSVC_YEAR 2012
    #elif _MSC_VER >= 1600
        #define COMPILER_MSVC_YEAR 2010
    #elif _MSC_VER >= 1500
        #define COMPILER_MSVC_YEAR 2008
    #elif _MSC_VER >= 1400
        #define COMPILER_MSVC_YEAR 2005
    #else
        #define COMPILER_MSVC_YEAR 0
    #endif // 

////////////////////////////////
// Clang Extraction

#elif defined(__clang__)
    #define COMPILER_CLANG 1

    #if defined(__MINGW32__) || defined(__MINGW64__)
        #define OS_WINDOWS 1
    #elif defined(__APPLE__) && defined(__MACH__)
        #define OS_MAC     1
    #elif defined(__gnu_linux__)
        #define OS_LINUX   1
    #else
        #error           __clang__ is defined, but one of {__APPLE__, __gnu_linux__} is not. This setup is not supported.
    #endif // defined(__APPLE__), defined(__MACH__) and defined(__gnu_linux__)

    #if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
        #define ARCH_X64   1
    #elif defined(i386) || defined(__i386) || defined(__i386__)
        #define ARCH_X86   1
    #elif defined(__aarch64__)
        #define ARCH_ARM64 1
    #elif defined(__arm__)
        #define ARCH_ARM32 1
    #else
        #error             Target architecture is not supported. __clang__ is defined, but one of {__amd64__, __amd64, __x86_64__, __x86_64, i386, __i386, __i386__, __aarch64__, __arm__} is not.
    #endif // defined(__amd64__), defined(__amd64), defined(__x86_64__), defined(__x86_64), defined(i386) || defined(__i386) || defined(__i386__), defined(__aarch64__) and defined(__arm__)

////////////////////////////////
// GCC Extraction

#elif defined(__GNUC__) || defined(__GNUG__)
    #define COMPILER_GCC 1

    #if defined(__gnu_linux__)
        #define OS_LINUX 1
    #else
        #error           __GNUC__ or __GNUG__ is defined, but __gnu_linux__ is not. This setup is not supported.
    #endif

    #if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
        #define ARCH_X64   1
    #elif defined(i386) || defined(__i386) || defined(__i386__)
        #define ARCH_X86   1
    #elif defined(__aarch64__)
        #define ARCH_ARM64 1
    #elif defined(__arm__)
        #define ARCH_ARM32 1
    #else
        #error             Target architecture is not supported. __GNU_C__ or __GNUG__ is defined, but one of {__amd64__, __amd64, __x86_64__, __x86_64, i386, __i386, __i386__, __aarch64__, __arm__} is not.
    #endif

#else
    #error Compiler is not supported. _MSC_VER, __clang__, __GNUC__, or __GNUG__ must be defined.
#endif // defined(_MSC_VER), defined(__clang__),  defined(__GNUC__) and defined(__GNUG__)

#if defined(ARCH_X64)
    #define ARCH_64BIT 1
#elif defined(ARCH_X86)
    #define ARCH_32BIT 1
#endif // defined(ARCH_X64) and defined(ARCH_X86)

////////////////////////////////
// Language

#if defined(__cplusplus)
    #define LANG_CPP 1
#else
    #define LANG_C   1
#endif // defined(__cplusplus)

////////////////////////////////
// Zero

#if !defined(ARCH_32BIT)
    #define ARCH_32BIT     0
#endif // !defined(ARCH_32BIT)

#if !defined(ARCH_64BIT)
    #define ARCH_64BIT     0
#endif // !defined(ARCH_64BIT)

#if !defined(ARCH_X64)
    #define ARCH_X64       0
#endif // !defined(ARCH_X64)

#if !defined(ARCH_X86)
    #define ARCH_X86       0
#endif // !defined(ARCH_X86)

#if !defined(ARCH_ARM64)
    #define ARCH_ARM64     0
#endif // !defined(ARCH_ARM64)

#if !defined(ARCH_ARM32)
    #define ARCH_ARM32     0
#endif // !defined(ARCH_ARM32)

#if !defined(COMPILER_MSVC)
    #define COMPILER_MSVC  0
#endif // !defined(COMPILER_MSVC)

#if !defined(COMPILER_GCC)
    #define COMPILER_GCC   0
#endif // !defined(COMPILER_GCC)

#if !defined(COMPILER_CLANG)
    #define COMPILER_CLANG 0
#endif // !defined(COMPILER_CLANG)

#if !defined(OS_WINDOWS)
    #define OS_WINDOWS     0
#endif // !defined(OS_WINDOWS)

#if !defined(OS_LINUX)
    #define OS_LINUX       0
#endif // !defined(OS_LINUX)

#if !defined(OS_MAC)
    #define OS_MAC         0
#endif // !defined(OS_MAC)

#if !defined(LANG_CPP)
    #define LANG_CPP       0
#endif // !defined(LANG_CPP)

#if !defined(LANG_C)
    #define LANG_C         0
#endif // !defined(LANG_C)

////////////////////////////////
// Build Parameters

#define BUILD_LINK_MODE_SOURCE  0
#define BUILD_LINK_MODE_STATIC  1
#define BUILD_LINK_MODE_DYNAMIC 2

#if !defined(BUILD_DEBUG)
    #define BUILD_DEBUG          1
#endif // !defined(BUILD_DEBUG)

#if !defined(BUILD_CONSOLE_OUTPUT)
    #define BUILD_CONSOLE_OUTPUT 0
#endif // !defined(BUILD_CONSOLE_OUTPUT)

#if !defined(BUILD_CORE_LINK_MODE)
    #define BUILD_CORE_LINK_MODE BUILD_LINK_MODE_DYNAMIC
#endif // !defined(BUILD_CORE_LINK_MODE)

#if !defined(BUILD_ROOT)
    #if BUILD_CORE_LINK_MODE == BUILD_LINK_MODE_SOURCE
        #define BUILD_ROOT 1
    #elif BUILD_CORE_LINK_MODE == BUILD_LINK_MODE_STATIC
        #define BUILD_ROOT 1
    #elif BUILD_CORE_LINK_MODE == BUILD_LINK_MODE_DYNAMIC
        #define BUILD_ROOT 0
    #endif
#endif // !defined(BUILD_ROOT)

#endif // BASIC_BUILD_H
