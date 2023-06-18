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

#if BUILD_ROOT
    per_thread Thread thread;
#endif

core_function void Thread_create(void) {
    thread = (Thread) {
        .is_main_thread = true,
    };
}

core_function void Thread_destroy(void) {
    thread = (Thread) {
        0,
    };
}

core_function void Thread_mainEntryPoint(void (*entry)(void), usize argument_count, char **arguments) {
    Context_create();
    TemporaryStorage_create();
    Thread_create();

    entry();

    Thread_destroy();
    TemporaryStorage_destroy();
    Context_destroy();
}

#endif // THREAD_H
