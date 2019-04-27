#ifndef FILETABLE_H
#define FILETABLE_H

#include <vnode.h>
#include <limits.h>


struct filetable_entries {    
    int r_flag;
    int ref_num;

    struct vn *vnode;
};

//struct filetable_entries *filetable = kmalloc(FILE_MAX*sizeof(struct *filetable_entries)); 

#endif
