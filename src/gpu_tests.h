#ifndef INCLUDE_GPU_TESTS_H
#define INCLUDE_GPU_TESTS_H

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

void GpuTests_test(void) {
    // Flags flags = FLAG_ENABLE_DEBUG_LAYER | FLAG_ENABLE_SHADER_DEBUGGING | FLAG_ENABLE_STABLE_POWER_STATE;
    // 
    // @TODO
    // Remove this or incorporate somewhere after I am done with DXC.
    //     - princessakokosowa, 24th of February 2023
    //
    // Enable the debug layer (and maybe a couple more things) provided by Direct3D 12.
    // {
    //     bool succeeded                        = true;
    //     bool should_enable_debug_layer        = cast(bool, (flags & FLAG_ENABLE_DEBUG_LAYER       ) != 0);
    //     // bool const should_enable_shader_debugging   = cast(bool, (flags & FLAG_ENABLE_SHADER_DEBUGGING  ) != 0);
    //     // bool const should_enable_stable_power_state = cast(bool, (flags & FLAG_ENABLE_STABLE_POWER_STATE) != 0);
    // 
    //     ID3D12Debug1 *debug_controller = null;
    // 
    //     if (should_enable_debug_layer == true) {
    //         succeeded = SUCCEEDED(D3D12GetDebugInterface(&IID_ID3D12Debug1, cast(void **, &debug_controller)));
    //         if (succeeded == true) {
    //             ID3D12Debug1_EnableDebugLayer(debug_controller);
    //             ID3D12Debug1_SetEnableSynchronizedCommandQueueValidation(debug_controller, true);
    //             ID3D12Debug1_Release(debug_controller);
    //         }
    //     }
    // }
}

#endif // INCLUDE_GPU_TESTS_H