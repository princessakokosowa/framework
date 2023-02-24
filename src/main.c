#include "foundation.h"

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

    #include <d3d12.h>
    #include <d3dcompiler.h>
    #include <dxgi1_6.h>
    #include <dxgidebug.h>
    #include <d3d12sdklayers.h>

	#pragma warning(pop)
#undef COBJMACROS

// Direct3D 12 etc.
// #include "d3d12shader.h"

// DirectX Shader Compiler etc.
// #include "dxcapi.h"

#include <stdio.h>

typedef enum Flag {
    FLAG_ENABLE_DEBUG_LAYER        = 1 << 0,
    FLAG_ENABLE_SHADER_DEBUGGING   = 1 << 1,
    FLAG_ENABLE_STABLE_POWER_STATE = 1 << 2,
} Flag;

// typedef struct Flags {
//     usize value;
// } Flags;

// That's less annoying.
typedef usize Flags;

int main(void) {
    Flags flags = FLAG_ENABLE_DEBUG_LAYER | FLAG_ENABLE_SHADER_DEBUGGING | FLAG_ENABLE_STABLE_POWER_STATE;

    // @TODO
    // Remove this or incorporate somewhere after I am done with DXC.
    //     - princessakokosowa, 24th of February 2023
    //
    // Enable the debug layer (and maybe a couple more things) provided by Direct3D 12.
    {
        bool       succeeded                        = true;
        bool const should_enable_debug_layer        = cast(bool, (flags & FLAG_ENABLE_DEBUG_LAYER       ) != 0);
        bool const should_enable_shader_debugging   = cast(bool, (flags & FLAG_ENABLE_SHADER_DEBUGGING  ) != 0);
        bool const should_enable_stable_power_state = cast(bool, (flags & FLAG_ENABLE_STABLE_POWER_STATE) != 0);

        ID3D12Debug1* debug_controller = null;

        if (should_enable_debug_layer == true) {
            succeeded = SUCCEEDED(D3D12GetDebugInterface(&IID_ID3D12Debug1, &debug_controller));
            if (succeeded == true) {
                ID3D12Debug1_EnableDebugLayer(debug_controller);
                ID3D12Debug1_SetEnableSynchronizedCommandQueueValidation(debug_controller, true);
                ID3D12Debug1_Release(debug_controller);
            }
        }
    }

    return 0;
}
