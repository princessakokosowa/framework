#ifndef BASIC_THREAD_H
#define BASIC_THREAD_H

enum {
    THREAD_NAME_COUNT = 32,
};

typedef struct {
    char  *file_name;
    isize line_number;
    u8    name[THREAD_NAME_COUNT];

    bool is_main_thread;
} Thread;

// core_global Thread thread;

core_function void Thread_create(void);
core_function void Thread_destroy(void);
core_function void Thread_mainEntryPoint(void (*entry)(void), isize argument_count, char **arguments);

#endif // BASIC_THREAD_H
