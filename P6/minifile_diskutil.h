#ifndef __MINIFILE_DISKUTIL_H__
#define __MINIFILE_DISKUTIL_H__

#include "minifile_private.h"

/* Make a file system with the specified number of blocks */
int minifile_mkfs(disk_t* disk, const char* name, blocknum_t size);

/* Check the consistency of the file system on disk */
int minifile_fsck(disk_t* disk);

#endif /* __MINIFILE_DISKUTIL_H__ */
