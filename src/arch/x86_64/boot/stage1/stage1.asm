; LolOS bootloader code
; For MBR/Legacy devices

ORG 0x7c00
BITS 16         ; Real mode, 16 bit

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

_start:
    jmp short start
    nop

times 33 db 0       ;BIOS Parameter block, avoid some BIOSes overwriting code

start:
    jmp 0:setup_registers

setup_registers:
    cli
    mov ax, 0   ;Sets segments to 0x7c0 * 16 
    mov ds, ax
    mov es, ax
    mov ss, ax      ;Stack segment is zero
    mov sp, 0x7c00  ;Stack is right below our bootloader
    sti

.load_protected:
    call load_memory_map
    cli
    lgdt[gdt_descriptor]
    mov eax, cr0
    or eax, 0x1     ;Enable protected
    mov cr0, eax
    jmp CODE_SEG:load32

; This 16 bit code will load the memory map into memory
mmap_lenght_ptr equ 0x8000
mmap_ptr equ 0x8004        
load_memory_map:
    mov eax, 0xE820         ;E820
    xor ebx, ebx            ;ebx = 0
    xor ebp, ebp            ;Entry count
    mov edx, 0x534D4150     ;Magic ?
    mov ecx, 24             
    mov di, mmap_ptr         ;Destination buffer (ES =0, we will store the number of entries at 0x7E04)
    int 0x15
    cmp eax, 0x534D4150     ;If the call failed something catastrophic might have happened, better crash
    je .loop
    jmp $                   ;If the magic value is not present, deadloop
.loop:
    inc ebp
    cmp ebx, 0
    je .return
    mov eax, 0xE820         
    mov ecx, 24             
    add di, cx
    int 0x15
    jmp .loop
.return:
    mov di, mmap_lenght_ptr
    mov [es:di], ebp        ;Write number of entries found before the entry list
    ret


gdt_start:
gdt_null:
    dq 0x0
gdt_code:     ;Code segment (CS)
    dw 0xffff ;Segment limit
    dw 0x0000 ;Base
    db 0      ; Base (16 to 23)
    db 0x9A   ;Access byte, present, system, executable, accessed
    db 0xCF   ; Limit byte + flag (page granularity)
    db 0
gdt_data:
    dw 0xffff ;Segment limit
    dw 0x0000 ;Base
    db 0      ; Base (16 to 23)
    db 0x92   ;Access byte, present, systemn accessed
    db 0xCF   ; Limit byte + flag (page granularity)
    db 0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start -1
    dd gdt_start 

[BITS 32]
load32:
    mov eax, 1
    mov ecx, 100
    mov edi, 0x0100000
    call ata_lba_read
    jmp CODE_SEG:0x0100000

;   Params
;   EAX: LBA to read, ECX: number of sectors to read, EDI: destination
;
ata_lba_read:
ata_lba_read:
    mov ebx, eax, ; Backup the LBA
    ; Send the highest 8 bits of the lba to hard disk controller
    shr eax, 24
    or eax, 0xE0 ; Select the  master drive
    mov dx, 0x1F6
    out dx, al
    ; Finished sending the highest 8 bits of the lba
    ; Send the total sectors to read
    mov eax, ecx
    mov dx, 0x1F2
    out dx, al
    ; Finished sending the total sectors to read
    ; Send more bits of the LBA
    mov eax, ebx ; Restore the backup LBA
    mov dx, 0x1F3
    out dx, al
    ; Finished sending more bits of the LBA
    ; Send more bits of the LBA
    mov dx, 0x1F4
    mov eax, ebx ; Restore the backup LBA
    shr eax, 8
    out dx, al
    ; Finished sending more bits of the LBA
    ; Send upper 16 bits of the LBA
    mov dx, 0x1F5
    mov eax, ebx ; Restore the backup LBA
    shr eax, 16
    out dx, al
    ; Finished sending upper 16 bits of the LBA
    mov dx, 0x1f7
    mov al, 0x20
    out dx, al

    ; Read all sectors into memory
.next_sector:
    push ecx

; Checking if we need to read
.try_again:
    mov dx, 0x1f7
    in al, dx
    test al, 8
    jz .try_again

; We need to read 256 words at a time
    mov ecx, 256
    mov dx, 0x1F0
    rep insw
    pop ecx
    loop .next_sector
    ; End of reading sectors into memory
    ret




times 446 - ($ - $$) db 0 ; 446 bytes minus current location minus start