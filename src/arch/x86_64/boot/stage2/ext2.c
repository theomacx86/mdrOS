#include "bootloader.h"
#include "disk.h"
#include <stdint.h>
#include "ext2.h"
#include "serial.h"

uint32_t ext2_start_lba;
MBR_s mbr = {0};
ext2_superblock_s superblock = {0};
uint32_t ext2_block_size;

void ext2_init()
{
    ext2_block_group_descriptor_s block_desc = {0};

    //Reads the MBR
    ata_read_bytes(0, sizeof(MBR_s), (char*) &mbr);
    if(mbr.signature == MBR_SIGNATURE)
    {
        serial_write_str("[EXT2] Found MBR signature.\n");
    }
    else
    {
        //No filesystem found, cannot continue
        serial_write_str("[EXT2] NO MBR FOUND, ABORT!.\n");
        deadloop();
    }

    //Read the first partition only, for now the OS will always be located at partition 0 lol
    //We might want to change this down the line
    ata_read_bytes( (mbr.partitions[0].start_LBA * SECTOR_SIZE) + 1024, sizeof(ext2_superblock_s), (char *) &superblock);

    if(superblock.ext2_signature == EXT2_SIGNATURE)
    {
        serial_write_str("[EXT2] Loaded superblock.\n");
        serial_write_str("[EXT2] Found superblock signature.\n");
        ext2_start_lba = mbr.partitions[0].start_LBA;
    }

    ext2_block_size = 1024 << superblock.log2_block_size;
    serial_printf("[EXT2] Block size is %x\n", ext2_block_size);

    ata_read_bytes( BLOCK_TO_BYTE(ext2_start_lba, ext2_block_size, 2), sizeof(ext2_block_group_descriptor_s), 
        (char*) &block_desc);
    
    serial_printf("[EXT2] Starting inode block %x directories count %x", block_desc.inode_table_address, block_desc.directories_count);
    while(1);
}



