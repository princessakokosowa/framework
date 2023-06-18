#ifndef INCLUDE_MEMORY_H
#define INCLUDE_MEMORY_H

core_function
void *Memory_rawCopy(void *destination, void *source, usize count_to_be_copied) {
    // Cache coherency.
    u8 *d = cast(u8 *, destination);
    u8 *s = cast(u8 *, source);

    // Copy blocks of size of `u32` bytes.
    //
    // @NOTE
    // Couldn't we try copying chunks of size of `u64`?
    //     ~ princessakokosowa, 18 June 2023
    for (usize index = 0; index < count_to_be_copied / sizeof(u32); index += 1) {
        *cast(u32 *,d) = *cast(u32 *, s);

        d += sizeof(u32);
        s += sizeof(u32);
    }

    // Copy remaining bytes individually.
    for (usize index = 0; index < count_to_be_copied % sizeof(u32); index += 1) {
        *d = *s;

        d += 1;
        s += 1;
    }

    return destination;
}

core_function
void *Memory_copy(void *destination, void *source, usize count_to_be_copied) {
    // Cache coherency.
    u8 *d = cast(u8 *, destination);
    u8 *s = cast(u8 *, source);

    // Nothing to copy.
    if (d == s)
        return d;

    // Non-overlapping memory blocks, simply copy the memory.
    if (s + count_to_be_copied <= d || d + count_to_be_copied <= s) {
        Memory_rawCopy(destination, source, count_to_be_copied);
    }

    if (d < s) {
        // Copy from start to end.
        while (count_to_be_copied > 0) {
            *d = *s;

            d += 1;
            s += 1;

            count_to_be_copied -= 1;
        }
    } else {
        // Copy from end to start.
        d += count_to_be_copied - 1;
        s += count_to_be_copied - 1;
        while (count_to_be_copied > 0) {
            *d = *s;

            d -= 1;
            s -= 1;

            count_to_be_copied -= 1;
        }
    }

    return destination;
}

#endif // INCLUDE_MEMORY_H
