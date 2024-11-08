#!/bin/zsh
set -e 
echo "Creating ISO file"
rm -fr mdrOS.iso
clang scripts/createx64iso.c -o scripts/createx64iso
./scripts/createx64iso 50
DISK_IDENTIFIER=$(hdiutil attach -nomount mdrOS.iso | awk 'NR==1 {print $1}')
mke2fs -t ext2 "${DISK_IDENTIFIER}s1"
fdisk mdrOS.iso