typedef _Bool bool;

#define null  0
#define true  1
#define false 0

#include <stdint.h>

typedef char      i8;
typedef short     i16;
typedef int       i32;
typedef long long i64;

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

typedef float  f32;
typedef double f64;

typedef size_t    usize;
typedef ptrdiff_t isize;

#define arraySize(value)                   ((sizeof((value))) / (sizeof((value[0]))))
#define maybeDefault(value, default_value) (((value) == 0) ? (default_value) : (value))
#define enumToInt(enum_)                   ((int)(enum_))
#define cast(Type, value)                  ((Type)(value))

#define staticAssert(expression)           _Static_assert((expression), "Static assert failed!")
#define dynamicAssert(expression) (void)(                                        \
    (!!(expression)) ||                                                              \
    (_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0) \
    )

#include <stdio.h>

#include "d3d12.h"
#include "dxgi1_4.h"

int main(void) {
    bool succeeded = true;
    
    ID3D12Debug1* debug_controller = NULL;

    succeeded = SUCCEEDED(D3D12GetDebugInterface(&IID_ID3D12Debug1, &debug_controller));
    if (succeeded)
    {
        ID3D12Debug_EnableDebugLayer(debug_controller);
        // debug_controller->EnableDebugLayer();
        // debug_controller->SetEnableGPUBasedValidation(true);
        // debug_controller->SetEnableSynchronizedCommandQueueValidation(true);
        // debug_controller->Release();
    }

    return 0;
}
