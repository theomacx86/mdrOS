#include "disk.h"
#include <stdint.h>
#include "serial.h"


void ata_lba_read(uint32_t LBA, uint8_t count, uint16_t * buffer)
{
    //Send bit 24 - 27 of the LBA
    outb(LBA_PORT_24_27, (uint8_t) ((LBA >> 24) | 0xE0));

    //Send number of sectors to read
    outb(LBA_PORT_SECTOR_COUNT, count);

    //Send bit 0 to 7 of LBA
    outb(LBA_PORT_0_7, (uint8_t) LBA);

    //Send bit 8 - 15
    outb(LBA_PORT_8_15, (uint8_t) (LBA >> 8));

    //Send bit 16 23
    outb(LBA_PORT_16_23, (uint8_t) (LBA >> 16));

    outb(LBA_COMMAND_PORT, 0x20);

    serial_write_str("Reading...\n");

    while(!(inb(LBA_COMMAND_PORT) & 0x8));

    for(int i = 0 ; i < ((uint32_t) count) * 256; ++i)
    {
        buffer[i] = inw(LBA_DATA_PORT);
    }
}
