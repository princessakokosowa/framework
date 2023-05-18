#ifndef INCLUDE_ARRAY_TESTS_H
#define INCLUDE_ARRAY_TESTS_H

#include "basic.h"
#include "allocator.h"
#include "context.h"

#include "memory.h"
#include "arena.h"

#include "array.h"

void ArrayTests_test(void) {
    {
        f32 *array = null;
        Array_add(array, 334);
        Array_add(array, 1);
        Array_add(array, -1);
        Array_add(array, 0);

        Array_free(array);
    }

    {
        Context_setAllocators(&temporary_allocator);

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

            Array_for(array) {
                if (index != Array_count(array) - 1) printf("%5.1f ",  array[index]);
                else                                 printf("%5.1f\n", array[index]);
            }

            Array_forEach(array) {
                if (iterator != Array_last(array) - 1) printf("%5.1f ",  *iterator);
                else                                   printf("%5.1f\n", *iterator);
            }
        }

        {
            f32 popped_value = Array_pop(array);
            printf("Popped one item, its value is %5.1f!\n", popped_value);

            Array_addAt(array, 666.f, 44);
            printf("Inserted one item at 44, its value is %5.1f!\n", array[44]);

            Array_for(array) {
                if (index != Array_count(array) - 1) printf("%5.1f ", array[index]);
                else                                 printf("%5.1f\n", array[index]);
            }
        }


        {
            Array_removeByValue(array, 0.f);
            Array_removeAtIndex(array, 0);
            printf("Removed 0.f by value and a value at 0th index, but in a unordered way and therefore, the order has not been perserved!\n");

            Array_for(array) {
                if (index != Array_count(array) - 1) printf("%5.1f ", array[index]);
                else                                 printf("%5.1f\n", array[index]);
            }
        }

        {
            Array_removeByValueOrdered(array, 1.f);
            Array_removeAtIndexOrdered(array, 33);
            printf("Removed 1.f by value and a value at 33rd index, but this time in an ordered way!\n");

            Array_for(array) {
                if (index != Array_count(array) - 1) printf("%5.1f ", array[index]);
                else                                 printf("%5.1f\n", array[index]);
            }
        }

        {
            Array_resize(array, 96);
            printf("Tried to resize the array so that it has 96 items, but nothing happened (and that's expected!), the count and the capacity still are respectively %lli and %lli\n", Array_count(array), Array_capacity(array));

            Array_resize(array, 261);
            printf("Tried to resize the array so that it has 224 items and that's alright, the count and the capacity are respectively %lli and %lli\n", Array_count(array), Array_capacity(array));

            Array_for(array) {
                if (index != Array_count(array) - 1) printf("%5.1f ", array[index]);
                else                                 printf("%5.1f\n", array[index]);
            }
        }

        {
            {
                isize counter = 0;
                Array_for(array) if (array[index] == 0.f) counter += 1;
                printf("There are %lli 0.fs in this array. If removed, there will be %lli\n", counter, Array_count(array) - counter);
            }

            Array_removeAllByValue(array, 0.f);
            printf("Removed all 0.f by value, the count is %lli\n", Array_count(array));

            Array_for(array) {
                if (index != Array_count(array) - 1) printf("%5.1f ", array[index]);
                else                                 printf("%5.1f\n", array[index]);
            }

            {
                isize counter = 0;
                Array_for(array) if (array[index] == 1.f) counter += 1;
                printf("There are %lli 1.fs in this array. If removed, there will be %lli\n", counter, Array_count(array) - counter);
            }

            Array_removeAllByValueOrdered(array, 1.f);
            printf("Removed all 1.f by value, the count is %lli\n", Array_count(array));

            Array_for(array) {
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

        Array_free(array);
        Context_remindAllocators();
        Arena_destroy(&arena);
    }
}

#endif // INCLUDE_ARRAY_TESTS_H
