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
    isize gb_snapped_size = size;

    gb_snapped_size += gigabytes(1) - 1;
    gb_snapped_size -= gb_snapped_size % gigabytes(1);

    void *ptr = VirtualAlloc(0, gb_snapped_size, MEM_RESERVE, PAGE_NOACCESS);

    return ptr;
}

core_function bool Os_release(void *ptr, isize size) {
    return cast(bool, VirtualFree(ptr, 0, MEM_RELEASE));
}

core_function void Os_commit(void *ptr, isize size) {
    isize page_snapped_size = size;

    page_snapped_size += Os_pageSize() - 1;
    page_snapped_size -= page_snapped_size % Os_pageSize();

    VirtualAlloc(ptr, page_snapped_size, MEM_COMMIT, PAGE_READWRITE);
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

            Map(PAGE_EXECUTE          ,                                              OS_ACCESS_FLAG_EXECUTE);
            Map(PAGE_EXECUTE_READ     , OS_ACCESS_FLAG_READ |                        OS_ACCESS_FLAG_EXECUTE);
            Map(PAGE_EXECUTE_READWRITE, OS_ACCESS_FLAG_READ | OS_ACCESS_FLAG_WRITE | OS_ACCESS_FLAG_EXECUTE);
            Map(PAGE_EXECUTE_WRITECOPY,                       OS_ACCESS_FLAG_WRITE | OS_ACCESS_FLAG_EXECUTE);
            Map(PAGE_READONLY         , OS_ACCESS_FLAG_READ                                                );
            Map(PAGE_READWRITE        , OS_ACCESS_FLAG_READ | OS_ACCESS_FLAG_WRITE                         );

            #undef Map
        }
    }

    // protect
    DWORD old_flags = 0;

    VirtualProtect(ptr, page_snapped_size, new_flags, &old_flags);
}
