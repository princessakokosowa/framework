////////////////////////////////
// Globals

#if BUILD_ROOT
    HINSTANCE os_win32_instance = NULL;
#endif

////////////////////////////////
// Memory

core_function isize Os_pageSize(void) {
    SYSTEM_INFO info;

    GetSystemInfo(&info);

    return info.dwPageSize;
}

core_function void *Os_reserve(isize size) {
    isize aligned_size = align(size, gigabytes(1));

    void *ptr = VirtualAlloc(0, aligned_size, MEM_RESERVE, PAGE_NOACCESS);

    return ptr;
}

core_function bool Os_release(void *ptr, isize size) {
    return cast(bool, VirtualFree(ptr, 0, MEM_RELEASE));
}

core_function void Os_commit(void *ptr, isize size) {
    isize aligned_size = align(size, Os_pageSize());

    VirtualAlloc(ptr, aligned_size, MEM_COMMIT, PAGE_READWRITE);
}

core_function void Os_decommit(void *ptr, isize size) {
    VirtualFree(ptr, size, MEM_DECOMMIT);
}

core_function void Os_setMemoryAccessFlags(void *ptr, isize size, OsAccessFlags flags) {
    // size -> page snapped size
    isize page_snapped_size = size;

    page_snapped_size += Os_pageSize() - 1;
    page_snapped_size -= page_snapped_size % Os_pageSize();
 
    // flags -> win32 flags
    DWORD new_flags = 0;
    {
        switch(flags) {
            default: { new_flags = PAGE_NOACCESS; } break;

            #define Map(win32_code, bitflags) case bitflags: { new_flags = win32_code; } break

            Map(PAGE_EXECUTE          ,                                          OsAccessFlag_Exectue);
            Map(PAGE_EXECUTE_READ     , OsAccessFlag_Read |                      OsAccessFlag_Exectue);
            Map(PAGE_EXECUTE_READWRITE, OsAccessFlag_Read | OsAccessFlag_Write | OsAccessFlag_Exectue);
            Map(PAGE_EXECUTE_WRITECOPY,                     OsAccessFlag_Write | OsAccessFlag_Exectue);
            Map(PAGE_READONLY         , OsAccessFlag_Read                                            );
            Map(PAGE_READWRITE        , OsAccessFlag_Read | OsAccessFlag_Write                       );

            #undef Map
        }
    }

    // protect
    DWORD old_flags = 0;

    VirtualProtect(ptr, page_snapped_size, new_flags, &old_flags);
}
