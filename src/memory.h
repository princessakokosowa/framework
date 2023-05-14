#ifndef INCLUDE_MEMORY_H
#define INCLUDE_MEMORY_H

void *Memory_copy(void *dest, const void *src, size_t n) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    size_t i;

    // Copy blocks of sizeof(unsigned long) bytes
    for (i = 0; i < n / sizeof(unsigned long); i++) {
        *((unsigned long *)d) = *((unsigned long *)s);
        d += sizeof(unsigned long);
        s += sizeof(unsigned long);
    }

    // Copy remaining bytes individually
    for (i = 0; i < n % sizeof(unsigned long); i++) {
        *d++ = *s++;
    }

    return dest;
}

bool Memory_isEqual(const void* ptr1, const void* ptr2, size_t size) {
    const unsigned char* bytePtr1 = (const unsigned char*)ptr1;
    const unsigned char* bytePtr2 = (const unsigned char*)ptr2;

    // Compare 64 bits (8 bytes) at a time
    const size_t chunkSize = sizeof(unsigned long long);
    const size_t numChunks = size / chunkSize;

    // Compare the memory blocks in 64-bit chunks
    for (size_t i = 0; i < numChunks; i++) {
        const unsigned long long* chunkPtr1 = (const unsigned long long*)(bytePtr1 + i * chunkSize);
        const unsigned long long* chunkPtr2 = (const unsigned long long*)(bytePtr2 + i * chunkSize);

        if (*chunkPtr1 != *chunkPtr2) {
            // Memory blocks are not equal
            return false;
        }
    }

    // Compare the remaining bytes (if any) byte by byte
    const size_t remainingBytes = size % chunkSize;
    for (size_t i = size - remainingBytes; i < size; i++) {
        if (bytePtr1[i] != bytePtr2[i]) {
            return false;
        }
    }

    return true;
}

void *Memory_move(void *dest, const void *src, size_t n) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    if (d == s)
        return d;

    if (s + n <= d || d + n <= s) {
        // Non-overlapping memory blocks, use optimized memcpy
        unsigned long *dest_aligned = (unsigned long *)dest;
        const unsigned long *src_aligned = (const unsigned long *)src;

        while (n >= sizeof(unsigned long)) {
            *dest_aligned++ = *src_aligned++;
            n -= sizeof(unsigned long);
        }

        d = (unsigned char *)dest_aligned;
        s = (const unsigned char *)src_aligned;
    }

    if (d < s) {
        // Copy from start to end
        while (n > 0) {
            *d++ = *s++;
            n--;
        }
    } else {
        // Copy from end to start
        d += n - 1;
        s += n - 1;
        while (n > 0) {
            *d-- = *s--;
            n--;
        }
    }

    return dest;
}

#endif // INCLUDE_MEMORY_H
