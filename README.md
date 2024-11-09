# mdrOS

mdrOS is an operating system that serves no purpose, at least not yet

Currently works on x86_64, mdrOS uses a two stage bootloading process and loads the kernel from an EXT2 partition.

```
.
├── CMakeLists.txt
├── include
├── scripts                 //ISO Building scripts
├── src
│   ├── arch                //Architecture dependant code
│   │   └── x86_64
│   │       ├── boot
│   │       │   ├── stage1  //Stage 1 x86_64 bootloader (MBR)
│   │       │   └── stage2  //Stage 2 bootloader (LBA 1 ->  2048 gap)
│   └── kernel              //Kernel source code
└── targets                 //Linker scripts
    └── x86_64              

```

### Building on macOS
Building on macOS is actually terrible but I made something

#### Requirements
- clang
- lld
- nasm
- mkefs (brew install e2fsprogs, make sure to have it in your PATH)
- fdisk

