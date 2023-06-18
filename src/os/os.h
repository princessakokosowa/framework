#ifndef OS_H
#define OS_H

#if OS_WINDOWS
    #include "win32.h"
#else
    #error   OS layer not implemented.
#endif

#endif // OS_H
