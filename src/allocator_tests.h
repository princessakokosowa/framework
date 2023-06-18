#ifndef INCLUDE_ALLOCATOR_TESTS_H
#define INCLUDE_ALLOCATOR_TESTS_H

#include "basic/basic.h"

#include "arena.h"

function isize cstringCount(u8 *c_string) {
    isize count = 0;
    while (*c_string) {
        c_string += 1;
        count    += 1;
    }

    return count;
}

function void cstringCopy(u8 *destination, u8 *source) {
    isize source_count = cstringCount(source);

    Memory_rawCopy(destination, source, source_count);

    destination[source_count] = '\0';
}

#define BUFFER_COUNT 64

function void AllocatorTests_test(void) {
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
                cstringCopy(buf, cast(u8 *, strings[i]));

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
                cstringCopy(buf, cast(u8 *, strings[i]));

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
                cstringCopy(buf, cast(u8 *, strings[i]));

                printf("%s\n", buf);
            }

            Context_remindAllocators();
        }

        Arena_destroy(&arena);
    }

    // Set `Context` allocators, similarly to how this is done in the previous scope,
    // but using predefined temporary allocator (and its temporary storage).
    {
        Context_setAllocators(&temporary_storage_allocator);

        isize buf_size  = sizeof(u8) * BUFFER_COUNT;
        u8    *buf      = alloc(buf_size);

        u8 strings[][BUFFER_COUNT] = {
            "Ghost of Mother lingering death",
            "Ghost on Mother's bed",
            "Black strands on the pillow contour of her health",
            "Twisted face upon the head",
        };

        for (isize i = 0; i < arrayCount(strings); i += 1) {
            cstringCopy(buf, cast(u8 *, strings[i]));

            printf("%s\n", buf);
        }

        TemporaryStorage_reset();
        Context_remindAllocators();
    }

    // Here goes the default allocator.
    {
        isize buf_size = sizeof(u8) * BUFFER_COUNT;
        u8    *buf     = alloc(buf_size);

        isize greater_buf_size = 2 * buf_size;
        buf = resize(buf, greater_buf_size);

        u8 strings[][BUFFER_COUNT] = {
            "Ghost of Mother lingering death",
            "Ghost on Mother's bed",
            "Black strands on the pillow contour of her health",
            "Twisted face upon the head",
        };

        for (isize i = 0; i < arrayCount(strings); i += 1) {
            cstringCopy(buf, cast(u8 *, strings[i]));

            printf("%s\n", buf);
        }

        free(buf);
    }
}

#endif // INCLUDE_ALLOCATOR_TESTS_H
