/*
mdrOS second stage bootloader memory header file
Copyright 2024 Théo Jehl
*/

#ifndef MEMORY_H
#define MEMORY_H

#include "arch/x86_64/E820.h"
#include <stdint.h>

/*
    Initializes kernel memory map based on the BIOS one
    Sanitize and clear map from overlapping regions
*/
void init_memory_map(E820_map_s * bios_map);

/*
    Mark a region as reserved
*/
void reserve_memory_range(uint64_t base, uint64_t size);

/*
    Mark a region as MMIO reserved
*/
void reserve_mmio_range(uint64_t base, uint64_t size);

/*
    Prints the map ?
*/
void print_memory_map();

#endif
