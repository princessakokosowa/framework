function isize cstringCount(u8 *c_string) {
    isize count = 0;
    while (*c_string) {
        c_string += 1;
        count    += 1;
    }

    return count;
}

function void cstringCopy(u8 *destination, u8 *source) {
    isize source_count = cstringCount(source);

    Memory_rawCopy(destination, source, source_count);

    destination[source_count] = '\0';
}

function void AllocatorTests_test(void) {
    // Create some sort of memory pool (here: `Arena`).
    {
        Arena arena = Arena_create(&(ArenaDescription) { 0, });

        // Get some memory directly from it via `arenaGet`.
        {
            isize buf_size  = sizeof(u8) * BUFFER_COUNT;
            u8    *buf      = Arena_get(&arena, buf_size);

            u8 strings[][BUFFER_COUNT] = {
                "Hi,",
                "my",
                "name",
                "is,",
                "what?",
            };

            for (isize i = 0; i < arrayCount(strings); i += 1) {
                cstringCopy(buf, cast(u8 *, strings[i]));

                printf("%s\n", buf);
            }
        }

        // Get an allocator out of it and use it to get some memory.
        {
            Allocator arena_allocator = Arena_getAllocator(&arena);

            isize buf_size  = sizeof(u8) * BUFFER_COUNT;
            u8    *buf      = arena_allocator.procedure(ALLOCATOR_MODE_ALLOCATE, &(AllocatorDescription) {
                .size_to_be_allocated_or_resized = buf_size,
                .impl                            = cast(void*, &arena),
            });

            u8 strings[][BUFFER_COUNT] = {
                "Hi,",
                "my",
                "name",
                "is,",
                "who?",
            };

            for (isize i = 0; i < arrayCount(strings); i += 1) {
                cstringCopy(buf, cast(u8 *, strings[i]));

                printf("%s\n", buf);
            }
        }

        // Set `Context` allocators and the default `alloc`-, `resize`- and `free`-based
        // interface.
        {
            Allocator arena_allocator = Arena_getAllocator(&arena);
            Context_setAllocators(&arena_allocator);

            isize buf_size  = sizeof(u8) * BUFFER_COUNT;
            u8    *buf      = alloc(buf_size);

            u8 strings[][BUFFER_COUNT] = {
                "Hi,",
                "my",
                "name",
                "is,",
                "chka-chka, Slim Shady",
            };

            for (isize i = 0; i < arrayCount(strings); i += 1) {
                cstringCopy(buf, cast(u8 *, strings[i]));

                printf("%s\n", buf);
            }

            Context_remindAllocators();
        }

        Arena_destroy(&arena);
    }

    // Create some sort of memory pool (here: `Arena`).
    {
        Pool pool = Pool_create(&(PoolDescription) { 0, });

        // Get some memory directly from it via `arenaGet`.
        {
            isize buf_size  = sizeof(u8) * BUFFER_COUNT;
            u8    *buf      = Pool_get(&pool, buf_size);

            u8 strings[][BUFFER_COUNT] = {
                "Hi,",
                "my",
                "name",
                "is,",
                "what?",
            };

            for (isize i = 0; i < arrayCount(strings); i += 1) {
                cstringCopy(buf, cast(u8 *, strings[i]));

                printf("%s\n", buf);
            }
        }

        // Get an allocator out of it and use it to get some memory.
        {
            Allocator pool_allocator = Pool_getAllocator(&pool);

            isize buf_size  = sizeof(u8) * BUFFER_COUNT;
            u8    *buf      = pool_allocator.procedure(ALLOCATOR_MODE_ALLOCATE, &(AllocatorDescription) {
                .size_to_be_allocated_or_resized = buf_size,
                .impl                            = cast(void*, &pool),
            });

            u8 strings[][BUFFER_COUNT] = {
                "Hi,",
                "my",
                "name",
                "is,",
                "who?",
            };

            for (isize i = 0; i < arrayCount(strings); i += 1) {
                cstringCopy(buf, cast(u8 *, strings[i]));

                printf("%s\n", buf);
            }
        }

        // Set `Context` allocators and the default `alloc`-, `resize`- and `free`-based
        // interface.
        {
            Allocator pool_allocator = Pool_getAllocator(&pool);
            Context_setAllocators(&pool_allocator);

            isize buf_size  = sizeof(u8) * BUFFER_COUNT;
            u8    *buf      = alloc(buf_size);

            u8 strings[][BUFFER_COUNT] = {
                "Hi,",
                "my",
                "name",
                "is,",
                "chka-chka, Slim Shady",
            };

            for (isize i = 0; i < arrayCount(strings); i += 1) {
                cstringCopy(buf, cast(u8 *, strings[i]));

                printf("%s\n", buf);
            }

            Context_remindAllocators();
        }

        Pool_destroy(&pool);
    }

    // Set `Context` allocators, similarly to how this is done in the previous scope,
    // but using predefined temporary allocator (and its temporary storage).
    {
        Context_setAllocators(&temporary_storage_allocator);

        isize buf_size  = sizeof(u8) * BUFFER_COUNT;
        u8    *buf      = alloc(buf_size);

        u8 strings[][BUFFER_COUNT] = {
            "Ghost of Mother lingering death",
            "Ghost on Mother's bed",
            "Black strands on the pillow contour of her health",
            "Twisted face upon the head",
        };

        for (isize i = 0; i < arrayCount(strings); i += 1) {
            cstringCopy(buf, cast(u8 *, strings[i]));

            printf("%s\n", buf);
        }

        TemporaryStorage_reset();
        Context_remindAllocators();
    }

    // Here goes the default allocator.
    {
        isize buf_size = sizeof(u8) * BUFFER_COUNT;
        u8    *buf     = alloc(buf_size);

        isize greater_buf_size = 2 * buf_size;
        buf = resize(buf, greater_buf_size);

        u8 strings[][BUFFER_COUNT] = {
            "Ghost of Mother lingering death",
            "Ghost on Mother's bed",
            "Black strands on the pillow contour of her health",
            "Twisted face upon the head",
        };

        for (isize i = 0; i < arrayCount(strings); i += 1) {
            cstringCopy(buf, cast(u8 *, strings[i]));

            printf("%s\n", buf);
        }

        free(buf);
    }
}

function void ArrayTests_test(void) {
    {
        f32 *array = null;
        Array_add(array, 334);
        Array_add(array, 1);
        Array_add(array, -1);
        Array_add(array, 0);

        Array_free(array);
    }

    {
        Context_setAllocators(&temporary_storage_allocator);

        f32 *array = null;
        Array_add(array, 334);
        Array_add(array, 1);
        Array_add(array, -1);
        Array_add(array, 0);

        Array_free(array);
        TemporaryStorage_reset();
        Context_remindAllocators();
    }

    {
        Arena     arena           = Arena_create(&(ArenaDescription) { 0, });
        Allocator arena_allocator = Arena_getAllocator(&arena);

        Context_setAllocators(&arena_allocator);

        f32 *array = null;
        Array_add(array, 334.f);
        Array_add(array, 1.f);
        Array_add(array, -1.f);
        Array_add(array, 0.f);
        Array_add(array, 1337.f);
        Array_add(array, 0xfade);
        Array_add(array, 0xdeaf);
        Array_add(array, 0xcafe);
        Array_add(array, 0xbabe);
        Array_add(array, 0.f);

        {
            for (f32 i = 0.f; i < 128.f; i += 1.f) {
                if      (cast(isize, i) % 2) Array_add(array, 0.f);
                else if (cast(isize, i) % 3) Array_add(array, 1.f);
                else                         Array_add(array, i);
            }

            printf("Filled in the array with values!\n");

            // That is for (isize index = 0; index < Array_count((array)); index += 1),
            // but done _there for you_.
            forEachByIndex(array) {
                if (index != Array_count(array) - 1) printf("%5.1f ",  array[index]);
                else                                 printf("%5.1f\n", array[index]);
            }

            // Iterators?
            forEachByValue(array) {
                // Here, we are comparing ADDRESSES of the currently iterated item and
                // the last item in the array.
                if (iterator != Array_last(array)) printf("%5.1f ",  *iterator);
                else                               printf("%5.1f\n", *iterator);
            }

            // That is probably not the most interesting consequence of iterating things
            // that way, but quite indeed it produces an interesting result.
            /*
                forEachByValue(array) {
                // Here, we are comparing VALUES of the currently iterated item and
                // the last item in the array.
                if (*iterator != *Array_last(array)) printf("%5.1f ",  *iterator);
                else                                 printf("%5.1f\n", *iterator);
                }
                */

            // Obviously, something similar can be achieved the following way.
            /*
                forEachByIndex(array) {
                __typeof(array) iterator = Array_first(array) + index;

                if (iterator != Array_last(array)) printf("%5.1f ",  *iterator);
                else                               printf("%5.1f\n", *iterator);
                }
                */
        }

        {
            f32 popped_value = Array_pop(array);
            printf("Popped one item, its value is %5.1f!\n", popped_value);

            Array_addAt(array, 666.f, 44);
            printf("Inserted one item at 44, its value is %5.1f!\n", array[44]);

            forEachByIndex(array) {
                if (index != Array_count(array) - 1) printf("%5.1f ", array[index]);
                else                                 printf("%5.1f\n", array[index]);
            }
        }


        {
            Array_removeByValue(array, 0.f);
            Array_removeAtIndex(array, 1);
            printf("Removed 0.f by value and a value at 0th index, but in a unordered way and therefore, the order has not been perserved!\n");

            forEachByIndex(array) {
                if (index != Array_count(array) - 1) printf("%5.1f ", array[index]);
                else                                 printf("%5.1f\n", array[index]);
            }
        }

        {
            Array_removeByValueOrdered(array, 1.f);
            Array_removeAtIndexOrdered(array, 33);
            printf("Removed 1.f by value and a value at 33rd index, but this time in an ordered way!\n");

            forEachByIndex(array) {
                if (index != Array_count(array) - 1) printf("%5.1f ", array[index]);
                else                                 printf("%5.1f\n", array[index]);
            }
        }

        {
            Array_resize(array, 96);
            printf("Tried to resize the array so that it has 96 items, but nothing happened (and that's expected!), the count and the capacity still are respectively %lli and %lli\n", Array_count(array), Array_capacity(array));

            Array_resize(array, 261);
            printf("Tried to resize the array so that it has 224 items and that's alright, the count and the capacity are respectively %lli and %lli\n", Array_count(array), Array_capacity(array));

            forEachByIndex(array) {
                if (index != Array_count(array) - 1) printf("%5.1f ", array[index]);
                else                                 printf("%5.1f\n", array[index]);
            }
        }

        {
            {
                isize counter = 0;
                forEachByIndex(array) if (array[index] == 0.f) counter += 1;
                printf("There are %lli 0.fs in this array. If removed, there will be %lli\n", counter, Array_count(array) - counter);
            }

            Array_removeAllByValue(array, 0.f);
            printf("Removed all 0.f by value, the count is %lli\n", Array_count(array));

            forEachByIndex(array) {
                if (index != Array_count(array) - 1) printf("%5.1f ", array[index]);
                else                                 printf("%5.1f\n", array[index]);
            }

            {
                isize counter = 0;
                forEachByIndex(array) if (array[index] == 1.f) counter += 1;
                printf("There are %lli 1.fs in this array. If removed, there will be %lli\n", counter, Array_count(array) - counter);
            }

            Array_removeAllByValueOrdered(array, 1.f);
            printf("Removed all 1.f by value, the count is %lli\n", Array_count(array));

            forEachByIndex(array) {
                if (index != Array_count(array) - 1) printf("%5.1f ", array[index]);
                else                                 printf("%5.1f\n", array[index]);
            }
        }

        Array_free(array);
        Context_remindAllocators();
        Arena_destroy(&arena);
    }

    {
        Arena     arena           = Arena_create(&(ArenaDescription) { 0, });
        Allocator arena_allocator = Arena_getAllocator(&arena);

        Context_setAllocators(&arena_allocator);

        f32 *array = null;
        Array_reserve(array, 384);

        forInRange(0, 5) {
            Array_add(array, cast(f32, index ^ index * 2 << 2));
        }

        forEach(array) {
            printf("%i %1.1f\n", index, *value);
        }

        Array_free(array);
        Context_remindAllocators();
        Arena_destroy(&arena);
    }

    {
        Arena     arena           = Arena_create(&(ArenaDescription) { 0, });
        Allocator arena_allocator = Arena_getAllocator(&arena);

        f32 *array = null;

        Array_setAllocators(array, &arena_allocator);
        Array_reserve(array, 384);

        forInRange(0, 5) {
            Array_add(array, cast(f32, index ^ index * 2 << 2));
        }

        forEach(array) {
            printf("%i %1.1f\n", index, *value);
        }

        printf("count               : %lld\n", Array_count(array));
        printf("capacity            : %lld\n", Array_capacity(array));
        printf("allocator.procedure : 0x%p\n", Array_allocator(array)->procedure);
        printf("allocator.impl      : 0x%p\n", Array_allocator(array)->impl);

        Array_free(array);
        Arena_destroy(&arena);
    }
}

function f64 Math_square(f64 a) {
    f64 result = a * a;

    return result;
}

function f64 Haversine_radiansFromDegrees(f64 degrees) {
    f64 result = 0.01745329251994329577 * degrees;

    return result;
}

function f64 Haversine_calculateDistance(f64 longitude_0, f64 latitude_0, f64 longitude_1, f64 latitude_1, f64 earth_radius) {
    f64 d_latitude  = Haversine_radiansFromDegrees(latitude_1 - latitude_0);
    f64 d_longitude = Haversine_radiansFromDegrees(longitude_1 - longitude_0);

    latitude_0 = Haversine_radiansFromDegrees(latitude_0);
    latitude_1 = Haversine_radiansFromDegrees(latitude_1);

    f64 a      = Math_square(sin(d_latitude / 2.0)) + cos(latitude_0) * cos(latitude_1) * Math_square(sin(d_longitude/2));
    f64 c      = 2.0 * asin(sqrt(a));
    f64 result = earth_radius * c;

    return result;
}

function usize Series_rotateLeft(usize value, usize shift) {
    usize result = (value << shift) | (value >> (64 - shift));

    return result;
}

function usize Series_getRandom(Series *series) {
    usize e = series->a - Series_rotateLeft(series->b, 27);

    series->a = series->b ^ Series_rotateLeft(series->c, 17);
    series->b = series->c + series->d;
    series->c = series->d + e;
    series->d = e         + series->a;

    return series->d;
}

function Series Series_createAndApplySeed(usize value) {
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

function f64 Series_generateRandomInRange(Series *series, f64 value_min, f64 value_max) {
    f64 t      = cast(f64, Series_getRandom(series)) / cast(f64, USIZE_MAX);
    f64 result = (1.0 - t) * value_min + t * value_max;
    
    return result;
}

function f64 Series_generateRandomDegree(Series *series, f64 center, f64 radius, f64 value_max_allowed) {
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

function void HaversineTests_test(void) {
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
