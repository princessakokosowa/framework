#include "basic.h"
#include "context.h"

#include "math.h"

f64 Math_square(f64 a)
{
    f64 result = a * a;

    return result;
}

static f64 Haversine_radiansFromDegrees(f64 degrees)
{
    f64 result = 0.01745329251994329577 * degrees;

    return result;
}

#define USIZE_MAX ~0ull

static f64 Haversine_calculateDistance(f64 longitude_0, f64 latitude_0, f64 longitude_1, f64 latitude_1, f64 earth_radius)
{
    f64 d_latitude  = Haversine_radiansFromDegrees(latitude_1 - latitude_0);
    f64 d_longitude = Haversine_radiansFromDegrees(longitude_1 - longitude_0);

    latitude_0 = Haversine_radiansFromDegrees(latitude_0);
    latitude_1 = Haversine_radiansFromDegrees(latitude_1);

    f64 a      = Math_square(sin(d_latitude / 2.0)) + cos(latitude_0) * cos(latitude_1) * Math_square(sin(d_longitude/2));
    f64 c      = 2.0 * asin(sqrt(a));
    f64 result = earth_radius * c;

    return result;
}

typedef struct {
    usize a;
    usize b;
    usize c;
    usize d;
} Series;

usize Series_rotateLeft(usize value, usize shift) {
    usize result = (value << shift) | (value >> (64 - shift));

    return result;
}

usize Series_getRandom(Series *series) {
    usize e = series->a - Series_rotateLeft(series->b, 27);

    series->a = series->b ^ Series_rotateLeft(series->c, 17);
    series->b = series->c + series->d;
    series->c = series->d + e;
    series->d = e         + series->a;

    return series->d;
}

// NOTE(casey): This is the seed pattern for JSF generators, as per the original post
Series Series_createAndApplySeed(usize value) {
    Series series = (Series) {
        .a = 0xf1ea5eed,
        .b = value,
        .c = value,
        .d = value,
    };

    isize count = 20;
    while (count -= 1) {
        Series_getRandom(&series);
    }

    return series;
}

f64 Series_generateRandomInRange(Series *series, f64 value_min, f64 value_max) {
    f64 t      = cast(f64, Series_getRandom(series)) / cast(f64, USIZE_MAX);
    f64 result = (1.0 - t) * value_min + t * value_max;
    
    return result;
}

static f64 Series_generateRandomDegree(Series *series, f64 center, f64 radius, f64 value_max_allowed)
{
    f64 value_min = center - radius;
    if (value_min < -value_max_allowed) {
        value_min = -value_max_allowed;
    }
    
    f64 value_max = center + radius;
    if (value_max > value_max_allowed) {
        value_max = value_max_allowed;
    }

    f64 result = Series_generateRandomInRange(series, value_min, value_max);

    return result;
}

#define EARTH_RADIUS 6372.8

#define X_MAX_ALLOWED 180.0
#define Y_MAX_ALLOWED 90.0
#define X_MIN_ALLOWED -X_MAX_ALLOWED
#define Y_MIN_ALLOWED -Y_MAX_ALLOWED

#define USE_CLUSTER                  false
#define COUNT_FOR_CLUSTER_TO_KICK_IN 0

void HaversineTests_test(void) {
    #if USE_CLUSTER == false
        // @NOTE
        // This means that we are very unlikely to ever reach 
        // `COUNT_FOR_CLUSTER_TO_KICK_IN`.
        usize count = USIZE_MAX;
    #else
        usize count = 0;
    #endif // USE_CLUSTER

    f64 x_center = 0.0;
    f64 y_center = 0.0;
    f64 x_radius = X_MAX_ALLOWED;
    f64 y_radius = Y_MAX_ALLOWED;

    usize  seed_value = 234089;
    Series series     = Series_createAndApplySeed(seed_value);

    usize pair_count_max = 1ull << 34;
    usize pair_count     = 100;

    if (pair_count < pair_count_max) {
        u64 cluster_count_max = 1 + (pair_count / 64);

        printf("{\"pairs\":[\n");

        f64 sum             = 0.0;
        f64 sum_coefficient = 1.0 / cast(f64, pair_count);
        forInRange(0, pair_count) {
            if (count == COUNT_FOR_CLUSTER_TO_KICK_IN) {
                count = cluster_count_max;

                x_center = Series_generateRandomInRange(&series, X_MIN_ALLOWED, X_MAX_ALLOWED);
                y_center = Series_generateRandomInRange(&series, Y_MIN_ALLOWED, Y_MAX_ALLOWED);
                x_radius = Series_generateRandomInRange(&series, 0            , X_MAX_ALLOWED);
                y_radius = Series_generateRandomInRange(&series, 0            , Y_MAX_ALLOWED);
            }

            count -= 1;

            f64 x_0 = Series_generateRandomDegree(&series, x_center, x_radius, X_MAX_ALLOWED);
            f64 y_0 = Series_generateRandomDegree(&series, y_center, y_radius, Y_MAX_ALLOWED);
            f64 x_1 = Series_generateRandomDegree(&series, x_center, x_radius, X_MAX_ALLOWED);
            f64 y_1 = Series_generateRandomDegree(&series, y_center, y_radius, Y_MAX_ALLOWED);

            f64 haversine_distance = Haversine_calculateDistance(x_0, y_0, x_1, y_1, EARTH_RADIUS);

            sum += sum_coefficient * haversine_distance;

            char const *separator = (index == (pair_count - 1))
                ? "\n"
                : ",\n";

            printf("    {\"x0\":%.16f, \"y0\":%.16f, \"x1\":%.16f, \"y1\":%.16f}%s", x_0, y_0, x_1, y_1, separator);
        }

        printf("]}\n");

        printf("Random seed: %llu\n", seed_value);
        printf("Pair count: %llu\n", pair_count);
        printf("Expected sum: %.16f\n", sum);

        printf("Cluster count max: %llu\n", cluster_count_max);
    }

    forInRange(0, 7) {
        isize i = index;
        forInRange(0, 6) {
            isize j = index;
            if (i + j != 7 - 1 + 6 - 1) printf("%i ", i * j);
            else                        printf("%i\n", i * j);
        }
    }
}
