#include "os_core.c"

#if OS_WINDOWS
    #include "os_core_win32.c"
#else
    #error   OS layer not implemented.
#endif
