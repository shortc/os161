/*
 * Current implementation of file oriented systemcalls
 */

#include <vfs.h>
#include <vnode.h>

// Syscall ID 74
int chdir() {
    return 0; 
}

// Syscall ID 76
int sys_getcwd(int *retval) {

	struct uio uio;
	int result;

	result = vfs_getcwd(&uio);
	
	if(result) {
		panic("Error getting directory!\n");
	}

	//value = uio?
    return 0;
}



///////////////////////////
//  Unsure if necessary  //
//                       //

// Syscall ID 45
int open() {

	return 0;
}

// Syscall ID 50
int read() {
    
	return 0;
}

// Syscall ID 55
int write() {
    
	return 0;
}

// Syscall ID 59
int lseek() {
    
	return 0;
}

// Syscall ID 49
int close() {
    
	return 0;
}

// Syscall ID 48
int dup2() {
    
	return 0;
}
   
//                       //
//  Unsure if necessary  //
///////////////////////////


