#include "bootloader.h"
#include "serial.h"
#include <stdint.h>


void bootloader_start(E820_map_s * memory_map)
{
    serial_write_str("mdrBoot Stage 2\nCopyright Theo Jehl\n");
    while(1);
    return;
}