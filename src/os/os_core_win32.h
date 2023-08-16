#pragma push_macro("function")

#undef function

#include "windows.h"
#include "windowsx.h"
#include "tlhelp32.h"
#include "shlobj.h"

#pragma pop_macro("function")

core_global HINSTANCE os_win32_instance;
