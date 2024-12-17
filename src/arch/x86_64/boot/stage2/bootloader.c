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


void bootloader_start(E820_map_s * memory_map)
{
    serial_init();
    serial_write_str("[INFO] mdrBoot Stage 2\n[INFO] Copyright Theo Jehl\n");
    
    if(memory_map == NULL)
    {
        serial_write_str("[ERROR] Memory map pointer NULL\n");
        deadloop();
    }

    for(int i = 0; i < memory_map->map_size; ++i)
    {
        serial_printf("[INFO] Memory entry type %x base %x to %x\n", (uint64_t) (memory_map->map[i].type),  memory_map->map[i].base,  memory_map->map[i].base + memory_map->map[i].len);
    }

    //Init memory map
    //Reserves the 0->3Mb range, as we have out bootloader loaded there with some important data.
    init_memory_map(memory_map);
    reserve_mmio_range(0x0, 0x300000);

    ext2_init();
    
    print_memory_map();

    deadloop();
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
