function void entryPoint(void);

#if BUILD_CONSOLE_OUTPUT
    int main(int argument_count, char **arguments) {
        Context_create();

        h_instance = GetModuleHandle(0);

        Thread_mainEntryPoint(entryPoint, cast(u64, argument_count), arguments);

        return Context_destroy();
    }
#else
    int WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR lp_cmd_line, int n_show_cmd) {
        Context_create();

        h_instance = instance;

        Thread_mainEntryPoint(entryPoint, cast(u64, __argc), __argv);

        return Context_destroy();
    }
#endif
