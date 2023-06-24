#ifndef BASIC_STRING_H
#define BASIC_STRING_H

enum {
    ARENA_DEFAULT_SIZE      = 65536,
    ARENA_DEFAULT_ALIGNMENT = 8,
};

typedef struct {
    u8    *str;
    isize size;
} String;

#endif // BASIC_STRING_H
