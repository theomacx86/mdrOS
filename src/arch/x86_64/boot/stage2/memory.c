/*
mdrOS second stage bootloader memory source file
Copyright 2024 Théo Jehl
*/

#include <stdbool.h>
#include <stdint.h>
#include "bootloader.h"
#include "serial.h"
#include "arch/x86_64/E820.h"


E820_map_s memory_map;
E820_entry_s entries[E820_MAX_ENTRIES];

void reserve_mmio_range(uint64_t base, uint64_t size)
{
    E820_entry_s entry;                 //New memory entry to be added

    entry.base = base;                  //Copy memory 
    entry.len = size;
    entry.type = E820_MEMORY_RESERVED;
    entry.ACPI_attributes = 0;

    serial_printf("Registering reserved %x -> %x range.\n", entry.base, entry.base + entry.len);

    //Looking for collision with free ranges.
    for(int i = 0; i < memory_map.map_size; ++i)
    {
        if(memory_map.map[i].type == E820_MEMORY_FREE)
        {
            if(entry.base < memory_map.map[i].base + memory_map.map[i].len && memory_map.map[i].base < entry.base + entry.len)
            {
                serial_printf("Collision between reserved %x -> %x with free %x -> %x\n", 
                    entry.base, entry.base + entry.len, memory_map.map[i].base, memory_map.map[i].base + memory_map.map[i].len);
                
                if(memory_map.map[i].base >= entry.base && memory_map.map[i].base + memory_map.map[i].len <= entry.base + entry.len)
                {
                    serial_write_str("Deleting free range\n");
                    memory_map.map[i].type = E820_MEMORY_RESERVED;
                }
                else
                {
                    serial_write_str("Truncate free range.\n");
                    //If the beginning of the memory zone is within the memory range, beginning = end of reserved range.
                    //The previous condition already handled a memory range that fits within the reserved range.
                    if(memory_map.map[i].base >= entry.base && memory_map.map[i].base <= entry.base +entry.len)
                    {
                        memory_map.map[i].base = entry.base + entry.len;
                    }

                    //Same for the end
                    //This the map.base + map.len = entry.base
                    if(memory_map.map[i].base + memory_map.map[i].len >= entry.base && memory_map.map[i].base + memory_map.map[i].len <= entry.base + entry.len)
                    {
                        memory_map.map[i].len = entry.base - memory_map.map[i].base;
                    }

                    serial_printf("New free range values %x -> %x\n", 
                    memory_map.map[i].base, memory_map.map[i].base + memory_map.map[i].len);
                }
            }
        }
    }

    //Once any conflicts has been cleared, add.
    memcpy((char *) &(memory_map.map[memory_map.map_size]) , (char*) &entry, sizeof(E820_entry_s));
    ++memory_map.map_size;
}

void print_memory_map()
{
    for(int i = 0; i < memory_map.map_size; ++i)
    {
        serial_printf("[MEM] %x -> %x ", memory_map.map[i].base, memory_map.map[i].base + memory_map.map[i].len);
        switch (memory_map.map[i].type) {
            case E820_MEMORY_FREE:
                serial_write_str("free\n");
                break;
            case E820_MEMORY_RESERVED:
                serial_write_str("reserved\n");
                break;
            case E820_MEMORY_ACPI:
                serial_write_str("reserved (ACPI)\n");
                break;
            case E820_MEMORY_NVS:
                serial_write_str("reserved (NVS)\n");
                break;
            case E820_MEMORY_UNUSABLE:
                serial_write_str("unusable\n");
                break;
            default:
                serial_write_str("?\n");
                break;
        }
    }
}

void init_memory_map(E820_map_s * bios_map)
{

    memory_map.map_size = bios_map->map_size;
    memcpy((char*) memory_map.map, (char*) bios_map->map, memory_map.map_size * sizeof(E820_entry_s));

    serial_printf("[MEM] Regisered %d reserved regions\n", memory_map.map_size);

    return;

    //TODO
    //QEMU gives us a clean memory map, how cool.
    
    
}
