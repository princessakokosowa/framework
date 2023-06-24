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

core_function void Os_entryPoint(void (*entry)(void), isize argument_count, char **arguments) {
    Context_create();
    TemporaryStorage_create();
    Thread_create();

    entry();

    Thread_destroy();
    TemporaryStorage_destroy();
    Context_destroy();
}
