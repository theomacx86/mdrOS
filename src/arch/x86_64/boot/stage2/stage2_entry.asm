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

switch_to_long:
    mov eax, cr0
    and eax, 0x7FFFFFFF ;Clear paging bit
    mov cr0, eax

    mov eax, cr4
    or eax, 0x20        ;Enable PAE
    mov cr4, eax


init_pml4:
    mov eax, pdpt_table
    or eax, 0x3         
    mov [pml4_table], eax
    

init_pdpt:
    mov eax, pd_table
    or eax, 0x3
    mov [pdpt_table], eax

    mov ecx, 1          ;Loop counter we need two pages
    xor ebx, ebx        ;Address starts = 0
    mov edx, 0          ;Entry counter

init_pd:
    mov eax, ebx
    or eax, 0b10000011              ;Set 2mb pages
    mov [pd_table + edx * 8], eax   ;ptr + i*8
    add ebx, 0x200000               ;Each page is 2mb, increment current addr
    add edx, 1                      ; ++i
    loop init_pd
    
enable_long:
    mov eax, pml4_table
    mov cr3, eax                    ;Load PML4 in CR3
    mov ecx, 0xC0000080             ;Load MSR 0xC0000080 in eax
    rdmsr   
    or eax, 1 << 8                  ;Long mode bit
    wrmsr
    
    mov eax, cr0                    ; enable paging
    or eax, 1 << 31
    or eax, 1 << 16
    mov cr0, eax
    jmp $

    lgdt [gdt64.pointer]            ;Load the GDT
    mov ax, gdt64.data
    mov ss, ax
    mov ds, ax
    mov es, ax
    
    jmp gdt64.code:long_mode_start

[BITS 64]
long_mode_start:
    jmp $
    jmp bootloader_start

section .bss
align 4096
pml4_table resb 4096
pdpt_table resb 4096
pd_table resb 4096


section .rodata
align 4096
gdt64:
    dq 0
.code: equ $ - gdt64
    dq (1<<44) | (1<<47) | (1<<41) | (1<<43) | (1<<53)
.data: equ $ - gdt64
    dq (1<<44) | (1<<47) | (1<<41)
.pointer:
    dw .pointer - gdt64 - 1
    dq gdt64