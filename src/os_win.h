#ifndef INCLUDE_WIN32_H
#define INCLUDE_WIN32_H

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN

#pragma push_macro("function")
#undef function

#define NOMINMAX
#include "windows.h"
#include "windowsx.h"
#include "tlhelp32.h"
#include "Shlobj.h"

#pragma pop_macro("function")

#define COBJMACROS

#pragma warning(push)
#pragma warning(disable : 4115) // 4115: Named type definition in parentheses.

#include "d3d12.h"
#include "dxgi1_6.h"

#pragma warning(pop)

#endif // INCLUDE_WIN32_H
