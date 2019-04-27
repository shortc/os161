/*
 * Current implementation of file oriented systemcalls
 */

#include <types.h>
#include <vfs.h>
#include <vnode.h>
#include <filetable.h>
#include <uio.h>
#include <syscall.h>

int sys___getcwd(int32_t *retval) {

	struct uio uio;
	int result;

	result = vfs_getcwd(&uio);
	
	if(result) {
		kprintf("Error getting current working directory!\n");
		return -1;
	}


	*retval = (int32_t) NULL;


    kprintf("successfully retreived cwd!\n");
	return 0;
}

