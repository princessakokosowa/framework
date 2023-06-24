#ifndef OS_H
#define OS_H

#if !defined(Allocator_reserve)
    #define Allocator_reserve  Os_reserve
#endif

#if !defined(Allocator_release)
    #define Allocator_release  Os_release
#endif

#if !defined(Allocator_commit)
    #define Allocator_commit   Os_commit
#endif

#if !defined(Allocator_decommit)
    #define Allocator_decommit Os_decommit
#endif

#include "core.h"

#if OS_WINDOWS
    #include "core_win32.h"
#else
    #error   OS layer not implemented.
#endif

#endif // OS_H
