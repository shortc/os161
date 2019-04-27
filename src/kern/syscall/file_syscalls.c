/*
 * Current implementation of file oriented systemcalls
 */

#include <vfs.h>
#include <vnode.h>
#include <filetable.h>

// Syscall ID 74
int sys_chdir() {
    return EFAULT;
    return EIO;
    return ENAMETOOLONG;
    return ENOENT;
    return ENOMEM;
    return ENOTDIR;
    return EBADF;
}

// Syscall ID 76
int sys___getcwd(int *retval) {

	struct uio uio;
	int result;

	result = vfs_getcwd(&uio);
	
	if(result) {
		kprintf("Error getting current working directory!\n");
		return -1;
	}


	//retval = uio?
    kprintf("successfully retreived cwd!\n");
	return 0;
}



// Syscall ID 45
int sys_open() {
    return 0;
}

// Syscall ID 50
int sys_read() {
    return 0;
}

// Syscall ID 55
int sys_write() {
    return 0;
}

// Syscall ID 59
int sys_lseek() {
    return 0;
}

// Syscall ID 49
int sys_close() {
    return 0;
}

// Syscall ID 48
int sys_dup2() {
    return 0;
}
   
int open_file() {

}

int close_file() {

}

