#ifndef FILETABLE_H
#define FILETABLE_H

#include <vnode.h>
#include <limits.h>


struct filetable_entry {    
    int r_flag;
    int ref_count;
    off_t offset;
    int fd_sys_filetable;
    struct vnode *vnode;
};

struct filetable_entry **filetable; 

#endif
