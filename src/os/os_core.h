#ifndef OS_CORE_H
#define OS_CORE_H

////////////////////////////////
// Basics

typedef enum {
    OsAccessFlag_Read       = 1 << 0,
    OsAccessFlag_Write      = 1 << 1,
    OsAccessFlag_Exectue    = 1 << 2,
    OsAccessFlag_CreateNew = 1 << 3,
    OsAccessFlag_All        = 0x7fffffff,
} OsAccessFlag;

typedef isize OsAccessFlags;

////////////////////////////////
// @os_per_backend Memory

core_function isize Os_pageSize(void);
core_function void  *Os_reserve(isize size);
core_function bool  Os_release(void *ptr, isize size);
core_function void  Os_commit(void *ptr, isize size);
core_function void  Os_decommit(void *ptr, isize size);
core_function void  Os_setMemoryAccessFlags(void *ptr, isize size, OsAccessFlags flags);

#endif // OS_CORE_H
