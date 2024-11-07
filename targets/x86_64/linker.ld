ENTRY(bootloader_start)
SECTIONS
{
    . = 0xC0000000;

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