/*
mdrOS second stage bootloader header file
Copyright 2024 Théo Jehl
*/
#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include "arch/x86_64/E820.h"
#include <stdint.h>

#define NULL (void*) 0

void bootloader_start(E820_map_s * memory_map);

void memcpy(char * dest, char * src, uint64_t size);

void memset(char * dest, uint8_t value, uint64_t count);

static inline void deadloop(){while(1);};

#endif
