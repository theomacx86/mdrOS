/*
mdrOS second stage bootloader ext2 header file
Copyright 2024 Théo Jehl
*/

#ifndef EXT2_h
#define EXT2_h

#include <stdint.h>
#include "disk.h"

/*
    Convert a EXT2 block address to a physical byte on disk
    @param START_BLOCK : starting block on disk (sector)
    @param BLOCK_SIZE : block size (from EXT2 superblock)
    @param BLOCK : target block
*/
#define BLOCK_TO_BYTE(START_BLOCK, BLOCK_SIZE, BLOCK) ((START_BLOCK*SECTOR_SIZE) + (BLOCK_SIZE*BLOCK))

#define EXT2_SIGNATURE 0xEF53

#define FILESYSTEM_STATE_CLEAN 1
#define FILESYSTEM_STATE_ERROR 2

#define ERROR_HANDLING_IGNORE 1
#define ERROR_HANDLING_REMOUNT_RO 2
#define ERROR_HANDLING_KP 3

typedef struct ext2_superblock
{
    uint32_t total_inode_count;
    uint32_t total_block_count;
    uint32_t superuser_reserved_block_count;
    uint32_t total_unallocated_blocks_count;
    uint32_t total_unallocated_inodes_count;
    uint32_t superblock_block_number;
    uint32_t log2_block_size;
    uint32_t log2_fragment_size;
    uint32_t blocks_count_per_group;
    uint32_t fragment_count_per_group;
    uint32_t inode_count_per_group;
    uint32_t last_mount_time;
    uint32_t last_written_time;
    uint16_t mount_count_since_check;
    uint16_t max_mount_count_before_check;
    uint16_t ext2_signature;
    uint16_t filesystem_state;
    uint16_t error_handling_method;
    uint16_t minor_version;
    uint32_t last_check_time;
    uint32_t time_interval_checks;
    uint32_t os_id;
    uint32_t major_version;
    uint16_t userid_reserved_block_;
    uint16_t groupid_reserved_block;
    char filler[428];
} __attribute__((packed)) ext2_superblock_s;

typedef struct ext2_block_group_descriptor
{
    uint32_t block_usage_bitmap;
    uint32_t inode_usage_bitmap;
    uint32_t inode_table_address;
    uint16_t unallocated_block_count;
    uint16_t unallocated_inodes_group;
    uint16_t directories_count;
} __attribute__((packed)) ext2_block_group_descriptor_s;

typedef struct ext2_inode {
    uint16_t type_and_permissions; // 0-1: Type and Permissions
    uint16_t user_id;              // 2-3: User ID
    uint32_t size_lower;           // 4-7: Lower 32 bits of size in bytes
    uint32_t access_time;          // 8-11: Last Access Time (POSIX time)
    uint32_t creation_time;        // 12-15: Creation Time (POSIX time)
    uint32_t modification_time;    // 16-19: Last Modification time (POSIX time)
    uint32_t deletion_time;        // 20-23: Deletion time (POSIX time)
    uint16_t group_id;             // 24-25: Group ID
    uint16_t hard_link_count;      // 26-27: Count of hard links
    uint32_t sector_count;         // 28-31: Count of disk sectors
    uint32_t flags;                // 32-35: Flags
    uint32_t os_specific_1;        // 36-39: OS Specific Value #1
    uint32_t direct_block[12];     // 40-87: Direct Block Pointers 0-11
    uint32_t singly_indirect;      // 88-91: Singly Indirect Block Pointer
    uint32_t doubly_indirect;      // 92-95: Doubly Indirect Block Pointer
    uint32_t triply_indirect;      // 96-99: Triply Indirect Block Pointer
    uint32_t generation_number;    // 100-103: Generation number
    uint32_t extended_attribute;   // 104-107: Extended attribute block
    uint32_t upper_file_size;      // 108-111: Upper 32 bits of file size or Directory ACL
    uint32_t fragment_block;       // 112-115: Block address of fragment
    uint8_t os_specific_2[12];     // 116-127: OS Specific Value #2
} __attribute__((packed)) ext2_inode_s;

#define INODE_TYPE_FIFO         0x1000
#define INODE_TYPE_CHARDEV      0x2000
#define INODE_TYPE_DIRECTORY    0x4000
#define INODE_TYPE_BLOCKDEV     0x6000
#define INODE_TYPE_FILE         0x8000
#define INODE_TYPE_SYMLINK      0xA000
#define INODE_TYPE_SOCKET       0xC000

void ext2_init();

uint32_t ext2_get_block_size();


#endif
