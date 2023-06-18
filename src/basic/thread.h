#ifndef THREAD_H
#define THREAD_H

enum {
    THREAD_NAME_COUNT = 32,
};

typedef struct {
    char  *file_name;
    isize line_number;
    u8    name[THREAD_NAME_COUNT];

    bool is_main_thread;
} Thread;

core_function void Thread_create(void);
core_function void Thread_destroy(void);
core_function void Thread_mainEntryPoint(void (*entry)(void), usize argument_count, char **arguments);

#endif // THREAD_H
