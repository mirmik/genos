#ifndef MVFS_DIRECTORY_H
#define MVFS_DIRECTORY_H

#include <stdint.h>
#include <mvfs/mvfs.h>

struct dirent {
    uint32_t       d_ino;    /* inode number */
    char           d_name[NAME_LENGTH_MAX + 1]; /* filename */
};

#endif 