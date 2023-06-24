////////////////////////////////
// Globals

#if BUILD_ROOT
    TemporaryStorage temporary_storage = { 0, };
#endif

core_function void TemporaryStorage_create(void) {
    temporary_storage = (TemporaryStorage) {
        .arena = Arena_create(&(ArenaDescription) {
            .alignment = TEMPORARY_STORAGE_DEFAULT_ALIGNMENT,
            .size      = TEMPORARY_STORAGE_DEFAULT_SIZE,
        }),
    };
}

core_function void TemporaryStorage_destroy(void) {
    Arena_destroy(&temporary_storage.arena);

    temporary_storage = (TemporaryStorage) {
        0,
    };
}

core_function void TemporaryStorage_reset(void) {
    temporary_storage.arena.last     = 0;
    temporary_storage.arena.occupied = 0;
}

core_function void *TemporaryStorage_get(isize type_size_times_count) {
    return Arena_get(&temporary_storage.arena, type_size_times_count);
}

core_function Allocator TemporaryStorage_getAllocator(void) {
    return Arena_getAllocator(&temporary_storage.arena);
}
