#ifndef __MINIFILE_INODE_H__
#define __MINIFILE_INODE_H__

#include <stdint.h>
#include "disk.h"
#include "minifile_cache.h"

#define INODE_DIRECT_BLOCKS 11

/* Address space of inodes */
typedef uint64_t inodenum_t;

/* Types of inodes */
typedef enum inode_type {
    MINIFILE,
    MINIDIRECTORY,
    INODE_EMPTY
} itype_t;

typedef enum istatus_type {
	UNCHANGED = 1,
	MODIFIED,
	TO_DELETE
} istatus_t;

/* inode on disk */
typedef struct inode {
        itype_t type;
        size_t size;
        blocknum_t direct[INODE_DIRECT_BLOCKS];
        blocknum_t indirect;
        blocknum_t double_indirect;
        blocknum_t triple_indirect;
} *inode_t;

/* indoe in memory */
typedef struct mem_inode {
    itype_t type;
    size_t size;
    blocknum_t direct[INODE_DIRECT_BLOCKS];
    blocknum_t indirect;
    blocknum_t double_indirect;
    blocknum_t triple_indirect;

    disk_t* disk;               /* Logic disk */
    inodenum_t num;             /* Inode number */
    buf_block_t buf;            /* Buffer cache containing this inode */
    blocknum_t size_blocks;     /* Number of blocks in data */
    size_t ref_count;           /* Reference count */
	istatus_t status;           /* Inode status: modified, to delete, etc... */
    semaphore_t inode_lock;     /* Read write lock */

	struct mem_inode* h_prev;   /* Hash table previous */
	struct mem_inode* h_next;   /* Hash table next */
	struct mem_inode* l_prev;   /* Free list previous */
	struct mem_inode* l_next;   /* Free list next */
} *mem_inode_t;

semaphore_t inode_lock;         /* Inode lock for iget and iput */
struct inode _root_inode;       /* Root inode */
mem_inode_t root_inode;         /* Root inode number */

extern int iclear(disk_t* disk, inodenum_t n);
extern int iget(disk_t* disk, inodenum_t n, mem_inode_t *inop);
extern void iput(mem_inode_t ino);
extern int iupdate(mem_inode_t ino);

/* Byte offset within inode to disk block number */
extern blocknum_t bytemap(disk_t* disk, mem_inode_t ino, size_t byte_offset);
/* Block offset within inode to disk block number */
extern blocknum_t blockmap(disk_t* disk, mem_inode_t ino, size_t block_offset);

#endif /* __MINIFILE_INODE_H__ */
