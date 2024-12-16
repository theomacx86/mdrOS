/*
mdrOS second stage bootloader source file
Copyright 2024 Théo Jehl
*/

#include "bootloader.h"
#include "serial.h"
#include "disk.h"
#include "ext2.h"
#include "memory.h"
#include <stdint.h>
#include "arch/x86_64/E820.h"

MBR_s mbr = {0};
ext2_superblock_s superblock = {0};

void bootloader_start(E820_map_s * memory_map)
{
    serial_init();
    serial_write_str("mdrBoot Stage 2\nCopyright Theo Jehl\n");
    
    if(memory_map == NULL)
    {
        serial_write_str("[ERROR] Memory map pointer NULL\n");
        while(1);
    }

    for(int i = 0; i < memory_map->map_size; ++i)
    {
        serial_printf("[INFO] Memory entry type %x base %x to %x\n", (uint64_t) (memory_map->map[i].type),  memory_map->map[i].base,  memory_map->map[i].base + memory_map->map[i].len);
    }

    init_memory_map(memory_map);

    //Reads the MBR
    //ata_lba_read(0, 1 , (uint16_t *) (&mbr));
    ata_read_bytes(0, sizeof(MBR_s), (char*) &mbr);
    if(mbr.signature == MBR_SIGNATURE)
    {
        serial_write_str("Found MBR signature.\n");
    }

    //ata_lba_read(mbr.partitions[0].start_LBA + (1024/512), 1, (uint16_t*) (&superblock));

    ata_read_bytes( (mbr.partitions[0].start_LBA * SECTOR_SIZE) + 1024, sizeof(ext2_superblock_s), (char *) &superblock);

    if(superblock.ext2_signature == EXT2_SIGNATURE)
    {
        serial_write_str("Loaded superblock.\n");
        serial_write_str("Found superblock signature.\n");
    }
    
    //Test: reserve the first 2 megs
    reserve_mmio_range(0x0, 0x200000);
    print_memory_map();

    while(1);
    return;
}


void memcpy(char * dest, char * src, uint64_t size)
{
    while (size != 0) {
        *dest = *src;
        ++src;
        ++dest;
        --size;
    }
}

void memset(char * dest, uint8_t value, uint64_t count)
{
    while(count != 0)
    {
        *dest = value;
        ++dest;
        --count;
    }
}
