#include "disk.h"
#include <stdint.h>
#include "serial.h"


void ata_read_bytes(uint32_t start, uint32_t n, char * buffer)
{
    //We can only read sectors by sectors
    //So we will have to adjust the way we read to the size of the buffer and start address.
    //Start address is an offset in bytes.

    char internal_buffer[SECTOR_SIZE];                      //Used in case of unaligned access.
    uint32_t start_block = start/SECTOR_SIZE;               //First block
    uint32_t offset_in_first_block = start%SECTOR_SIZE;     //In case of unaligned reading, use first block offset;
    uint32_t bytes_to_read = n;                             //Remaining bytes to read
    uint32_t bytes_in_this_block = 0;                       //Bytes in the current block, used when reads are not aligned
    uint32_t destination_offset = 0;                        //Current position in the target buffer;

    while(bytes_to_read != 0)
    {
        serial_printf("%d Bytes to read", bytes_to_read);
        ata_lba_read(start_block, 1, (uint16_t * ) internal_buffer);
        bytes_in_this_block = SECTOR_SIZE - offset_in_first_block;

        //The first block might be offset
        if(bytes_in_this_block < SECTOR_SIZE)
        {
            for(int i = 0; i < bytes_in_this_block; ++i)
            {
                buffer[destination_offset] = internal_buffer[i + offset_in_first_block];
                ++destination_offset;
            }
            bytes_to_read = bytes_to_read - bytes_in_this_block;
            offset_in_first_block = 0;
            ++start_block;
            continue;
        }

        //Reaching end block
        if(bytes_to_read < SECTOR_SIZE)
        {
            for(int i = 0; i < bytes_to_read; ++i)
            {
                buffer[destination_offset] = internal_buffer[i];
                ++destination_offset;
            }
            bytes_to_read = 0;
            continue;
        }

        //Otherwise, full block copy
        for(int i = 0; i < SECTOR_SIZE; ++i)
        {
            buffer[destination_offset] = internal_buffer[i];
            ++destination_offset;
        }
        bytes_to_read = bytes_to_read - SECTOR_SIZE;
        ++start_block;
    }
}

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
