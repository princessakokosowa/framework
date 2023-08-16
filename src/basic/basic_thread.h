#ifndef BASIC_THREAD_H
#define BASIC_THREAD_H

enum {
    Thread_ScratchCount = 2,
    Thread_NameCount    = 32,
};

typedef struct {
    Pool pools[Thread_ScratchCount];

    char  *file_name;
    isize line_number;
    u8    name[Thread_NameCount];

    bool is_main_thread;
} ThreadContext;

// core_global Thread thread;

core_function void ThreadContext_init(void);
core_function void ThreadContext_deinit(void);
core_function void ThreadContext_mainEntryPoint(void (*entry)(void), isize argument_count, char **arguments);

#endif // BASIC_THREAD_H
