#ifndef INCLUDE_ARENA_H
#define INCLUDE_ARENA_H

#include "foundation.h"
#include "allocator.h"

#define ARENA_DEFAULT_SIZE      65536
#define ARENA_DEFAULT_ALIGNMENT 8

typedef struct {
    isize alignment;

    bool should_overwrite;
} ArenaDescription;

typedef struct {
    isize alignment;

    isize size;
    isize occupied;
    isize last;
    u8    *ptr;

    bool should_overwrite;

    // @TODO
    // Do we need that?
    Allocator* backing_allocator;
} Arena;

void *Arena_allocatorProcedure(AllocatorMode mode, AllocatorDescription *description) {
    assert(description->impl != null);
    Arena *arena = cast(Arena*, description->impl);

    switch (mode) {
        case ALLOCATOR_MODE_FREE: {
            return cast(void*, cast(isize, true));
        } break;
        case ALLOCATOR_MODE_RESIZE: {
            assert(description->ptr_to_be_resized_or_freed != null);

            bool is_this_the_previous_allocation = arena->ptr + arena->last == cast(u8*, description->ptr_to_be_resized_or_freed);
            if (is_this_the_previous_allocation == true) {
                isize previous_allocation_size = arena->occupied - arena->last;
                isize allocation_size          = description->size_to_be_allocated_or_resized - previous_allocation_size;
                isize aligned_allocation_size  = align(allocation_size, ALLOCATOR_ALIGNMENT);

                assert(arena->occupied + aligned_allocation_size <= arena->size);

                arena->occupied += aligned_allocation_size;

                return description->ptr_to_be_resized_or_freed;
            }

            assert(arena->occupied + description->size_to_be_allocated_or_resized <= arena->size);

            isize aligned_allocation_size = align(description->size_to_be_allocated_or_resized, ALLOCATOR_ALIGNMENT);
            u8    *chunk                  = arena->ptr + arena->occupied;

            arena->occupied += aligned_allocation_size;

            for (isize i = 0; i < description->size_to_be_allocated_or_resized; i += 1) {
                chunk[i] = cast(u8*, description->ptr_to_be_resized_or_freed)[i];
            }

            return chunk;
        } break;
        case ALLOCATOR_MODE_ALLOCATE: {
            assert(arena->occupied + description->size_to_be_allocated_or_resized <= arena->size);

            if (arena->ptr == null) {
                bool are_we_already_set_in_context = context.allocator->impl == arena;

                if (are_we_already_set_in_context == true) arena->backing_allocator = &default_allocator;
                else                                       arena->backing_allocator = context.allocator;

                arena->ptr = arena->backing_allocator->procedure(ALLOCATOR_MODE_ALLOCATE, &(AllocatorDescription) {
                    .size_to_be_allocated_or_resized = arena->size,
                });
            }

            isize aligned_allocation_size = align(description->size_to_be_allocated_or_resized, ALLOCATOR_ALIGNMENT);
            u8    *chunk                  = arena->ptr + arena->occupied;

            arena->last     =  arena->occupied;
            arena->occupied += aligned_allocation_size;

            return chunk;
        } break;
    }

    unreachable();
}

Arena Arena_create(ArenaDescription *description) {
    // @SUCCINCTNESS
    // Here's a problem, we may have allocated arena with one allocator, but want to free
    // its memory with another. This is probably due to misuse of the framework's
    // features, but either way, we might want to be proof of such foolishness and
    // therefore introduce a mechanism to prevent users from doing such stupid things.
    //
    // Similar to how we set allocators and remember them in `Context`, we could also try
    // to remember allocators at the time this allocator (or any other thing?) is created.
    // That way, when we destroy an object, we can check that the allocator we are about
    // to destroy is actually the same allocator we used to allocate the object in the
    // first place.
    //
    // This may add some overhead to our program, but well, allocators are usually not in
    // the hot paths of programs (and even if they were, they are globally accessible
    // due to the architecture of this framework.
    //
    // We shall see.
    //     ~ princessakokosowa, 3rd of April 2023
    return (Arena) {
        .alignment        = ARENA_DEFAULT_ALIGNMENT,
        .size             = ARENA_DEFAULT_SIZE,
        .should_overwrite = false,
    };
}

void Arena_destroy(Arena *arena) {
    if (arena->ptr != null) {
        void *result_but_ptr = arena->backing_allocator->procedure(ALLOCATOR_MODE_FREE, &(AllocatorDescription) {
            .ptr_to_be_resized_or_freed = arena->ptr,
            .impl                       = arena->backing_allocator,
        });

        assert(result_but_ptr != null);
    }

    *arena = (Arena) {
        0,
    };
}

void *Arena_get(Arena *arena, isize type_size_times_count) {
    return Arena_allocatorProcedure(ALLOCATOR_MODE_ALLOCATE, &(AllocatorDescription) {
        .size_to_be_allocated_or_resized = type_size_times_count,
        .impl                            = cast(void*, arena),
    });
}


Allocator Arena_getAllocator(Arena *arena) {
    return (Allocator) {
        .procedure = Arena_allocatorProcedure,
        .impl      = cast(void*, arena),
    };
}

#endif // INCLUDE_ARENA_H
