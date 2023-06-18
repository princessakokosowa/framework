#ifndef ARENA_H
#define ARENA_H

enum {
    ARENA_DEFAULT_SIZE      = 65536,
    ARENA_DEFAULT_ALIGNMENT = 8,
};

typedef struct {
    isize alignment;
    isize size;

    bool should_overwrite;

    Allocator *backing_allocator;
} ArenaDescription;

typedef struct {
    isize alignment;

    isize size;
    isize occupied;
    isize last;
    u8    *ptr;

    bool should_overwrite;

    Allocator *backing_allocator;
} Arena;

core_function void Arena_setAllocators(Arena *arena, Allocator *allocator);
core_function void *Arena_allocatorProcedure(AllocatorMode mode, AllocatorDescription *description);
core_function Arena Arena_create(ArenaDescription *description);
core_function void Arena_destroy(Arena *arena);
core_function void *Arena_get(Arena *arena, isize type_size_times_count);
core_function Allocator Arena_getAllocator(Arena *arena);

#endif // ARENA_H
