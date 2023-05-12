// @TODO
// Figure out the order of these included headers.
//     ~ princessakokosowa, 3rd of April 2023

// Foreign headers.
#include "foreign.h"

// Local headers.
#include "foundation.h"
#include "context.h"
#include "array.h"
#include "arena.h"
#include "pool.h"
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

#define BUFFER_COUNT 64

void graphicsTests(void) {
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
    //         succeeded = SUCCEEDED(D3D12GetDebugInterface(&IID_ID3D12Debug1, cast(void**, &debug_controller)));
    //         if (succeeded == true) {
    //             ID3D12Debug1_EnableDebugLayer(debug_controller);
    //             ID3D12Debug1_SetEnableSynchronizedCommandQueueValidation(debug_controller, true);
    //             ID3D12Debug1_Release(debug_controller);
    //         }
    //     }
    // }
}

void allocatorsTests(void) {
    // Create some sort of memory pool (here: `Arena`).
    {
        Arena arena = Arena_create(&(ArenaDescription) { 0, });

        // Get some memory directly from it via `arenaGet`.
        {
            isize buf_size  = sizeof(u8) * BUFFER_COUNT;
            u8    *buf      = Arena_get(&arena, buf_size);

            u8 strings[][BUFFER_COUNT] = {
                "Hi,",
                "my",
                "name",
                "is,",
                "what?",
            };

            for (isize i = 0; i < arrayCount(strings); i += 1) {
                stringCopy(buf, cast(u8*, strings[i]));

                printf("%s\n", buf);
            }
        }

        // Get an allocator out of it and use it to get some memory.
        {
            Allocator arena_allocator = Arena_getAllocator(&arena);

            isize buf_size  = sizeof(u8) * BUFFER_COUNT;
            u8    *buf      = arena_allocator.procedure(ALLOCATOR_MODE_ALLOCATE, &(AllocatorDescription) {
                .size_to_be_allocated_or_resized = buf_size,
                .impl                            = cast(void*, &arena),
            });

            u8 strings[][BUFFER_COUNT] = {
                "Hi,",
                "my",
                "name",
                "is,",
                "who?",
            };

            for (isize i = 0; i < arrayCount(strings); i += 1) {
                stringCopy(buf, cast(u8*, strings[i]));

                printf("%s\n", buf);
            }
        }

        // Set `Context` allocators and the default `alloc`-, `resize`- and `free`-based
        // interface.
        {
            Allocator arena_allocator = Arena_getAllocator(&arena);
            Context_setAllocators(&arena_allocator);

            isize buf_size  = sizeof(u8) * BUFFER_COUNT;
            u8    *buf      = alloc(buf_size);

            u8 strings[][BUFFER_COUNT] = {
                "Hi,",
                "my",
                "name",
                "is,",
                "chka-chka, Slim Shady",
            };

            for (isize i = 0; i < arrayCount(strings); i += 1) {
                stringCopy(buf, cast(u8*, strings[i]));

                printf("%s\n", buf);
            }

            Context_remindAllocators();
        }

        Arena_destroy(&arena);
    }

    // Set `Context` allocators, similarly to how this is done in the previous scope,
    // but using predefined temporary allocator (and its temporary storage).
    {
        Context_setAllocators(&temporary_allocator);

        isize buf_size  = sizeof(u8) * BUFFER_COUNT;
        u8    *buf      = alloc(buf_size);

        u8 strings[][BUFFER_COUNT] = {
            "Ghost of Mother lingering death",
            "Ghost on Mother's bed",
            "Black strands on the pillow contour of her health",
            "Twisted face upon the head",
        };

        for (isize i = 0; i < arrayCount(strings); i += 1) {
            stringCopy(buf, cast(u8*, strings[i]));

            printf("%s\n", buf);
        }

        TemporaryStorage_reset();
        Context_remindAllocators();
    }

    // Here goes the default allocator.
    {
        isize buf_size  = sizeof(u8) * BUFFER_COUNT;
        u8    *buf      = alloc(buf_size);

        u8 strings[][BUFFER_COUNT] = {
            "Ghost of Mother lingering death",
            "Ghost on Mother's bed",
            "Black strands on the pillow contour of her health",
            "Twisted face upon the head",
        };

        for (isize i = 0; i < arrayCount(strings); i += 1) {
            stringCopy(buf, cast(u8*, strings[i]));

            printf("%s\n", buf);
        }

        free(buf);
    }
}

void arraysTests(void) {
    {
        f32 *array = null;
        Array_add(array, 334);
        Array_add(array, 1);
        Array_add(array, -1);
        Array_add(array, 0);

        Array_free(array);
    }

    {
        Context_setAllocators(&temporary_allocator);

        f32 *array = null;
        Array_add(array, 334);
        Array_add(array, 1);
        Array_add(array, -1);
        Array_add(array, 0);

        Array_free(array);
        TemporaryStorage_reset();
        Context_remindAllocators();
    }

    {
        Arena     arena           = Arena_create(&(ArenaDescription) { 0, });
        Allocator arena_allocator = Arena_getAllocator(&arena);

        Context_setAllocators(&arena_allocator);

        f32 *array = null;
        Array_add(array, 334.f);
        Array_add(array, 1.f);
        Array_add(array, -1.f);
        Array_add(array, 0.f);
        Array_add(array, 1337.f);
        Array_add(array, 0xfade);
        Array_add(array, 0xdeaf);
        Array_add(array, 0xcafe);
        Array_add(array, 0xbabe);
        Array_add(array, 0.f);

        for (f32 i = 0.f; i < 127.f; i += 1.f) {
            Array_add(array, i);
        }

        printf("%5.1f ", Array_pop(array));

        Array_addAt(array, 666.f, 44);

        for (isize i = 0; i < Array_count(array); i += 1) {
            printf("%5.1f\n", array[i]);
        }

        Array_removeByValue(array, 0.f);
        Array_removeAtIndex(array, 0);

        Array_removeByValueOrdered(array, 1.f);
        Array_removeAtIndexOrdered(array, 33);

        Array_resize(array, 96);
        Array_resize(array, 384);

        Array_free(array);
        Context_remindAllocators();
        Arena_destroy(&arena);
    }

    {
        Arena     arena           = Arena_create(&(ArenaDescription) { 0, });
        Allocator arena_allocator = Arena_getAllocator(&arena);

        Context_setAllocators(&arena_allocator);

        f32 *array = null;
        Array_reserve(array, 384);

        Array_free(array);
        Context_remindAllocators();
        Arena_destroy(&arena);
    }
}

int main(void) {
    graphicsTests();
    allocatorsTests();
    arraysTests();

    return 0;
}
