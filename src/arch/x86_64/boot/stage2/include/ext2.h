/*
mdrOS second stage bootloader ext2 header file
Copyright 2024 Théo Jehl
*/

#ifndef EXT2_h
#define EXT2_h

#include <stdint.h>

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

#endif
