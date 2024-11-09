#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include "arch/x86_64/E820.h"
#include <stdint.h>

#define NULL (void*) 0

void bootloader_start(E820_map_s * memory_map);

#endif