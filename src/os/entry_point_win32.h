function void entryPoint(void);

#if BUILD_CONSOLE_OUTPUT
    int main(int argument_count, char **arguments) {
        h_instance = GetModuleHandle(0);

        Thread_mainEntryPoint(entryPoint, cast(u64, argument_count), arguments);

        return 0;
    }
#else
    int WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR lp_cmd_line, int n_show_cmd) {
        h_instance = instance;

        Thread_mainEntryPoint(entryPoint, cast(u64, __argc), __argv);

        return 0;
    }
#endif
