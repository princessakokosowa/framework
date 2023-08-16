#ifndef BASIC_STRING_H
#define BASIC_STRING_H

enum {
    String_Size      = 65536,
    String_Alignment = 8,
};

typedef struct {
    u8    *str;
    isize size;
} String;

#endif // BASIC_STRING_H
