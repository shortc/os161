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
    
	error = thread_fork(...);
	return 0;
}
*/


/*
int sys_execv(const char *path, char *const argv[]) {
   */




/*
	struct addrspace *as;
    struct vnode *v;
    vaddr_t entrypoint, stackptr;
    int result;
*/    



	/* Open the file. */
/*	result = vfs_open(progname, O_RDONLY, 0, &v);
	if (result) {
	    return result;
	}
*/

	/* We should be a new process. */
/*	KASSERT(proc_getas() == NULL);
*/			 
	/* Create a new address space. */
/*	as = as_create();
	if (as == NULL) {
		vfs_close(v);
		return ENOMEM;
	}
*/				    
	/* Switch to it and activate it. */
/*	proc_setas(as);
	as_activate();
*/					    
	/* Load the executable. */
/*	result = load_elf(v, &entrypoint);
	if (result) {
*/		/* p_addrspace will go away when curproc is destroyed */
/*		vfs_close(v);
		return result;
	}
*/							    
	/* Done with the file now. */
/*	vfs_close(v);
*/								   
	/* Define the user stack in the address space */
/*	result = as_define_stack(as, &stackptr);
	if (result) {
*/		/* p_addrspace will go away when curproc is destroyed */
/*		return result;
	}
*/										  
	/* Warp to user mode. */
//	enter_new_process(0 /*argc*/, NULL /*userspace addr of argv*/, 
//			NULL /*userspace addr of environment*/,
//			stackptr, entrypoint);
   
	/* enter_new_process does not return. */
//	panic("enter_new_process returned\n");
//	return EINVAL;




/*

	return 0;
}
*/



int sys_waitpid(int32_t pid, int *status, int options, int *retval) {

	(void)pid;
	(void)status;
	(void)options;
	(void)retval;

	//if pid < -1
		//wait for a thread with process group ID equal to absolute value of pid
	//if pid == -1
		//wait for any child thread 
	//if pid == -1
		//wait for a thread with process group ID equal to that of the calling process
	//if pid == -1
		//wait for a thread with process group pid




	return 0;
}



int sys__exit(int retcode) {
	(void)retcode;
	
	
	thread_exit();
	//Should not reach past this (i.e. never returns)
	return 0;
}


