ENTRY(bootloader_start)
SECTIONS
{
    . = 0x200000;

    .entry :
    {
        *(.entry)
    }

    . = ALIGN(8);
    .text :
    {
        *(.text)
    }

    .rodata :
    {
        *(.rodata)
    }
    .data : 
    {
        *(.data)
    }
    .bss :
    {
        *(COMMON)
        *(.bss)
    }

    .asm :
    {
        *(.asm)
    }
}