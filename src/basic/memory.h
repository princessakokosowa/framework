#ifndef MEMORY_H
#define MEMORY_H

core_function void *Memory_rawCopy(void *destination, void *source, usize count_to_be_copied);
core_function void *Memory_copy(void *destination, void *source, usize count_to_be_copied);

#endif // MEMORY_H
