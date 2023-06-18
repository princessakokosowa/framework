#ifndef TESTS_H
#define TESTS_H

function isize cstringCount(u8 *c_string);
function void cstringCopy(u8 *destination, u8 *source);

#define BUFFER_COUNT 64

function void AllocatorTests_test(void);
function void ArrayTests_test(void);

function f64 Math_square(f64 a);
function f64 Haversine_radiansFromDegrees(f64 degrees);

#define USIZE_MAX ~0ull

function f64 Haversine_calculateDistance(f64 longitude_0, f64 latitude_0, f64 longitude_1, f64 latitude_1, f64 earth_radius);

typedef struct {
    usize a;
    usize b;
    usize c;
    usize d;
} Series;

function usize Series_rotateLeft(usize value, usize shift);
function usize Series_getRandom(Series *series);
function Series Series_createAndApplySeed(usize value);
function f64 Series_generateRandomInRange(Series *series, f64 value_min, f64 value_max);
function f64 Series_generateRandomDegree(Series *series, f64 center, f64 radius, f64 value_max_allowed);

#define EARTH_RADIUS 6372.8

#define X_MAX_ALLOWED 180.0
#define Y_MAX_ALLOWED 90.0
#define X_MIN_ALLOWED -X_MAX_ALLOWED
#define Y_MIN_ALLOWED -Y_MAX_ALLOWED

#define USE_CLUSTER                  false
#define COUNT_FOR_CLUSTER_TO_KICK_IN 0

function void HaversineTests_test(void);

#endif // TESTS_H
