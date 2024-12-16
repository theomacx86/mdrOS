/*
    e820 definition header file
    Copyright Théo JEHL - 2024
*/

#ifndef E820_H
#define E820_H

#include <stdint.h>

#define E820_MAX_ENTRIES 128

typedef struct E820_entry
{
    uint64_t base;
    uint64_t len;
    uint32_t type;
    uint32_t ACPI_attributes;
} __attribute__((packed)) E820_entry_s;

typedef struct E820_map
{
    uint32_t map_size;
    E820_entry_s map[E820_MAX_ENTRIES];
} E820_map_s;

#define E820_MEMORY_FREE            1
#define E820_MEMORY_RESERVED        2
#define E820_MEMORY_ACPI            3
#define E820_MEMORY_NVS             4
#define E820_MEMORY_UNUSABLE        5

#define KMEM_MMIO                   6

#endif
