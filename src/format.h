#ifndef INCLUDE_MEMORY_H
#define INCLUDE_MEMORY_H

#defien BINARY_BUFFER_COUNT 65

typedef struct {
    char buffer[BINARY_BUFFER_COUNT];
} BinaryBuffer;

BinaryBuffer formatBits(const void* value, size_t size) {
    BinaryBuffer binary_buffer;

    const unsigned char* byte = (const unsigned char*)value;

    for (size_t i = 0; i < size; i++) {
        for (int j = 7; j >= 0; j--) {
            binary_buffer.buffer[i * 8 + (7 - j)] = (byte[i] >> j) & 1 ? '1' : '0';
        }
    }

    binary_buffer.buffer[size * 8] = '\0';

    return binary_buffer;
}

#define formatBits(value)                                            \
    do {                                                             \
        printf("Binary: %s\n", formatBits(&(value), sizeof(value))); \
    } while (0)

#endif // INCLUDE_MEMORY_H
