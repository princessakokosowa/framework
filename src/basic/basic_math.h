#ifndef BASIC_MATH_H
#define BASIC_MATH_H

////////////////////////////////
// Macros

// floating point operations

#define Floorf32(f)        floorf(f)
#define Ceilf32(f)         ceilf(f)
#define Roundf32(f)        roundf(f)
#define Modf32(x, y)       fmodf(x, y)
#define DegFromRadf32(v)   ((180.f/Pif32) * (v))
#define RadFromDegf32(v)   ((Pif32/180.f) * (v))
#define TurnsFromDegf32(v) ((v) / 360.f)
#define TurnsFromRadf32(v) ((v) / (2*Pif32))
#define DegFromTurnsf32(v) ((v) * 360.f)
#define RadFromTurnsf32(v) ((v) * (2*Pif32))
#define SquareRootf32(x)   sqrtf(x)
#define Sinf32(v)          sinf(RadFromTurnsf32(v))
#define Cosf32(v)          cosf(RadFromTurnsf32(v))
#define Tanf32(v)          tanf(RadFromTurnsf32(v))
#define Sin2f32(v)         Powf32(Sin(v), 2)
#define Cos2f32(v)         Powf32(Cos(v), 2)
#define Powf32(b, exp)     powf(b, exp)
#define Log10f32(v)        log10f(v)
#define LogEf32(v)         logf(v)

#define Floorf64(f)        floor(f)
#define Ceilf64(f)         ceil(f)
#define Roundf64(f)        round(f)
#define Modf64(x, y)       fmod(x, y)
#define DegFromRadf64(v)   ((180.0/Pif64) * (v))
#define RadFromDegf64(v)   ((Pif64/180.0) * (v))
#define TurnsFromDegf64(v) ((v) / 360.0)
#define TurnsFromRadf64(v) ((v) / (2*Pif64))
#define DegFromTurnsf64(v) ((v) * 360.0)
#define RadFromTurnsf64(v) ((v) * (2*Pif64))
#define SquareRootf64(x)   sqrt(x)
#define Sinf64(v)          sin(RadFromTurnsf64(v))
#define Cosf64(v)          cos(RadFromTurnsf64(v))
#define Tanf64(v)          tan(RadFromTurnsf64(v))
#define Sin2f64(v)         Powf64(Sin(v), 2)
#define Cos2f64(v)         Powf64(Cos(v), 2)
#define Powf64(b, exp)     pow(b, exp)
#define Log10f64(v)        log10(v)
#define LogEf64(v)         log(v)

// omitted base type aliases

#define Floor(f)           Floorf32(f)
#define Ceil(f)            Ceilf32(f)
#define Mod(x, y)          Modf32(x, y)
#define DegFromRad(v)      DegFromRadf32(v)
#define RadFromDeg(v)      RadFromDegf32(v)
#define TurnsFromDeg(v)    TurnsFromDegf32(v)
#define TurnsFromRad(v)    TurnsFromRadf32(v)
#define DegFromTurns(v)    ((v) * 360.0)
#define RadFromTurns(v)    ((v) * (2*Pif64))
#define SquareRoot(x)      SquareRootf32(x)
#define Sin(v)             Sinf32(v)
#define Cos(v)             Cosf32(v)
#define Tan(v)             Tanf32(v)
#define Sin2(v)            Sin2f32(v)
#define Cos2(v)            Cos2f32(v)
#define Pow(b, exp)        Powf32(b, exp)
#define Log10(v)           Log10f32(v)
#define LogE(v)            LogEf32(v)

////////////////////////////////
// Constants

read_only global f32 Infinityf32 = INFINITY;

read_only global f32 Pif32                       = 3.1415926535897f;
read_only global f32 OneOverSquareRootOfTwoPif32 = 0.3989422804f;
read_only global f32 EulersNumberf32             = 2.7182818284590452353602874713527f;

read_only global f64 Pif64                       = 3.1415926535897;
read_only global f64 OneOverSquareRootOfTwoPif64 = 0.3989422804;
read_only global f64 EulersNumberf64             = 2.7182818284590452353602874713527;

////////////////////////////////
// Vector types

// 2-vectors

typedef union Vector2f32 Vector2f32;
union Vector2f32 {
    struct {
        f32 x;
        f32 y;
    };

    f32 elements[2];
    f32 v[2];
};

typedef union Vector2f64 Vector2f64;
union Vector2f64 {
    struct {
        f64 x;
        f64 y;
    };

    f64 elements[2];
    f64 v[2];
};

typedef union Vector2i32 Vector2i32;
union Vector2i32 {
    struct {
        i32 x;
        i32 y;
    };

    i32 elements[2];
    i32 v[2];
};

typedef union Vector2i64 Vector2i64;
union Vector2i64 {
    struct {
        i64 x;
        i64 y;
    };

    i64 elements[2];
    i64 v[2];
};

// 3-vectors

typedef union Vector3f32 Vector3f32;
union Vector3f32 {
    struct {
        f32 x;
        f32 y;
        f32 z;
    };

    struct {
        f32 r;
        f32 g;
        f32 b;
    };

    struct {
        Vector2f32 xy;
        f32   _z1;
    };

    struct {
        f32   _x1;
        Vector2f32 yz;
    };

    f32 elements[3];
    f32 v[3];
};

typedef union Vector3f64 Vector3f64;
union Vector3f64 {
    struct {
        f64 x;
        f64 y;
        f64 z;
    };
    
    struct {
        f64 r;
        f64 g;
        f64 b;
    };

    struct {
        Vector2f64 xy;
        f64   _z1;
    };

    struct {
        f64   _x1;
        Vector2f64 yz;
    };

    f64 elements[3];
    f64 v[3];
};

typedef union Vector3i32 Vector3i32;
union Vector3i32 {
    struct {
        i32 x;
        i32 y;
        i32 z;
    };

    struct {
        i32 r;
        i32 g;
        i32 b;
    };

    i32 elements[3];
    i32 v[3];
};

typedef union Vector3i64 Vector3i64;
union Vector3i64 {
    struct {
        i64 x;
        i64 y;
        i64 z;
    };

    struct {
        i64 r;
        i64 g;
        i64 b;
    };

    i64 elements[3];
    i64 v[3];
};

// 4-vectors

typedef union Vector4f32 Vector4f32;
union Vector4f32 {
    struct {
        f32 x;
        f32 y;
        f32 z;
        f32 w;
    };

    struct {
        Vector2f32 xy;
        Vector2f32 zw;
    };

    struct {
        Vector3f32 xyz;
        f32   _w1;
    };

    struct {
        f32   _x1;
        Vector3f32 yzw;
    };

    struct {
        Vector3f32 rgb;
        f32   _a1;
    };

    struct {
        f32   _r1;
        Vector3f32 gba;
    };

    struct {
        f32 r;
        f32 g;
        f32 b;
        f32 a;
    };

    struct {
        f32 left;
        f32 up;
        f32 right;
        f32 down;
    };

    struct {
        f32 padding_[2];
        f32 dim[2];
    };

    f32 elements[4];
    f32 v[4];
};

typedef union Vector4f64 Vector4f64;
union Vector4f64 {
    struct {
        f64 x;
        f64 y;
        f64 z;
        f64 w;
    };

    struct {
        Vector2f64 xy;
        Vector2f64 zw;
    };

    struct {
        Vector3f64 xyz;
        f64   _w1;
    };

    struct {
        f64   _x1;
        Vector3f64 yzw;
    };

    struct {
        Vector3f64 rgb;
        f64   _a1;
    };

    struct {
        f64   _r1;
        Vector3f64 gba;
    };

    struct {
        f64 r;
        f64 g;
        f64 b;
        f64 a;
    };

    struct {
        f64 left;
        f64 up;
        f64 right;
        f64 down;
    };

    struct {
        f64 padding_[2];
        f64 dim[2];
    };

    f64 elements[4];
    f64 v[4];
};

typedef union Vector4i32 Vector4i32;
union Vector4i32 {
    struct {
        i32 x;
        i32 y;
        i32 z;
        i32 w;
    };

    struct {
        Vector2i32 xy;
        Vector2i32 zw;
    };

    struct {
        Vector3i32 xyz;
        i32   _w1;
    };

    struct {
        i32   _x1;
        Vector3i32 yzw;
    };

    struct {
        Vector3i32 rgb;
        i32   _a1;
    };

    struct {
        i32   _r1;
        Vector3i32 gba;
    };

    struct {
        i32 r;
        i32 g;
        i32 b;
        i32 a;
    };

    struct {
        i32 left;
        i32 up;
        i32 right;
        i32 down;
    };

    struct {
        i32 padding_[2];
        i32 dim[2];
    };

    i32 elements[4];
    i32 v[4];
};

typedef union Vector4i64 Vector4i64;
union Vector4i64 {
    struct {
        i64 x;
        i64 y;
        i64 z;
        i64 w;
    };

    struct {
        Vector2i64 xy;
        Vector2i64 zw;
    };

    struct {
        Vector3i64 xyz;
        i64   _w1;
    };

    struct {
        i64   _x1;
        Vector3i64 yzw;
    };

    struct {
        Vector3i64 rgb;
        i64   _a1;
    };

    struct {
        i64   _r1;
        Vector3i64 gba;
    };

    struct {
        i64 r;
        i64 g;
        i64 b;
        i64 a;
    };

    struct {
        i64 left;
        i64 up;
        i64 right;
        i64 down;
    };

    struct {
        i64 padding_[2];
        i64 dim[2];
    };

    i64 elements[4];
    i64 v[4];
};

#define make2f32FromVector(vec) make2f32((f32)(vec).x, (f32)(vec).y)
#define make2f64FromVector(vec) Make2f64((f64)(vec).x, (f64)(vec).y)
#define make2i32FromVector(vec) Make2i32((i32)(vec).x, (i32)(vec).y)
#define make2i64FromVector(vec) Make2i64((i64)(vec).x, (i64)(vec).y)

#define make3f32FromVector(vec) make3f32((f32)(vec).x, (f32)(vec).y, (f32)(vec).z)
#define make3f64FromVector(vec) Make3f64((f64)(vec).x, (f64)(vec).y, (f64)(vec).z)
#define make3i32FromVector(vec) Make3i32((i32)(vec).x, (i32)(vec).y, (i32)(vec).z)
#define make3i64FromVector(vec) Make3i64((i64)(vec).x, (i64)(vec).y, (i64)(vec).z)

#define make4f32FromVector(vec) make4f32((f32)(vec).x, (f32)(vec).y, (f32)(vec).z, (f32)(vec).w)
#define make4f64FromVector(vec) Make4f64((f64)(vec).x, (f64)(vec).y, (f64)(vec).z, (f64)(vec).w)
#define make4i32FromVector(vec) Make4i32((i32)(vec).x, (i32)(vec).y, (i32)(vec).z, (i32)(vec).w)
#define make4i64FromVector(vec) Make4i64((i64)(vec).x, (i64)(vec).y, (i64)(vec).z, (i64)(vec).w)

////////////////////////////////
// Matrix types

typedef struct Matrix33f32 Matrix33f32;
struct Matrix33f32 {
    f32 elements[3][3];
};

typedef struct Matrix33f64 Matrix33f64;
struct Matrix33f64 {
    f64 elements[3][3];
};

typedef struct Matrix44f32 Matrix44f32;
struct Matrix44f32 {
    f32 elements[4][4];
};

typedef struct Matrix44f64 Matrix44f64;
struct Matrix44f64 {
    f64 elements[4][4];
};

////////////////////////////////
// Quaternion Types

typedef union Quatf32 Quatf32;
union Quatf32 {
    Vector4f32 xyzw;

    struct {
        Vector3f32 xyz;
        f32     w;
    };

    struct {
        Vector2f32 xy;
        Vector2f32 zw;
    };

    struct {
        f32     x;
        Vector3f32 yzw;
    };

    struct {
        f32 _x;
        f32 y;
        f32 z;
        f32 _w;
    };

    f32 elements[4];
};

////////////////////////////////
// Interval Types

// 1D intervals

typedef union Rng1f32 Rng1f32;
union Rng1f32 {
    struct {
        f32 min;
        f32 max;
    };

    f32 v[2];
};

typedef union Rng1f64 Rng1f64;
union Rng1f64 {
    struct {
        f64 min;
        f64 max;
    };

    f64 v[2];
};

typedef union Rng1i32 Rng1i32;
union Rng1i32 {
    struct {
        i32 min;
        i32 max;
    };

    i32 v[2];
};

typedef union Rng1i64 Rng1i64;
union Rng1i64 {
    struct {
        i64 min;
        i64 max;
    };

    i64 v[2];
};

typedef union Rng1u32 Rng1u32;
union Rng1u32 {
    struct {
        u32 min;
        u32 max;
    };

    u32 v[2];
};

typedef union Rng1u64 Rng1u64;
union Rng1u64 {
    struct {
        u64 min;
        u64 max;
    };

    u64 v[2];
};

// 2D intervals

typedef union Rng2f32 Rng2f32;
union Rng2f32 {
    struct {
        Vector2f32 min;
        Vector2f32 max;
    };

    struct {
        Vector2f32 p0;
        Vector2f32 p1;
    };

    struct {
        f32 x0;
        f32 y0;
        f32 x1;
        f32 y1;
    };

    Vector2f32 v[2];
};

typedef union Rng2f64 Rng2f64;
union Rng2f64 {
    struct {
        Vector2f64 min;
        Vector2f64 max;
    };

    struct {
        Vector2f64 p0;
        Vector2f64 p1;
    };

    struct {
        f64 x0;
        f64 y0;
        f64 x1;
        f64 y1;
    };

    Vector2f64 v[2];
};

typedef union Rng2i32 Rng2i32;
union Rng2i32 {
    struct {
        Vector2i32 min;
        Vector2i32 max;
    };

    struct {
        Vector2i32 p0;
        Vector2i32 p1;
    };

    struct {
        i32 x0;
        i32 y0;
        i32 x1;
        i32 y1;
    };

    Vector2i32 v[2];
};

typedef union Rng2i64 Rng2i64;
union Rng2i64 {
    struct {
        Vector2i64 min;
        Vector2i64 max;
    };

    struct {
        Vector2i64 p0;
        Vector2i64 p1;
    };

    struct {
        i64 x0;
        i64 y0;
        i64 x1;
        i64 y1;
    };

    Vector2i64 v[2];
};

// 3D intervals

typedef union Rng3f32 Rng3f32;
union Rng3f32 {
    struct {
        Vector3f32 min;
        Vector3f32 max;
    };

    struct {
        Vector3f32 p0;
        Vector3f32 p1;
    };

    struct {
        f32 x0;
        f32 y0;
        f32 z0;
        f32 x1;
        f32 y1;
        f32 z1;
    };

    Vector3f32 v[2];
};

// 1D interval list types

typedef struct Rng1u64Node Rng1u64Node;
struct Rng1u64Node {
    Rng1u64Node *next;
    Rng1u64     v;
};

typedef struct Rng1u64List Rng1u64List;
struct Rng1u64List {
    Rng1u64Node *first;
    Rng1u64Node *last;

    u64 count;
    u64 total_count;
};

////////////////////////////////
// Scalar ops

core_function f32 Mix1f32(f32 a, f32 b, f32 t);

////////////////////////////////
// Vector ops

core_function Vector2f32 make2f32(f32 x, f32 y);
core_function Vector2f32 Add2f32(Vector2f32 a, Vector2f32 b);
core_function Vector2f32 Sub2f32(Vector2f32 a, Vector2f32 b);
core_function Vector2f32 Mul2f32(Vector2f32 a, Vector2f32 b);
core_function Vector2f32 Div2f32(Vector2f32 a, Vector2f32 b);
core_function Vector2f32 Scale2f32(Vector2f32 a, f32 scale);
core_function f32        Dot2f32(Vector2f32 a, Vector2f32 b);
core_function f32        LengthSquared2f32(Vector2f32 v);
core_function f32        Length2f32(Vector2f32 v);
core_function Vector2f32 Normalize2f32(Vector2f32 v);
core_function Vector2f32 Mix2f32(Vector2f32 a, Vector2f32 b, f32 t);

core_function Vector2f64 Make2f64(f64 x, f64 y);
core_function Vector2f64 Add2f64(Vector2f64 a, Vector2f64 b);
core_function Vector2f64 Sub2f64(Vector2f64 a, Vector2f64 b);
core_function Vector2f64 Mul2f64(Vector2f64 a, Vector2f64 b);
core_function Vector2f64 Div2f64(Vector2f64 a, Vector2f64 b);
core_function Vector2f64 Scale2f64(Vector2f64 a, f64 scale);
core_function f64        Dot2f64(Vector2f64 a, Vector2f64 b);
core_function f64        LengthSquared2f64(Vector2f64 v);
core_function f64        Length2f64(Vector2f64 v);
core_function Vector2f64 Normalize2f64(Vector2f64 v);
core_function Vector2f64 Mix2f64(Vector2f64 a, Vector2f64 b, f64 t);

core_function Vector2i32 Make2i32(i32 x, i32 y);
core_function Vector2i32 Add2i32(Vector2i32 a, Vector2i32 b);
core_function Vector2i32 Sub2i32(Vector2i32 a, Vector2i32 b);

core_function Vector2i64 Make2i64(i64 x, i64 y);
core_function Vector2i64 Add2i64(Vector2i64 a, Vector2i64 b);
core_function Vector2i64 Sub2i64(Vector2i64 a, Vector2i64 b);

core_function Vector3f32 make3f32(f32 x, f32 y, f32 z);
core_function Vector3f32 Add3f32(Vector3f32 a, Vector3f32 b);
core_function Vector3f32 Sub3f32(Vector3f32 a, Vector3f32 b);
core_function Vector3f32 Mul3f32(Vector3f32 a, Vector3f32 b);
core_function Vector3f32 Div3f32(Vector3f32 a, Vector3f32 b);
core_function Vector3f32 Scale3f32(Vector3f32 a, f32 scale);
core_function f32     Dot3f32(Vector3f32 a, Vector3f32 b);
core_function f32     LengthSquared3f32(Vector3f32 v);
core_function f32     Length3f32(Vector3f32 v);
core_function Vector3f32 Normalize3f32(Vector3f32 v);
core_function Vector3f32 Mix3f32(Vector3f32 a, Vector3f32 b, f32 t);
core_function Vector3f32 Cross3f32(Vector3f32 a, Vector3f32 b);
core_function Vector3f32 Transform3f32(Vector3f32 v, Matrix33f32 m);

core_function Vector3f64 Make3f64(f64 x, f64 y, f64 z);
core_function Vector3f64 Add3f64(Vector3f64 a, Vector3f64 b);
core_function Vector3f64 Sub3f64(Vector3f64 a, Vector3f64 b);
core_function Vector3f64 Mul3f64(Vector3f64 a, Vector3f64 b);
core_function Vector3f64 Div3f64(Vector3f64 a, Vector3f64 b);
core_function Vector3f64 Scale3f64(Vector3f64 a, f64 scale);
core_function f64        Dot3f64(Vector3f64 a, Vector3f64 b);
core_function f64        LengthSquared3f64(Vector3f64 v);
core_function f64        Length3f64(Vector3f64 v);
core_function Vector3f64 Normalize3f64(Vector3f64 v);
core_function Vector3f64 Mix3f64(Vector3f64 a, Vector3f64 b, f64 t);
core_function Vector3f64 Cross3f64(Vector3f64 a, Vector3f64 b);
core_function Vector3f64 Transform3f64(Vector3f64 v, Matrix33f64 m);

core_function Vector3i32 Make3i32(i32 x, i32 y, i32 z);

core_function Vector3i64 Make3i64(i64 x, i64 y, i64 z);

core_function Vector4f32 make4f32(f32 x, f32 y, f32 z, f32 w);
core_function Vector4f32 Add4f32(Vector4f32 a, Vector4f32 b);
core_function Vector4f32 Sub4f32(Vector4f32 a, Vector4f32 b);
core_function Vector4f32 Mul4f32(Vector4f32 a, Vector4f32 b);
core_function Vector4f32 Div4f32(Vector4f32 a, Vector4f32 b);
core_function Vector4f32 Scale4f32(Vector4f32 a, f32 scale);
core_function f32        Dot4f32(Vector4f32 a, Vector4f32 b);
core_function f32        LengthSquared4f32(Vector4f32 v);
core_function f32        Length4f32(Vector4f32 v);
core_function Vector4f32 Normalize4f32(Vector4f32 v);
core_function Vector4f32 Mix4f32(Vector4f32 a, Vector4f32 b, f32 t);
core_function Vector4f32 Transform4f32(Vector4f32 v, Matrix44f32 m);

core_function Vector4f64 Make4f64(f64 x, f64 y, f64 z, f64 w);
core_function Vector4f64 Add4f64(Vector4f64 a, Vector4f64 b);
core_function Vector4f64 Sub4f64(Vector4f64 a, Vector4f64 b);
core_function Vector4f64 Mul4f64(Vector4f64 a, Vector4f64 b);
core_function Vector4f64 Div4f64(Vector4f64 a, Vector4f64 b);
core_function Vector4f64 Scale4f64(Vector4f64 a, f64 scale);
core_function f64        Dot4f64(Vector4f64 a, Vector4f64 b);
core_function f64        LengthSquared4f64(Vector4f64 v);
core_function f64        Length4f64(Vector4f64 v);
core_function Vector4f64 Normalize4f64(Vector4f64 v);
core_function Vector4f64 Mix4f64(Vector4f64 a, Vector4f64 b, f64 t);
core_function Vector4f64 Transform4f64(Vector4f64 v, Matrix44f64 m);

core_function Vector4i32 Make4i32(i32 x, i32 y, i32 z, i32 w);

core_function Vector4i64 Make4i64(i64 x, i64 y, i64 z, i64 w);

////////////////////////////////
// Matrix constructors

core_function Matrix33f32 MakeMatrix33f32(f32 d);
core_function Matrix33f32 MakeTranslate33f32(Vector2f32 translation);
core_function Matrix33f32 MakeScale33f32(Vector2f32 scale);
core_function Matrix33f32 MakeRotate33f32(f32 turns);

core_function Matrix44f32 MakeMatrix44f32(f32 d);
core_function Matrix44f32 MakeTranslate44f32(Vector3f32 translation);
core_function Matrix44f32 MakeScale44f32(Vector3f32 scale);
core_function Matrix44f32 MakePerspective44f32(f32 fov, f32 aspect_ratio, f32 near_z, f32 far_z);
core_function Matrix44f32 MakeOrthographic44f32(f32 left, f32 right, f32 bottom, f32 top, f32 near_z, f32 far_z);
core_function Matrix44f32 MakeLookAt44f32(Vector3f32 eye, Vector3f32 center, Vector3f32 up);
core_function Matrix44f32 MakeRotate44f32(Vector3f32 axis, f32 turns);

////////////////////////////////
// Matrix ops

core_function Matrix33f32 Mul33f32(Matrix33f32 a, Matrix33f32 b);
core_function Matrix33f32 Scale33f32(Matrix33f32 m, f32 scale);

core_function Matrix44f32 Mul44f32(Matrix44f32 a, Matrix44f32 b);
core_function Matrix44f32 Scale44f32(Matrix44f32 m, f32 scale);
core_function Matrix44f32 Inverse44f32(Matrix44f32 m);
core_function Matrix44f32 RemoveRotation44f32(Matrix44f32 mat);

////////////////////////////////
// Quaternion constructors

core_function Quatf32 MakeQuatf32(f32 x, f32 y, f32 z, f32 w);
core_function Quatf32 QuatFromAxisAnglef32(Vector3f32 axis, f32 turns);

////////////////////////////////
// Quaternion ops

core_function Quatf32 AddQf32(Quatf32 a, Quatf32 b);
core_function Quatf32 SubQf32(Quatf32 a, Quatf32 b);
core_function Quatf32 MulQf32(Quatf32 a, Quatf32 b);
core_function Quatf32 ScaleQf32(Quatf32 a, f32 scale);
core_function Quatf32 NormalizeQf32(Quatf32 q);
core_function Quatf32 MixQf32(Quatf32 a, Quatf32 b, f32 t);
core_function f32     DotQf32(Quatf32 a, Quatf32 b);
core_function Matrix44f32 Mat44FromQuatf32(Quatf32 q);

////////////////////////////////
// Range ops

core_function Rng1f32 R1f32(f32 min, f32 max);
core_function Rng1f32 Shift1f32(Rng1f32 r, f32 v);
core_function Rng1f32 Pad1f32(Rng1f32 r, f32 x);
core_function f32     Center1f32(Rng1f32 r);
core_function bool    Contains1f32(Rng1f32 r, f32 v);
core_function f32     Dim1f32(Rng1f32 r);
core_function Rng1f32 Union1f32(Rng1f32 a, Rng1f32 b);
core_function Rng1f32 Intersection1f32(Rng1f32 a, Rng1f32 b);

core_function Rng1f64 R1f64(f64 min, f64 max);
core_function Rng1f64 Shift1f64(Rng1f64 r, f64 v);
core_function Rng1f64 Pad1f64(Rng1f64 r, f64 x);
core_function f64     Center1f64(Rng1f64 r);
core_function bool    Contains1f64(Rng1f64 r, f64 v);
core_function f64     Dim1f64(Rng1f64 r);
core_function Rng1f64 Union1f64(Rng1f64 a, Rng1f64 b);
core_function Rng1f64 Intersection1f64(Rng1f64 a, Rng1f64 b);

core_function Rng1i32 R1i32(i32 min, i32 max);
core_function Rng1i32 Shift1i32(Rng1i32 r, i32 v);
core_function Rng1i32 Pad1i32(Rng1i32 r, i32 x);
core_function i32     Center1i32(Rng1i32 r);
core_function bool    Contains1i32(Rng1i32 r, i32 v);
core_function i32     Dim1i32(Rng1i32 r);
core_function Rng1i32 Union1i32(Rng1i32 a, Rng1i32 b);
core_function Rng1i32 Intersection1i32(Rng1i32 a, Rng1i32 b);

core_function Rng1i64 R1i64(i64 min, i64 max);
core_function Rng1i64 Shift1i64(Rng1i64 r, i64 v);
core_function Rng1i64 Pad1i64(Rng1i64 r, i64 x);
core_function i64     Center1i64(Rng1i64 r);
core_function bool    Contains1i64(Rng1i64 r, i64 v);
core_function i64     Dim1i64(Rng1i64 r);
core_function Rng1i64 Union1i64(Rng1i64 a, Rng1i64 b);
core_function Rng1i64 Intersection1i64(Rng1i64 a, Rng1i64 b);

core_function Rng1u32 R1u32(u32 min, u32 max);
core_function Rng1u32 Shift1u32(Rng1u32 r, u32 v);
core_function Rng1u32 Pad1u32(Rng1u32 r, u32 x);
core_function u32     Center1u32(Rng1u32 r);
core_function bool    Contains1u32(Rng1u32 r, u32 v);
core_function u32     Dim1u32(Rng1u32 r);
core_function Rng1u32 Union1u32(Rng1u32 a, Rng1u32 b);
core_function Rng1u32 Intersection1u32(Rng1u32 a, Rng1u32 b);

core_function Rng1u64 R1u64(u64 min, u64 max);
core_function Rng1u64 Shift1u64(Rng1u64 r, u64 v);
core_function Rng1u64 Pad1u64(Rng1u64 r, u64 x);
core_function u64     Center1u64(Rng1u64 r);
core_function bool    Contains1u64(Rng1u64 r, u64 v);
core_function u64     Dim1u64(Rng1u64 r);
core_function Rng1u64 Union1u64(Rng1u64 a, Rng1u64 b);
core_function Rng1u64 Intersection1u64(Rng1u64 a, Rng1u64 b);

core_function Rng2f32    R2f32(Vector2f32 min, Vector2f32 max);
core_function Rng2f32    Shift2f32(Rng2f32 r, Vector2f32 v);
core_function Rng2f32    Pad2f32(Rng2f32 r, f32 x);
core_function Vector2f32 Center2f32(Rng2f32 r);
core_function bool       Contains2f32(Rng2f32 r, Vector2f32 v);
core_function Vector2f32 Dim2f32(Rng2f32 r);
core_function Rng2f32    Union2f32(Rng2f32 a, Rng2f32 b);
core_function Rng2f32    Intersection2f32(Rng2f32 a, Rng2f32 b);

core_function Rng2f64    R2f64(Vector2f64 min, Vector2f64 max);
core_function Rng2f64    Shift2f64(Rng2f64 r, Vector2f64 v);
core_function Rng2f64    Pad2f64(Rng2f64 r, f64 x);
core_function Vector2f64 Center2f64(Rng2f64 r);
core_function bool       Contains2f64(Rng2f64 r, Vector2f64 v);
core_function Vector2f64 Dim2f64(Rng2f64 r);
core_function Rng2f64    Union2f64(Rng2f64 a, Rng2f64 b);
core_function Rng2f64    Intersection2f64(Rng2f64 a, Rng2f64 b);

core_function Rng2i32    R2i32(Vector2i32 min, Vector2i32 max);
core_function Rng2i32    Shift2i32(Rng2f32 r, Vector2i32 v);
core_function Rng2i32    Pad2i32(Rng2i32 r, i32 x);
core_function Vector2i32 Center2i32(Rng2i32 r);
core_function bool       Contains2i32(Rng2i32 r, Vector2i32 v);
core_function Vector2i32 Dim2i32(Rng2i32 r);
core_function Rng2i32    Union2i32(Rng2i32 a, Rng2i32 b);
core_function Rng2i32    Intersection2i32(Rng2i32 a, Rng2i32 b);

core_function Rng2i64    R2i64(Vector2i64 min, Vector2i64 max);
core_function Rng2i64    Shift2i64(Rng2f32 r, Vector2i64 v);
core_function Rng2i64    Pad2i64(Rng2i64 r, i64 x);
core_function Vector2i64 Center2i64(Rng2i64 r);
core_function bool       Contains2i64(Rng2i64 r, Vector2i64 v);
core_function Vector2i64 Dim2i64(Rng2i64 r);
core_function Rng2i64    Union2i64(Rng2i64 a, Rng2i64 b);
core_function Rng2i64    Intersection2i64(Rng2i64 a, Rng2i64 b);

core_function Rng3f32    R3f32(Vector3f32 min, Vector3f32 max);
core_function Rng3f32    Shift3f32(Rng3f32 r, Vector3f32 v);
core_function Rng3f32    Pad3f32(Rng3f32 r, f32 x);
core_function Vector3f32 Center3f32(Rng3f32 r);
core_function bool       Contains3f32(Rng3f32 r, Vector3f32 v);
core_function Vector3f32 Dim3f32(Rng3f32 r);
core_function Rng3f32    Union3f32(Rng3f32 a, Rng3f32 b);
core_function Rng3f32    Intersection3f32(Rng3f32 a, Rng3f32 b);

////////////////////////////////
// Lists

core_function void Rng1u64ListPushNode(Rng1u64List *list, Rng1u64Node *node);
core_function void Rng1u64ListPushNodeFront(Rng1u64List *list, Rng1u64Node *node);
core_function void Rng1u64ListPush(Arena *arena, Rng1u64List *list, Rng1u64 rng);
core_function void Rng1u64ListPushFront(Arena *arena, Rng1u64List *list, Rng1u64 rng);

////////////////////////////////
// Miscellaneous ops

core_function Vector3f32 HSVFromRGB(Vector3f32 rgb);
core_function Vector3f32 RGBFromHSV(Vector3f32 hsv);
core_function Vector4f32 Vector4f32FromHexRGBA(u32 hex);
core_function f32        MillisecondsFromMicroseconds(u64 microseconds);
core_function u64        MicrosecondsFromMilliseconds(f32 milliseconds);
core_function Vector2i64 SideVertexFromCorner(Corner corner);

////////////////////////////////
// Omitted Base Type Wrappers (Assume f32)

#if !BASIC_DISABLE_OMITTED_BASE_TYPES

// types
typedef Vector2f32 Vector2;
typedef Vector3f32 Vector3;
typedef Vector4f32 Vector4;
typedef Rng1f32 Rng1;
typedef Rng2f32 Rng2;
typedef Rng3f32 Rng3;
typedef Matrix33f32 Matrix33;
typedef Matrix44f32 Matrix44;

// constructors
#define make2(x, y)       make2f32((x), (y))
#define make3(x, y, z)    make3f32((x), (y), (z))
#define make4(x, y, z, w) make4f32((x), (y), (z), (w))

#define make2FromVector(v) make2f32FromVector(v)
#define make3FromVector(v) make3f32FromVector(v)
#define make4FromVector(v) make4f32FromVector(v)

#define range1(p0, p1) R1f32((p0), (p1))
#define range2(p0, p1) R2f32((p0), (p1))
#define range3(p0, p1) R3f32((p0), (p1))

#define make33(d)                MakeMatrix33f32(d)
#define translate33(translation) MakeTranslate33f32(translation)
#define scale33(scale)           MakeScale33f32(scale)
#define rotate33(angle)          MakeRotate33f32(angle)

#define make44(d)                                               MakeMatrix44f32(d)
#define translate44(translation)                                MakeTranslate44f32(translation)
#define scale44(scale)                                          MakeScale44f32(scale)
#define perspective44(fov, aspect_ratio, near_z, far_z)         MakePerspective44f32((fov), (aspect_ratio), (near_z), (far_z))
#define orthographic44(left, right, bottom, top, near_z, far_z) MakeOrthographic44f32((left), (right), (bottom), (top), (near_z), (far_z))
#define lookAt44(eye, center, up)                               MakeLookAt44f32((eye), (center), (up))

// scalar ops
#define mix1(a, b, t) Mix1f32((a), (b), (t))

// 2d vector ops
#define add2(a, b)        Add2f32((a), (b))
#define sub2(a, b)        Sub2f32((a), (b))
#define mul2(a, b)        Mul2f32((a), (b))
#define div2(a, b)        Div2f32((a), (b))
#define scale2(v, scale)  Scale2f32((v), (scale))
#define dot2(a, b)        Dot2f32((a), (b))
#define lengthSquared2(v) LengthSquared2f32(v)
#define length2(v)        Length2f32(v)
#define normalize2(v)     Normalize2f32(v)
#define mix2(a, b, t)     Mix2f32((a), (b), (t))

// 3d vector ops
#define add3(a, b)            Add3f32((a), (b))
#define sub3(a, b)            Sub3f32((a), (b))
#define mul3(a, b)            Mul3f32((a), (b))
#define div3(a, b)            Div3f32((a), (b))
#define scale3(v, scale)      Scale3f32((v), (scale))
#define dot3(a, b)            Dot3f32((a), (b))
#define lengthSquared3(v)     LengthSquared3f32(v)
#define length3(v)            Length3f32(v)
#define normalize3(v)         Normalize3f32(v)
#define mix3(a, b, t)         Mix3f32((a), (b), (t))
#define cross3(a, b)          Cross3f32((a), (b))
#define transform3(v, mat3x3) Transform3f32((v), (mat3x3))

// 4d vector ops
#define add4(a, b)            Add4f32((a), (b))
#define sub4(a, b)            Sub4f32((a), (b))
#define mul4(a, b)            Mul4f32((a), (b))
#define div4(a, b)            Div4f32((a), (b))
#define scale4(v, scale)      Scale4f32((v), (scale))
#define dot4(a, b)            Dot4f32((a), (b))
#define lengthSquared4(v)     LengthSquared4f32(v)
#define length4(v)            Length4f32(v)
#define normalize4(v)         Normalize4f32(v)
#define mix4(a, b, t)         Mix4f32((a), (b), (t))
#define transform4(v, mat4x4) Transform4f32((v), (mat4x4))

// r1 ops
#define shift1(rng, v)      Shift1f32((rng), (v))
#define pad1(rng, v)        Pad1f32((rng), (v))
#define center1(rng)        Center1f32(rng)
#define contains1(rng, v)   Contains1f32((rng), (v))
#define dim1(rng)           Dim1f32(rng)
#define union1(a, b)        Union1f32((a), (b))
#define intersection1(a, b) Intersection1f32((a), (b))

// r2 ops
#define shift2(rng, v)      Shift2f32((rng), (v))
#define pad2(rng, v)        Pad2f32((rng), (v))
#define center2(rng)        Center2f32(rng)
#define contains2(rng, v)   Contains2f32((rng), (v))
#define dim2(rng)           Dim2f32(rng)
#define union2(a, b)        Union2f32((a), (b))
#define intersection2(a, b) Intersection2f32((a), (b))

// r3 ops
#define shift3(rng, v)      Shift3f32((rng), (v))
#define pad3(rng, v)        Pad3f32((rng), (v))
#define center3(rng)        Center3f32(rng)
#define contains3(rng, v)   Contains3f32((rng), (v))
#define dim3(rng)           Dim3f32(rng)
#define union3(a, b)        Union3f32((a), (b))
#define intersection3(a, b) Intersection3f32((a), (b))

// 3x3 ops
#define mul33(a, b)        Mul33f32((a), (b))
#define scale33(m, scalar) Scale33f32((m), (scalar))

// 4x4 ops
#define mul44(a, b)         Mul44f32((a), (b))
#define scale44(m, scalar)  Scale44f32((m), (scalar))
#define inverse44(m)        Inverse44f32(m)
#define removeRotation44(m) RemoveRotation44f32(m)

#endif // !BASIC_DISABLE_OMITTED_BASE_TYPES

#endif // BASIC_MATH_H
