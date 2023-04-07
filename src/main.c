// @TODO
// Figure out the order of these included headers.
//     ~ princessakokosowa, 3rd of April 2023

// Foreign headers.
#include "foreign.h"

// Local headers.
#include "foundation.h"
#include "context.h"
#include "dynamic_array.h"
#include "arena.h"
#include "gpu.h"

f64 timeStamp(void) {
    // `counter_start` and `counter_end` are both, eh, counters, which are dependent on
    // the clock frequency.
    //
    // To get the time in seconds, you need to know the start and end stamp counter and
    // divide it by the clock frequency.
    //
    // @TODO
    // We can have the time returned in e.g. milliseconds or nanoseconds, `frequency`
    // should be multiplied accordingly, but I haven't decided yet how to do this. Should
    // it be a separate procedure, or e.g. an if-else block in this procedure and
    // possibly pass the mode as an argument? I don't know, mate, but it's not really
    // relevant to me at the moment.
    //     ~ princessakokosowa, 8th of March 2023
    isize static counter_start;
    isize static frequency;
    if (counter_start == 0) QueryPerformanceCounter(cast(LARGE_INTEGER*, &counter_start));
    if (frequency     == 0) QueryPerformanceFrequency(cast(LARGE_INTEGER*, &frequency));

    isize counter_end;
    QueryPerformanceCounter(cast(LARGE_INTEGER*, &counter_end));

    isize const counter_split = counter_end - counter_start;
    f64   const split         = cast(f64, counter_split) / cast(f64, frequency);

    return split;
}

// _Pragma("startup contextCreate")
// _Pragma("exit    contextDestroy")

#ifndef _MSC_VER

    void __attribute__ ((constructor)) preload(void) {
        contextCreate();
    }

    void __attribute__ ((destructor)) tidyUp(void) {
        contextDestroy();
    }

#endif // _MSC_VER

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

void copy(void *destination, isize destination_size, void *source, isize source_size) {
    assert(destination != null);
    assert(source != null);
    assert(destination_size >= source_size);

    for (isize i = 0; i < source_size; i += 1) {
        *(cast(u8*, destination) + i) = *(cast(u8*, source) + i);
    }
}

isize stringCount(u8 *c_string) {
    isize count = 0;
    while (*c_string) {
        c_string += 1;
        count    += 1;
    }

    return count;
}

void stringCopy(u8 *destination, u8 *source) {
    isize source_count = stringCount(source);

    copy(destination, source_count, source, source_count);

    destination[source_count] = '\0';
}

int main(void) {
    Flags flags = FLAG_ENABLE_DEBUG_LAYER | FLAG_ENABLE_SHADER_DEBUGGING | FLAG_ENABLE_STABLE_POWER_STATE;

    // @TODO
    // Remove this or incorporate somewhere after I am done with DXC.
    //     - princessakokosowa, 24th of February 2023
    //
    // Enable the debug layer (and maybe a couple more things) provided by Direct3D 12.
    {
        bool succeeded                        = true;
        bool should_enable_debug_layer        = cast(bool, (flags & FLAG_ENABLE_DEBUG_LAYER       ) != 0);
        // bool const should_enable_shader_debugging   = cast(bool, (flags & FLAG_ENABLE_SHADER_DEBUGGING  ) != 0);
        // bool const should_enable_stable_power_state = cast(bool, (flags & FLAG_ENABLE_STABLE_POWER_STATE) != 0);

        ID3D12Debug1 *debug_controller = null;

        if (should_enable_debug_layer == true) {
            succeeded = SUCCEEDED(D3D12GetDebugInterface(&IID_ID3D12Debug1, cast(void**, &debug_controller)));
            if (succeeded == true) {
                ID3D12Debug1_EnableDebugLayer(debug_controller);
                ID3D12Debug1_SetEnableSynchronizedCommandQueueValidation(debug_controller, true);
                ID3D12Debug1_Release(debug_controller);
            }
        }
    }

    Arena arena = arenaCreate(&(ArenaDescription) {
        0,
    });

    isize buf_count = 24;
    isize buf_size  = sizeof(u8) * buf_count;
    u8    *buf      = arenaGet(&arena, buf_size);

    u8 strings[][24] = {
    //           ^^
    //           FML
        "Hello,",
        "world!",
        "I am still standing.",
    };

    for (isize i = 0; i < arrayCount(strings); i += 1) {
        stringCopy(buf, cast(u8*, strings[i]));

        printf("%s\n", buf);
    }

    return 0;
}
