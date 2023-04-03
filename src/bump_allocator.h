#ifndef INCLUDE_ARENA_ALLOCATOR_H
#define INCLUDE_ARENA_ALLOCATOR_H

#include "foundation.h"
#include "allocator.h"

#define AREA_STORAGE_COUNT 128

typedef struct {
    isize count;
    void* ptr_to_heap;
} ArenaDescription;

typedef struct {
    bool  has_user_provided_heap;
    isize count;
    isize occupied;
    isize last;

    u8* ptr_to_heap;
} Arena;

void* arenaAllocatorProcedure(AllocatorMode mode, AllocatorDescription *description) {
    assert(description->ptr_to_heap != null);

    Arena *arena = cast(Arena*, *(cast(isize*, description->ptr_to_heap) - 1));

    if (mode == ALLOCATOR_MODE_ALLOCATE) {
        assert(arena->occupied + description->size_to_be_allocated_or_resized <= arena->count);

        isize const aligned_allocation_size = allocatorAlign(description->size_to_be_allocated_or_resized, ALLOCATOR_ALIGNMENT);
        u8          *chunk                  = cast(u8*, description->ptr_to_heap) + arena->occupied;

        arena->last     =  arena->occupied;
        arena->occupied += aligned_allocation_size;

        return cast(void*, chunk);
    } else if (mode == ALLOCATOR_MODE_RESIZE) {
        assert(description->ptr_to_be_resized_or_freed != null);

        bool const is_this_the_previous_allocation = cast(u8*, description->ptr_to_heap) + arena->last == cast(u8*, description->ptr_to_be_resized_or_freed);

        if (is_this_the_previous_allocation == true) {
            isize const previous_allocation_size = arena->occupied - arena->last;
            isize const allocation_size          = description->size_to_be_allocated_or_resized - previous_allocation_size;
            isize const aligned_allocation_size  = allocatorAlign(allocation_size, ALLOCATOR_ALIGNMENT);

            assert(arena->occupied + aligned_allocation_size <= AREA_STORAGE_COUNT);

            arena->occupied += aligned_allocation_size;
        
            return description->ptr_to_be_resized_or_freed;
        }

        assert(arena->occupied + description->size_to_be_allocated_or_resized <= AREA_STORAGE_COUNT);

        isize const aligned_allocation_size = allocatorAlign(description->size_to_be_allocated_or_resized, ALLOCATOR_ALIGNMENT);
        u8          *chunk                  = cast(u8*, description->ptr_to_heap) + arena->occupied;

        arena->occupied += aligned_allocation_size;

        for (isize i = 0; i < description->size_to_be_allocated_or_resized; i += 1) {
            chunk[i] = cast(u8*, description->ptr_to_be_resized_or_freed)[i];
        }

        return cast(void*, chunk);

        return null;
    } else {
        return cast(void*, cast(i64, true));
    }

    unreachable();
}

Arena arenaCreate(ArenaDescription *description) {
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
    // the hot paths of our program (and even if they were, they are globally accessible
    // due to the architecture of this framework.
    //
    // We shall see.
    //     ~ princessakokosowa, 3rd of April 2023
    u8   *ptr_to_heap           = null;
    bool has_user_provided_heap = description->ptr_to_heap != null;
    if (!has_user_provided_heap) ptr_to_heap = cast(u8*, alloc(AREA_STORAGE_COUNT + sizeof(isize)));
    else                         ptr_to_heap = description->ptr_to_heap;
    
    return (Arena) {
        .has_user_provided_heap = has_user_provided_heap,
        .count                  = has_user_provided_heap ? description->count : AREA_STORAGE_COUNT,
        .ptr_to_heap            = ptr_to_heap,
    };
}

void arenaDestroy(Arena *arena) {
    if (!arena->has_user_provided_heap) free(arena->ptr_to_heap);

    *arena = (Arena) {
        0,
    };
}

Allocator arenaGetAllocator(Arena *arena) {
    // @SUCCINCTNESS
    // Here's another problem, because of the fact that we want to have allocators that
    // are used-defined, and because of the fact that we need to access the internal
    // state of an allocator, we need to establish the way in which these two things
    // interact with each other, somehow. Here's the problem, because the allocator
    // procedure is defined as follows.
    //
    // typedef void* (AllocatorProcedure)(AllocatorMode mode, AllocatorDescription *description);
    //
    // (So let it be, it's really obstruction-free, see allocator.h for more details)
    //
    // There's no sensible way to pass this metadata without carrying a pointer to the
    // state of the allocator along with the allocator itself. This might have looked
    // like this.
    //
    // typedef struct {
    //     AllocatorProcedure *procedure;
    //     void               *ptr_to_heap;
    //     
    //     void               *ptr_to_impl;
    //     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    //  // Here's the newly-inserted line of code.
    // } Allocator;
    //
    // (Also refer to allocator.h for more details)
    //
    // But that's not the point, if it was supposed to look like that, we could have made
    // the implementation explicit in the first place, and we wouldn't need this
    // `AllocatorProcedure` thing.
    //
    // There are a few ways to deal with this, quite a few to be fair, and the one that
    // gets the job done is to simply allocate the state of the allocator on the stack
    // (in the case of this framework, allocators and their states are only valid within
    // a stack frame, unless otherwise stated) and query the state of the allocator for
    // the pointer to its heap. The state would then return its allocator, where the
    // pointer to its implementation would be at the very beginning of the pointer to the
    // heap.
    //
    // It's similar to the way _stb_ things are implemented, I suppose. Although I don't
    // like this gimmicky way of doing things, I suppose this is actually the best way to
    // expose allocators to a user.
    //     ~ princessakokosowa, 3rd of April 2023
    *cast(isize*, arena->ptr_to_heap) = cast(isize, arena);
    
    return (Allocator) {
        .ptr_to_heap = arena->ptr_to_heap + sizeof(isize),
        .procedure   = arenaAllocatorProcedure,
    };
}

#endif // INCLUDE_ARENA_ALLOCATOR_H
