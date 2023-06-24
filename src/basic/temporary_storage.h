#ifndef BASIC_TEMPORARY_STORAGE_H
#define BASIC_TEMPORARY_STORAGE_H

enum {
    OVERFLOW_PAGE_COUNT = 32768,
};

typedef struct {
    u8 *ptr;
} OverflowPage;

enum {
    TEMPORARY_STORAGE_DEFAULT_ALIGNMENT = 8,
    TEMPORARY_STORAGE_DEFAULT_SIZE      = 32768,
};

typedef struct {
    Arena arena;
} TemporaryStorage;

core_global TemporaryStorage temporary_storage;

core_function void TemporaryStorage_create(void);
core_function void TemporaryStorage_destroy(void);
core_function void TemporaryStorage_reset(void);
core_function void *TemporaryStorage_get(isize type_size_times_count);
core_function Allocator TemporaryStorage_getAllocator(void);

#endif // BASIC_TEMPORARY_STORAGE_H
