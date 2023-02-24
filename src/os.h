// Exclude rarely-used items from Windows headers.
#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif

// Include this pile of random garbage.
#define NOMINMAX
#include <Windows.h>

// Include the actaully useful headers.
#define COBJMACROS

#pragma warning(push)
#pragma warning(disable:4115) // 4115: Named type definition in parentheses.

#include "d3d12.h"
#include "dxgi1_6.h"

#include <d3dcompiler.h>
#include <dxgidebug.h>

#pragma warning(pop)

#undef COBJMACROS
