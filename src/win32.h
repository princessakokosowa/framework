#ifndef INCLUDE_WIN32_H
#define INCLUDE_WIN32_H

/////////////////////////////////////////////////////////////////////////////////////////
// Include this pile of garbage.
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN

#define NOMINMAX
#include "Windows.h"

// Include the actaully useful headers.
#define COBJMACROS

#pragma warning(push)
#pragma warning(disable:4115) // 4115: Named type definition in parentheses.

#include "d3d12.h"
#include "dxgi1_6.h"

#pragma warning(pop)

#endif // INCLUDE_WIN32_H
