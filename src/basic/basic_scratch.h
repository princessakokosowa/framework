#ifndef BASIC_TEMPORARY_STORAGE_H
#define BASIC_TEMPORARY_STORAGE_H

enum {
    Scratch_OverflowPageCount = 32768,
};

typedef struct {
    u8 *ptr;
} OverflowPage;

enum {
    Scratch_Alignment = 8,
    Scratch_Size      = 32768,
};

typedef struct {
    Arena arena;
} Scratch;

core_global Scratch scratch;

core_function void      Scratch_init(void);
core_function void      Scratch_deinit(void);
core_function void      Scratch_reset(void);
core_function void      *Scratch_alloc(isize type_size_times_count);
core_function Allocator Scratch_getAllocator(void);

#endif // BASIC_TEMPORARY_STORAGE_H
