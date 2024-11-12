#!/bin/bash
echo "Creating ISO file"
rm -fr mdrOS.iso
clang scripts/createx64iso.c -o scripts/createx64iso
./scripts/createx64iso 50 build/bin/stage1_bootloader build/bin/stage2_bootloader
