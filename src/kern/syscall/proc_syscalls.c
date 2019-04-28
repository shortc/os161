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

/*
int sys_fork(int *retval) {
   
	int id;
	sys_getpid(&id);
    
	return 0;
}
*/

/*
int sys_execv(const char *path, char *const argv[]) {
    
	return 0;
}
*/


int sys_waitpid(int32_t pid, int *status, int options, int *retval) {

	(void)pid;
	(void)status;
	(void)options;
	(void)retval;

	return 0;
}



int sys__exit(int retcode) {
    //int b;
	//b = retcode;
	(void)retcode;
	thread_exit();
	return 0;
}


