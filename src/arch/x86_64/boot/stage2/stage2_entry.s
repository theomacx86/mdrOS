[BITS 32]
section .entry

CODE_SEG equ 0x8
DATA_SEG equ 0x10

global _start
extern bootloader_start

_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp

    ; Enable A20 line
    in al, 0x92
    or al, 2
    out 0x92, al

    jmp bootloader_start