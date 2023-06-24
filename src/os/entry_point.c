#if OS_WINDOWS
    #include "entry_point_win32.c"
#else
    #error   Entry point not defined for OS.
#endif
