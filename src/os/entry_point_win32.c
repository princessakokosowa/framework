function void entryPoint(void);

#if BUILD_CONSOLE_OUTPUT
    int main(int argument_count, char **arguments) {
        os_win32_instance = GetModuleHandle(0);

        Os_entryPoint(entryPoint, cast(u64, argument_count), arguments);

        return 0;
    }
#else
    int WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR lp_cmd_line, int n_show_cmd) {
        os_win32_instance = instance;

        Os_entryPoint(entryPoint, cast(u64, __argc), __argv);

        return 0;
    }
#endif
