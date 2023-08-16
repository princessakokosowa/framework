#ifndef OS_H
#define OS_H

#if !defined(AllocatorImpl_reserve)
    #define AllocatorImpl_reserve  Os_reserve
#endif

#if !defined(AllocatorImpl_release)
    #define AllocatorImpl_release  Os_release
#endif

#if !defined(AllocatorImpl_commit)
    #define AllocatorImpl_commit   Os_commit
#endif

#if !defined(AllocatorImpl_decommit)
    #define AllocatorImpl_decommit Os_decommit
#endif

#include "os_core.h"

#if OS_WINDOWS
    #include "os_core_win32.h"
#else
    #error   OS layer not implemented.
#endif

#endif // OS_H
