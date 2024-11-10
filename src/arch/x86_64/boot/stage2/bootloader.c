#include "bootloader.h"
#include "serial.h"
#include "disk.h"
#include "ext2.h"
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

    //Reads the MBR
    ata_lba_read(0, 1 , (uint16_t *) (&mbr));
    if(mbr.signature == MBR_SIGNATURE)
    {
        serial_write_str("Found MBR signature.\n");
    }

    ata_lba_read(mbr.partitions[0].start_LBA + (1024/512), 1, (uint16_t*) (&superblock));

    if(superblock.ext2_signature == EXT2_SIGNATURE)
    {
        serial_write_str("Loaded superblock.\n");
        serial_write_str("Found superblock signature.\n");
    }
    
    while(1);
    return;
}
