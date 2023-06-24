#ifndef OS_CORE_H
#define OS_CORE_H

////////////////////////////////
// Basics

typedef enum {
    OS_ACCESS_FLAG_READ       = 1 << 0,
    OS_ACCESS_FLAG_WRITE      = 1 << 1,
    OS_ACCESS_FLAG_EXECUTE    = 1 << 2,
    OS_ACCESS_FLAG_CREATE_NEW = 1 << 3,
    OS_ACCESS_FLAG_ALL        = 0x7fffffff,
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
