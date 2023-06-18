#ifndef INCLUDE_FORMAT_H
#define INCLUDE_FORMAT_H

enum {
    BINARY_BUFFER_COUNT = 65,
};

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

// Function pointer type for custom data type printers
typedef void (*CustomPrinter)(void* data);

// Struct to store registered custom data type printers
typedef struct {
    char specifier;
    CustomPrinter printer;
} PrinterRegistry;

// Array to store registered custom data type printers
PrinterRegistry registry[10];
int registry_count = 0;

// Function to register a custom data type printer
void register_printer(char specifier, CustomPrinter printer) {
    PrinterRegistry entry;
    entry.specifier = specifier;
    entry.printer = printer;
    
    registry[registry_count++] = entry;
}

// Function to pretty print data using format placeholders
void pretty_print(const char* format, ...) {
    va_list list;
    va_start(list, format);

    // u8 *print_buffer = allocUsingAllocator(256 * sizeof(u8), &temporary_allocator);

    while (*format != '\0') {
        if (*format == '{') {
            format += 1;
            if (*format == '}') {
                format += 1;
                continue;
            }

            // Check if it's a basic data type specifier
            switch (*format) {
                case 'i': {
                    int value = *(int*)va_arg(list, int*);
                    printf("%d", value);
                } break;
                case 'f': {
                    float value = *(float*)va_arg(list, float*);
                    printf("%f", value);
                } break;
                case 'd': {
                    double value = *(double*)va_arg(list, double*);
                    printf("%lf", value);
                } break;
                case 's': {
                    short value = *(short*)va_arg(list, short*);
                    printf("%hd", value);
                } break;
                case 'c': {
                    char value = *(char*)va_arg(list, char*);
                    printf("%c", value);
                } break;
                case 'p': {
                    const char* value = *(const char**)va_arg(list, const char**);
                    printf("%s", value);
                } break;
                default: {
                    // Check if it's a registered custom data type specifier
                    for (int i = 0; i < registry_count; i++) {
                        if (registry[i].specifier == *format) {
                            registry[i].printer(va_arg(list, void*));
                            break;
                        }
                    }
                } break;
            }
        } else {
            putchar(*format);
        }

        format++;
    }

    TemporaryStorage_reset();

    va_end(list);
}

// Custom data type printer
typedef struct {
    char name[20];
    int age;
} Cat;

void print_cat(void* data) {
    Cat* cat = (Cat*)data;
    printf("Cat: %s, Age: %d", cat->name, cat->age);
}

void PrinterTests_test(void) {
    register_printer('c', print_cat);

    // Test the pretty_print function
    int num = 42;
    float pi = 3.14159;
    double e = 2.71828;
    short s = -10;
    char ch = 'A';
    const char* str = "Hello, World!";
    Cat cat = {
        .name = "Tom",
        .age = 5,
    };

    pretty_print("Number: {i}, Float: {f}, Double: {d}, Short: {s}, Char: {c}, String: {p}, Cat: {c}\n", &num, &pi, &e, &s, &ch, &str, &cat);

    TemporaryStorage_reset();
}

#define formatBits(value)                                            \
    do {                                                             \
        printf("Binary: %s\n", formatBits(&(value), sizeof(value))); \
    } while (0)

#endif // INCLUDE_FORMAT_H
