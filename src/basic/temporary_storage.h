#ifndef TEMPORARY_STORAGE_H
#define TEMPORARY_STORAGE_H

enum {
    OVERFLOW_PAGE_COUNT = 32768,
};

typedef struct {
    u8 *ptr;
} OverflowPage;

enum {
    TEMPORARY_STORAGE_DEFAULT_SIZE      = 32768,
    TEMPORARY_STORAGE_DEFAULT_ALIGNMENT = 8,
};

typedef struct {
    isize alignment;

    isize size;
    isize occupied;
    isize last;
    u8    *ptr;

    Allocator *backing_allocator;
} TemporaryStorage;

core_function void *TemporaryStorage_allocatorProcedure(AllocatorMode mode, AllocatorDescription *description);
core_function void TemporaryStorage_create(void);
core_function void TemporaryStorage_destroy(void);
core_function void TemporaryStorage_reset(void);

#endif // TEMPORARY_STORAGE_H
