////////////////////////////////
// Globals

#if BUILD_ROOT
    per_thread ThreadContext t_thread_context = { 0, };
#endif

core_function ThreadContext ThreadContext_init(void) {
    Pool pools[Thread_ScratchCount] = { 0, };

    return (ThreadContext) {
        .is_main_thread = true,
    };
}

core_function void ThreadContext_deinit(void) {
    t_thread_context = (ThreadContext) {
        0,
    };
}

core_function void ThreadContext_mainEntryPoint(void (*entry)(void), isize argument_count, char **arguments) {
    Context_init();

    Scratch_init();
    ThreadContext_init();

    entry();

    ThreadContext_deinit();
    Scratch_deinit();

    Context_release();
}
