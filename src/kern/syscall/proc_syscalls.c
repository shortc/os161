/*
 * Current implementation of the getpid system call
 */

#include <types.h>
#include <syscall.h>
#include <current.h>
#include <proc.h>
#include <filetable.h>

int sys_getpid(int32_t *retval) {
    *retval = (int32_t)curproc->pid;
    return 0;
}


