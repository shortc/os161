/*
 * Current implementation of the getpid system call
 */



//#include <unistd.h>
#include <types.h>
#include <syscall.h>
#include <current.h>
#include <proc.h>
#include <filetable.h>

int sys_getpid(int32_t *retval) {
    *retval = (int32_t)curproc->pid;
    return 0;
}


int sys_fork(/* other stuff? */int *retval) {
    
	sys_getpid(&id);
    
	return 0;
}

int sys_execv(const char *path, char *const argv[]) {
    
	return 0;
}

int sys_waitpid(pid_t pid, int *status, int options, int *retval) {
    
	return 0;
}

int sys___exit(int retcode = 0) {
    
	thread_exit();
	return 0;
}
