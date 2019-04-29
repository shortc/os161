/*
 * Current implementation of the getpid system call
 */



//#include <unistd.h>
#include <types.h>
#include <syscall.h>
#include <current.h>
#include <proc.h>
#include <filetable.h>
#include <kern/errno.h>
#include <synch.h>

int sys_getpid(int32_t *retval) {
    *retval = (int32_t)curproc->pid;
    return 0;
}


int sys_fork(struct trapframe *tf, int *retval) {
  
	(void)tf;
	int id;
	sys_getpid(&id);


	//error = thread_fork(...);
	*retval = -1;

	//*retval = new_thread->pid;
	return 0;
}




int sys_execv(const char *path, char **args, int *retval) {
   
	(void)path;
	(void)args;
	(void)retval;
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
/*	result = as_define_stack(as, &stackptr)	if (result) {
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
	return 0;
}





// Wait for a certain thread to exit

int sys_waitpid(int32_t pid, int *status, int options, int *retval) {

	

	struct proc *child_proc;

	if(options) {
		kprintf("waitpid: options attempted but not implemented\n");
		*retval = -1;
		return EINVAL;

	}

/*pid < -1: wait for a thread with pid group equal to absolute value of pid*/
/*pid == 0: wait for a thread with pid group equal to that of the calling process*/
	if(pid < -1 || pid == 0) {
		kprintf("waitpid: functionality for pid (%d) not yet implemented\n", pid);
		*retval = -1;
		return EINVAL;
	}

	if(pid > 0 && !test_bit(pid)){
		*retval = -1;
		kprintf("waitpid: tried to wait on a pid that is not in use\n");
		return ECHILD;
		
	}


//pid == -1: wait for any child thread 
//pid > 0: wait for a thread with process group pid

	if(pid > 0) {
		child_proc = fetch_proc(pid);
	}
	else {
		child_proc = fetch_gen_proc();
	}
	lock_acquire(child_proc->exit_lock);
	cv_wait(child_proc->exit_cv, child_proc->exit_lock);
	lock_release(child_proc->exit_lock);

	/*"Take Care Of" child thread?*/

	*status = child_proc->exitcode;


/*on success, returns the process ID of  the  child  whose  state  has changed;  
if  WNOHANG was specified and one or more child(ren) specified by pid exist, 
but have not yet changed state, then 0 is returned.   On  error,  -1  is 
returned

ECHILD (for  waitpid() or waitid()) The process specified by pid (waitpid()) or
idtype and id (waitid()) does not exist or is not a child of the calling
process.  (This can happen for one's own child if the action for SIGCHLD
is set to SIG_IGN.  See also the Linux Notes section about threads.)
*/


	proc_destroy(child_proc);

	return 0;
}



int sys__exit(int retcode) {
/*
The  function  _exit()  terminates the calling process "immediately".  Any open
file descriptors belonging to the process  are  closed;  any  children  of  the
process  are  inherited  by process 1, init, and the process's parent is sent a
SIGCHLD signal.

The value status is returned to the parent process as the process's  exit  sta-
tus, and can be collected using one of the wait(2) family of calls.
*/

	//Close open files and any taken file descriptors
		//vfs_close(vn);?

	lock_acquire(curproc->exit_lock);
	cv_broadcast(curproc->exit_cv, curproc->exit_lock);
	gen_exit_signal();
	curproc->exitcode = retcode;
	lock_release(curproc->exit_lock);

	thread_exit();
	//Should not reach past this (i.e. never returns)
	return 0;
}


