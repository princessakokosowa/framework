#include "core.c"

#if OS_WINDOWS
    #include "core_win32.c"
#else
    #error   OS layer not implemented.
#endif
