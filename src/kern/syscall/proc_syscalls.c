/*
 * Current implementation of the getpid system call
 */


#include <unistd.h>
#include <current.h>
#include <proc.h>

int sys_getpid(pid_t curpid) {
    curpid = curproc->pid;
    return 0;
}
