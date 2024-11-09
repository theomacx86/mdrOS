#ifndef DISK_H
#define DISK_H

#define LBA_PORT_0_7            0x1F3
#define LBA_PORT_8_15           0x1F4
#define LBA_PORT_16_23          0x1F5
#define LBA_PORT_24_27          0x1F6
#define LBA_PORT_SECTOR_COUNT   0x1F2
#define LBA_COMMAND_PORT        0x1F7
#define LBA_DATA_PORT           0x1F0

#define LBA_COMMAND_READ_RETRY  0x20

#include <stdint.h>
/**
    Read n LBA into the designated buffer.
 */
void ata_lba_read(uint32_t LBA, uint8_t count, uint16_t * buffer);


#endif