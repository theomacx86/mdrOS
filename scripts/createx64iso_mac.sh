#!/bin/zsh
set -e 
echo "Creating ISO file"
rm -fr mdrOS.iso
dd if=/dev/zero of=mdrOS.iso bs=1M count=100  && sync
DISK="$(hdiutil attach -nomount mdrOS.iso)"

DISK_IDENTIFIER=$(echo $DISK | awk '{print $1}')
echo "Disk mounted on $DISK_IDENTIFIER"

#Why ? well on macos sfdisk doesn't exist so the only way to create a proper
#file with a correctly formed MBR is to create a FAT32 disk with diskutil 
#and then format it again

echo "Initial MBR wipe"
diskutil partitionDisk $DISK_IDENTIFIER 1 MBR FAT32 MDROS 100%
umount ${DISK_IDENTIFIER}s1

echo "Create EXT2 partition" 
yes |mke2fs -t ext2 ${DISK_IDENTIFIER}s1
hdiutil detach ${DISK_IDENTIFIER}

echo "Patching MBR partition ID"
printf '\x83' | dd of=mdrOS.iso bs=1 seek=450 conv=notrunc

echo "Installing stage 1 bootloader"
dd if=build/bin/stage1.bin of=mdrOS.iso bs=1 seek=0 conv=notrunc

echo "Installint stage 2 bootloader"
dd if=build/bin/stage2_bootloader of=mdrOS.iso bs=1 seek=512 conv=notrunc
