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
#include "arena_mproved.h"


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

int main(void) {
#ifdef _MSC_VER

    contextCreate();

#endif // _MSC_VER

    Flags flags = FLAG_ENABLE_DEBUG_LAYER | FLAG_ENABLE_SHADER_DEBUGGING | FLAG_ENABLE_STABLE_POWER_STATE;

    // @TODO
    // Remove this or incorporate somewhere after I am done with DXC.
    //     - princessakokosowa, 24th of February 2023
    //
    // Enable the debug layer (and maybe a couple more things) provided by Direct3D 12.
    {
        bool       succeeded                        = true;
        bool const should_enable_debug_layer        = cast(bool, (flags & FLAG_ENABLE_DEBUG_LAYER       ) != 0);
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

    printf("Default allocator.\n");
    {
        f64 const start = timeStamp();

        isize *ptr = null;

        ptr = context.allocator->procedure(ALLOCATOR_MODE_ALLOCATE, &(AllocatorDescription){
            .size_to_be_allocated_or_resized = sizeof(isize) * 128,
        });

        ptr = context.allocator->procedure(ALLOCATOR_MODE_RESIZE, &(AllocatorDescription){
            .ptr_to_be_resized_or_freed      = ptr,
            .size_to_be_allocated_or_resized = sizeof(isize) * 192,
        });

        context.allocator->procedure(ALLOCATOR_MODE_FREE, &(AllocatorDescription){
            .ptr_to_be_resized_or_freed = ptr,
        });

        f64 const end = timeStamp();

        printf("%.9f\n", end - start);
    }

    printf("Temporary-storage-based allocator.\n");
    {
        f64 const start = timeStamp();

        isize *ptr = null;

        ptr = temporary_allocator.procedure(ALLOCATOR_MODE_ALLOCATE, &(AllocatorDescription){
            .size_to_be_allocated_or_resized = sizeof(isize) * 128,
        });

        ptr = temporary_allocator.procedure(ALLOCATOR_MODE_RESIZE, &(AllocatorDescription){
            .ptr_to_be_resized_or_freed      = ptr,
            .size_to_be_allocated_or_resized = sizeof(isize) * 192,
        });

        temporary_allocator.procedure(ALLOCATOR_MODE_FREE, &(AllocatorDescription){
            .ptr_to_be_resized_or_freed = ptr,
        });

        temporaryStorageReset();

        f64 const end = timeStamp();

        printf("%.9f\n", end - start);
    }

    printf("Default allocator (wrapped).\n");
    {
        f64 const start = timeStamp();

        isize *ptr = null;

        ptr = alloc(sizeof(isize) * 128);
        ptr = resize(ptr, sizeof(isize) * 192);
        free(ptr);

        f64 const end = timeStamp();

        printf("%.9f\n", end - start);
    }

    printf("Temporary-storage-based allocator (wrapped).\n");
    {
        f64 const start = timeStamp();

        contextSetAllocators(&temporary_allocator);

        isize *ptr = null;

        ptr = alloc(sizeof(isize) * 128);
        ptr = resize(ptr, sizeof(isize) * 192);
        free(ptr);

        f64 const end = timeStamp();

        {
            Arena arena = arenaCreate(&(ArenaDescription) {
                0,
            });

            Allocator arena_allocator = arenaGetAllocator(&arena);

            contextSetAllocators(&arena_allocator);

            isize const chuj_count = 5;
            char        *dummy     = alloc(sizeof(char) * chuj_count);

            dummy[0] = 'c';
            dummy[1] = 'h';
            dummy[2] = 'u';
            dummy[3] = 'j';
            dummy[4] = '\0';

            printf("%s\n", dummy);

            contextRemindAllocators();

            arenaDestroy(&arena);
        }

        {
            isize const heap_size = 32;
            i32         *ptr_2    = cast(i32*, alloc(sizeof(i32) * heap_size));

            Arena arena = arenaCreate(&(ArenaDescription) {
                .ptr_to_heap = ptr_2,
                .count       = sizeof(i32) * heap_size,
            });

            Allocator arena_allocator = arenaGetAllocator(&arena);

            contextSetAllocators(&arena_allocator);

            isize const chuj_count = 5;
            char        *dummy     = alloc(sizeof(char) * chuj_count);

            dummy[0] = 'c';
            dummy[1] = 'h';
            dummy[2] = 'u';
            dummy[3] = 'j';
            dummy[4] = '\0';

            printf("%s\n", dummy);

            contextRemindAllocators();

            arenaDestroy(&arena);

            free(ptr_2);
        }

        {
            Arena2 arena2 = arena2Create(&(Arena2Description) {
                0,
            });

            Allocator arena2_allocator = arena2GetAllocator(&arena2);

            contextSetAllocators(&arena2_allocator);

            isize const chuj_count = 5;
            char        *dummy     = alloc(sizeof(char) * chuj_count);

            dummy[0] = 'c';
            dummy[1] = 'h';
            dummy[2] = 'u';
            dummy[3] = 'j';
            dummy[4] = '\0';

            printf("%s\n", dummy);

            contextRemindAllocators();

            arena2Destroy(&arena2);
        }

        {
            isize const heap_size = 32;
            i32         *ptr_2    = cast(i32*, alloc(sizeof(i32) * heap_size));

            Arena2 arena2 = arena2Create(&(Arena2Description) {
                .ptr_to_heap = ptr_2,
                .count       = sizeof(i32) * heap_size,
            });

            Allocator arena2_allocator = arena2GetAllocator(&arena2);

            contextSetAllocators(&arena2_allocator);

            isize const chuj_count = 5;
            char        *dummy     = alloc(sizeof(char) * chuj_count);

            dummy[0] = 'c';
            dummy[1] = 'h';
            dummy[2] = 'u';
            dummy[3] = 'j';
            dummy[4] = '\0';

            printf("%s\n", dummy);

            contextRemindAllocators();

            arena2Destroy(&arena2);

            free(ptr_2);
        }

        temporaryStorageReset();
        contextRemindAllocators();

        printf("%.9f\n", end - start);
    }

    printf("Default allocator (wrapped), once again to test whether the default allocator has been reminded to `context`.\n");
    {
        f64 const start = timeStamp();

        isize *ptr = null;

        ptr = alloc(sizeof(isize) * 128);
        ptr = resize(ptr, sizeof(isize) * 192);
        free(ptr);

        f64 const end = timeStamp();

        printf("%.9f\n", end - start);
    }

    printf("Dynamic array with the default allocator.\n");
    {
        f64 const start = timeStamp();

        isize const iterations = 8192;
        isize const count      = 1024;
        for (isize i = 0; i < iterations; i += 1) {
            isize *dynamic_array = null;

            for (isize j = 0; j < count; j += 1) {
                darrAppend(dynamic_array, j * j * i);
            }

            darrFree(dynamic_array);
        }

        f64 const end = timeStamp();

        printf("%.9f\n", end - start);
    }


    printf("Dynamic array with the temporary-storage-based allocator.\n");
    {
        f64 const start = timeStamp();

        contextSetAllocators(&temporary_allocator);

        isize const iterations = 8192;
        isize const count      = 1024;
        for (isize i = 0; i < iterations; i += 1) {
            isize *dynamic_array = null;

            for (isize j = 0; j < count; j += 1) {
                darrAppend(dynamic_array, j * j * i);
            }

            temporaryStorageReset();
        }

        contextRemindAllocators();

        f64 const end = timeStamp();

        printf("%.9f\n", end - start);
    }

    printf("Temporary-storage-based allocator (wrapped), testing resize.\n");
    {
        f64 const start = timeStamp();

        contextSetAllocators(&temporary_allocator);

        isize *ptr   = null;
        isize *ptr_2 = null;

        ptr   = alloc(sizeof(isize) * 128);
        ptr   = resize(ptr, sizeof(isize) * 192);
        ptr_2 = alloc(sizeof(isize) * 128);
        ptr   = resize(ptr, sizeof(isize) * 256);
        free(ptr_2);
        free(ptr);

        f64 const end = timeStamp();

        temporaryStorageReset();
        contextRemindAllocators();

        printf("%.9f\n", end - start);
    }

#ifdef _MSC_VER

    return contextDestroy();

#else

    return 0;

#endif // defined(_MSC_VER)
}
