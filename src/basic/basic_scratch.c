////////////////////////////////
// Globals

#if BUILD_ROOT
    Scratch scratch = { 0, };
#endif

core_function void Scratch_init(void) {
    scratch = (Scratch) {
        .arena = Arena_init(&(ArenaDescription) {
            .alignment = Scratch_Alignment,
            .size      = Scratch_Size,
        }),
    };
}

core_function void Scratch_deinit(void) {
    Arena_deinit(&scratch.arena);

    scratch = (Scratch) {
        0,
    };
}

core_function void Scratch_reset(void) {
    scratch.arena.last     = 0;
    scratch.arena.occupied = 0;
}

core_function void *Scratch_alloc(isize type_size_times_count) {
    return Arena_alloc(&scratch.arena, type_size_times_count);
}

core_function Allocator Scratch_getAllocator(void) {
    return Arena_getAllocator(&scratch.arena);
}
