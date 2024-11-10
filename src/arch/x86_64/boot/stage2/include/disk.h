#ifndef DISK_H
#define DISK_H

#include <stdint.h>

#define LBA_PORT_0_7            0x1F3
#define LBA_PORT_8_15           0x1F4
#define LBA_PORT_16_23          0x1F5
#define LBA_PORT_24_27          0x1F6
#define LBA_PORT_SECTOR_COUNT   0x1F2
#define LBA_COMMAND_PORT        0x1F7
#define LBA_DATA_PORT           0x1F0

#define LBA_COMMAND_READ_RETRY  0x20

#define MBR_SIGNATURE 0xAA55
typedef struct partition_entry
{
    unsigned char drive_attr;
    unsigned char CHS_addr_start[3];
    char partition_type;
    unsigned char CHS_addr_end[3];
    uint32_t start_LBA;
    uint32_t number_sectors;
} __attribute__((packed)) partition_entry_s;

typedef struct MBR 
{
    char MBR_Bootstrap[440];
    uint32_t disk_signature;
    uint16_t reserved;
    partition_entry_s partitions[4];
    uint16_t signature;
} __attribute__((packed)) MBR_s;

/**
    Read n LBA into the designated buffer.
 */
void ata_lba_read(uint32_t LBA, uint8_t count, uint16_t * buffer);


#endif
