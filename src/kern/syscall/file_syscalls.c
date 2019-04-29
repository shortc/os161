/*
 * Current implementation of file oriented systemcalls
 */

#include <types.h>
#include <vfs.h>
#include <vnode.h>
#include <filetable.h>
#include <uio.h>
#include <syscall.h>
#include <device.h>
#include <current.h>
#include <proc.h>
#include <kern/seek.h>
#include <kern/errno.h>
#include <copyinout.h>

int sys___getcwd(void* buffer, size_t buffer_length, int32_t *retval) {

	struct uio uio;
	size_t size;
	int result;

	char *name = (char*)kmalloc(buffer_length);

	result = vfs_getcwd(&uio);
	
	if(result) {
		kprintf("Error getting current working directory!\n");
		return -1;
	};	
	copyoutstr((const void *) name, (userptr_t) buffer, buffer_length, &size);
	*retval = (int32_t) NULL;


    kprintf("successfully retreived cwd!\n");
	return 0;
}

int sys_open(userptr_t user_filename, int flags, int32_t *retval){

    struct filetable_entry *file = kmalloc(sizeof(struct filetable_entry*));
    
    open_file(user_filename, flags, file);
    
    int *file_descript = NULL;

    for (int i = 3; i < OPEN_MAX; i++) {
        if (curproc->entry[i] == (struct filetable_entry*) NULL) {
            curproc->entry[i] = file;
            *file_descript = i;
            break;
        }
    }

    if (file_descript == NULL) {
        return EMFILE;
    }

    *retval = (int32_t) file_descript;

    return 0;
}

int sys_close(int fd) {
    
    if (curproc->entry[fd] == NULL) {
        return EBADF;
    }
    
    close_file(fd);
    curproc->entry[fd] = NULL;
    return 0;
}

int sys_read(int fd, userptr_t user_buffer, size_t buflen, int32_t *retval) {
   
    if (curproc->entry[fd] == NULL) {
        return EBADF;
    }
    
    struct uio uio_reader;
    struct iovec ioc;
    
    uio_kinit(&ioc, &uio_reader, user_buffer, buflen, curproc->entry[fd]->offset, curproc->entry[fd]->r_flag);

    uio_reader.uio_segflg = UIO_USERSPACE;

    VOP_READ(curproc->entry[fd]->vnode, &uio_reader); 

    curproc->entry[fd]->offset = uio_reader.uio_resid;
    
    *retval = (int32_t) (buflen - uio_reader.uio_resid); 
    
    return 0;
}

int sys_write(int fd, userptr_t user_buffer, size_t nbytes, int32_t *retval) {
    
    if (curproc->entry[fd] == NULL) {
        return EBADF;
    }
    
    struct uio uio_writer;
    struct iovec ioc;
    
    uio_kinit(&ioc, &uio_writer, user_buffer, nbytes, curproc->entry[fd]->offset, curproc->entry[fd]->r_flag);
   
    uio_writer.uio_segflg = UIO_USERSPACE;


    VOP_WRITE(curproc->entry[fd]->vnode, &uio_writer);

    curproc->entry[fd]->offset = uio_writer.uio_resid;

    *retval = (int32_t) (nbytes - uio_writer.uio_resid);

    return 0;
}

int sys_lseek(int fd, off_t pos, int whence, int32_t *retval) {

    if (curproc->entry[fd] == NULL) {
        return EBADF;
    }

    if (whence != SEEK_SET || whence != SEEK_CUR || whence != SEEK_END) {
        return EINVAL;
    }
   
    curproc->entry[fd]->offset = pos;

    // Not sure what to do here
 
    *retval = (int32_t) NULL;
    
    return 0;
}

int sys_dup2(int oldfd, int newfd, int32_t *retval) {
    
    if (curproc->entry[oldfd] == NULL) {
        return EBADF;
    }
    if (curproc->entry[newfd] != NULL) {
        sys_close(newfd);
    }
    
    struct filetable_entry *file = kmalloc(sizeof(struct filetable_entry*));

    file->r_flag = curproc->entry[oldfd]->r_flag;
    file->vnode = curproc->entry[oldfd]->vnode;
    file->offset = curproc->entry[oldfd]->offset;

    int *file_descript = NULL;

    for (int i = 3; i < OPEN_MAX; i++) {
        if (filetable[i] == (struct filetable_entry*) NULL) {
            filetable[i] = file;
            *file_descript = i;
            break;
        }
    }

    if (file_descript == NULL) {
        return EMFILE;
    }

    filetable[*file_descript] = file;

    curproc->entry[newfd] = filetable[*file_descript];

    *retval = (int32_t) newfd;
    return 0;
}

int sys_chdir(userptr_t user_filename) {
    
    vfs_chdir((char *)user_filename);

    return 0;
}

void open_file(userptr_t filename, int rflags, struct filetable_entry *file){
    
    struct vnode *vn = kmalloc(sizeof(struct vnode*));

    /* vn may need to be made into a double pointer in line 130  */
    vfs_open((char *)filename, rflags, 0, &vn);


    file->r_flag = rflags;
    file->vnode = vn;
    file->offset = 0;

    for (int i = 3; i < FILE_MAX; i++) {
        if (filetable[i] == NULL) {
            filetable[i] = file;
            break;
        }
    }

}

void close_file(int fd) {
    
    vfs_close(curproc->entry[fd]->vnode);

    kfree(filetable[curproc->entry[fd]->fd_sys_filetable]);
    
    filetable[curproc->entry[fd]->fd_sys_filetable] = NULL;
}

