#ifndef THREAD_H
#define THREAD_H

typedef struct {
    char *file_name;

    isize line_number;

    u8    thread_name[32];
    isize thread_name_size;

    bool is_main_thread;
} Thread;

core_function
Thread Thread_createMain(void) {
    return (Thread) {
        .is_main_thread = true,
    };
}

/*
core_function
Thread Thread_create(void) {
    return (Thread) {
        0,
    };
}
*/

core_function void
Thread_destroy(Thread *thread) {
    // ...
}

#if BUILD_ROOT
    per_thread Thread *main_thread = null;

    no_name_mangle
    void Thread_set(Thread *thread) {
        main_thread = thread;
    }

    no_name_mangle
    Thread *Thread_get(void) {
        return main_thread;
    }
#endif

/*
core_function
bool Thread_isMain(void) {
    Thread *thread = Thread_get();

    return thread->is_main_thread;
}
*/

core_function
void Thread_mainEntryPoint(void (*entry)(void), usize argument_count, char **arguments) {
    Thread thread = Thread_createMain();

    Thread_set(&thread);

    entry();

    Thread_destroy(&thread);
}

#endif // THREAD_H
