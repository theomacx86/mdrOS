#!/bin/zsh
set -e 
echo "Creating ISO file"
rm -fr mdrOS.iso
clang scripts/createx64iso.c -o scripts/createx64iso
./scripts/createx64iso 50 build/bin/stage1_bootloader build/bin/stage2_bootloader
DISK_IDENTIFIER=$(hdiutil attach -nomount mdrOS.iso | awk 'NR==1 {print $1}')
echo "Creating disk-wide EXT2 partition"
mke2fs -t ext2 "${DISK_IDENTIFIER}s1"
echo "Disk geometry"
fdisk mdrOS.iso
hdiutil detach $DISK_IDENTIFIER
echo "Successfully created bootable MBR ISO"