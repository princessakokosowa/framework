////////////////////////////////
// Globals

#if BUILD_ROOT
    per_thread Thread thread = { 0, };
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

core_function void Thread_mainEntryPoint(void (*entry)(void), isize argument_count, char **arguments) {
    Thread_create();
    Context_create();
    TemporaryStorage_create();

    entry();

    TemporaryStorage_destroy();
    Context_destroy();
    Thread_destroy();
}
