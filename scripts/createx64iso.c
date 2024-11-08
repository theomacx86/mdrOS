#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define SECTOR_SIZE 512
#define HPC 16
#define SPT 63

typedef struct CHS
{
    uint32_t cylinder;
    uint32_t head;
    uint32_t sector;
} CHS_s;

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

CHS_s * LBA_to_CHS(uint32_t LBA, CHS_s * CHS)
{
    CHS->cylinder = LBA/(HPC * SPT);
    CHS->head = (LBA/SPT)%HPC;
    CHS->sector = (LBA%SPT) +1;
    return CHS;
}

bool install_mbr(FILE * iso, char * stage1path)
{
    FILE * mbr;
    MBR_s mbr_s = {0};
    uint32_t filesize = 0;
    fseek(iso, 0, 0);
    mbr = fopen(stage1path, "r");
    if(!mbr)
    {
        return false;
    }
    
    fseek(mbr, 0, SEEK_END);
    filesize = ftell(mbr);
    if(filesize > 446)
    {
        puts("MBR too large");
        return -1;
    }

    printf("MBR Code is %d bytes long\n", filesize);

    fseek(mbr, 0, 0);
    fread(&(mbr_s.MBR_Bootstrap) , filesize, 1, mbr);

    fseek(iso, 0, 0);
    fwrite(&(mbr_s.MBR_Bootstrap), filesize, 1,iso);
    fclose(mbr);
    return true;
}

void create_mbr(FILE * iso, uint32_t disk_size)
{
    MBR_s mbr;
    partition_entry_s part1;
    CHS_s CHS;

    memset(&mbr, 0, sizeof(MBR_s));
    memset(&part1, 0, sizeof(partition_entry_s));

    part1.partition_type = 0x83; //linux
    part1.drive_attr = 0x80;

    //First LBA, LBA 2014 = 1Mb
    LBA_to_CHS(2048, &CHS);

    part1.CHS_addr_start[0] = (unsigned char) CHS.head;
    part1.CHS_addr_start[1] = (unsigned char) CHS.sector;
    part1.CHS_addr_start[2] = (unsigned char) CHS.cylinder;

    LBA_to_CHS((disk_size - 512) / 512, &CHS);
    part1.CHS_addr_end[0] = (unsigned char) CHS.head;
    part1.CHS_addr_end[1] = (unsigned char) CHS.sector;
    part1.CHS_addr_end[2] = (unsigned char) CHS.cylinder;

    part1.start_LBA = 2048;
    part1.number_sectors = ((disk_size - 512)/512) - 2048;

    mbr.signature = 0xAA55;

    memcpy(&(mbr.partitions[0]), &part1, sizeof(partition_entry_s));

    fwrite(&mbr, sizeof(MBR_s), 1, iso);
    
    return;
}

bool install_secondary_bootlooader(FILE * iso, char * stage2path)
{
    FILE * payload;
    uint32_t filesize = 0;
    fseek(iso, 0, 0);
    payload = fopen(stage2path, "r");

    char * payload_mem;
    payload_mem = malloc(filesize);
    fseek(payload, 0, SEEK_END);
    filesize = ftell(payload);
    fseek(payload, 0, 0);
    fread(payload_mem, filesize, 1, payload);

    
    fseek(iso, 512, 0);
    fwrite(payload_mem, filesize,1 , iso);
    fclose(payload);
    free(payload_mem);
    return true;
}

void fill_image(FILE * iso, uint32_t filesize)
{
    filesize = filesize - 512;      //LBA 0 is filled !!
    char buffer[512] = {0};
    fseek(iso, 512, 0);             //LBA 1
    for(int i = 0; i < filesize / 512; ++i)
    {
        fwrite(&(buffer[0]), 512, 1, iso);
    }
}

int main(int argc, char *argv[])
{
    FILE * iso = NULL;
    uint32_t disk_size = 0;
    if(argc <= 3)
    {
        puts("Usage: Disk size (Mb), stage1 bootloader path, stage2 bootloader path.");
        return -1;
    }

    iso = fopen("mdrOS.iso","w+");
    if(!iso)
    {
        puts("Error when opening file");
        return -1;
    }

    disk_size = atoi(argv[1]);
    
    disk_size = disk_size * 0x100000;               //byte size
    disk_size = disk_size - (disk_size % 512);      //Align
    create_mbr(iso, disk_size);
    puts("Partition table created\n");

    if(!install_mbr(iso, argv[2]))
    {
        puts("Error while writing MBR");
        return -1;
    }
    puts("Bootstrap code installed\n");

    fill_image(iso, disk_size);
    printf("Disk image is now %d bytes large\n", disk_size);

    if(!install_secondary_bootlooader(iso, argv[3]))
    {
        puts("Failed to install secondary bootloader, image might be corrupted !");
        fclose(iso);
        return -1;
    }
    puts("Installed secondary bootloader\n");

    fclose(iso);
}