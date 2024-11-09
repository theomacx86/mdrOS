#include "bootloader.h"
#include "serial.h"
#include "disk.h"
#include <stdint.h>
#include "arch/x86_64/E820.h"

char sector[512];

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

    ata_lba_read(0, 1 , (uint16_t *) &sector);
    
    for(int i = 0; i < 512; ++i)
    {
        serial_printf("%x ", sector[i]);
    }
    while(1);
    return;
}