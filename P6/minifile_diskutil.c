#include "defs.h"

#include "disk.h"
#include "minifile_cache.h"
#include "minifile_diskutil.h"
#include "minifile_fs.h"
#include "minifile_path.h"

/* Make a file system with parameters specified in disk.c */
int
minifile_remkfs()
{
    mem_inode_t inode;
    buf_block_t buf;
    blocknum_t blocknum;
    dir_entry_t dir;

    /* Initialize superblock and bit map*/
    fs_format(mainsb);

    /* Create root inode */
    iget(maindisk, mainsb->root_inum, &inode);
    ilock(inode);
    inode->type = MINIDIRECTORY;
    inode->size = 2;
    blocknum = balloc(maindisk);
    iadd_block(inode, blocknum);
    iupdate(inode);
    iunlock(inode);

    /* Create root inode entries */
    bread(maindisk, blocknum, &buf);
    dir = (dir_entry_t) buf->data;
    strcpy(dir[0].name, ".");
    dir[0].inode_num = mainsb->root_inum;
    strcpy(dir[1].name, "..");
    dir[1].inode_num = mainsb->root_inum;
    bwrite(buf);

    iput(inode);

    printf("minifile system established.\n");

    return 0;
}

/* Check the consistency of the file system on disk */
int
minifile_fsck(disk_t* disk)
{
    char *disk_block_map;
    char *used_block_map;
    mem_inode_t inode;
    blocknum_t i, j;
    blocknum_t count = 0;
    blocknum_t blocknum;

    disk_block_map = malloc(disk->layout.size * sizeof(*disk_block_map));
    used_block_map = malloc(disk->layout.size * sizeof(*used_block_map));

    /* Copy over disk bitmap */
    for (i = 0; i < mainsb->disk_num_blocks; ++i) {
        disk_block_map[i] = bitmap_get(mainsb->block_bitmap, i);
        if (0 == disk_block_map[i])
            count++;
    }
    if (count != mainsb->free_blocks) {
        printf("Inconsistent numer of free blocks: system - %ld, bitmap - %ld.",
               mainsb->free_blocks, count);
        mainsb->free_blocks = count;
        printf("    Fixed.\n");
    }

    /* Set file system used blocks */
    for (i = 0; i <= mainsb->block_bitmap_last; ++i) {
        used_block_map[i]++;
    }

    /* Set file used blocks */
    for (i = 1; i < mainsb->total_inodes; ++i) {
        if (bitmap_get(mainsb->inode_bitmap, i) == 1) {
            iget(maindisk, i, &inode);
            for (j = 0; j < inode->size_blocks; ++j) {
                blocknum = blockmap(maindisk, inode, j);
                used_block_map[blocknum]++;
            }
            iput(inode);
        }
    }

    /* Compare disk bitmap with counted block usage */
    for (i = 0; i < mainsb->disk_num_blocks; ++i) {
        if (disk_block_map[i] != used_block_map[i]) {
            printf("Inconsistency at block %ld.", i);
            if (disk_block_map[i] == 1 || used_block_map[i] == 0) {
                printf("    Free block marked as used on disk \
                       - fixed by freeing the block in bitmap.");
                bfree(i);
            } else if (disk_block_map[i] == 0 || used_block_map[i] == 1) {
                printf("    Used block marked as free on disk \
                       - fixed by setting the block to used in bitmap.");
                bset(i);
            } else {
                printf("    Unable to fix inconsistency: for block %ld - \
                       on disk count %d, actual used count %d",
                       i, disk_block_map[i], used_block_map[i]);
            }
        }
    }

    return 0;
}
