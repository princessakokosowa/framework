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
    default_allocator = (Allocator) {
        .procedure = &Default_allocatorProcedure,
    };

    Context_create();
    TemporaryStorage_create();

    Thread_create();

    entry();

    Thread_destroy();
    TemporaryStorage_destroy();
    Context_destroy();
}
