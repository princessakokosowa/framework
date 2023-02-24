typedef _Bool bool;

#define null  0
#define true  1
#define false 0

#include <stdint.h>

typedef char      i8;
typedef short     i16;
typedef int       i32;
typedef long long i64;

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

typedef unsigned short f16;
typedef float          f32;
typedef double         f64;

typedef size_t    usize;
typedef ptrdiff_t isize;

#define arraySize(value)  ((sizeof((value))) / (sizeof((value[0]))))
#define enumToInt(value)  ((isize)(value))
#define cast(Type, value) ((Type)(value))

#define assert(expression) (void)((!!(expression)) || (_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (usize)(__LINE__)), 0))
#define staticAssert(expression)  _Static_assert((expression), "Static assert failed!")
