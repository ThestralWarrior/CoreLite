[org 0x7c00]
KERNEL_OFFSET equ 0x1000
    mov [BOOT_DRIVE], dl    ; BIOS stores boot drive in dl

    mov bp, 0x9000          ; stack set-up
    mov sp, bp

    mov bx, MSG_REAL_MODE   ; announce that we are starting booting from 16-bit real mode
    call print_string

    call load_kernel        ; load kernel

    call switch_to_pm       ; we never return from this call

    jmp $

    %include 'print.asm'
    %include 'disc.asm'
    %include 'gdt.asm'
    %include 'print32.asm'
    %include 'switch.asm'

[bits 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print_string

    mov bx, KERNEL_OFFSET   ; load the kernel to ES:BX
    mov dh, 15              ; load the first 15 sectors from the boot disk (excluding it)
    mov dl, [BOOT_DRIVE]    ; physical device from which BIOS will read data
    call disc_load

    ret

[bits 32]
begin_pm:
    mov ebx, MSG_PROTECTED_MODE ; announce that we are now in 32-bit protected mode
    call print_string_32

    call KERNEL_OFFSET          ; jump to the address where the kernel was loaded

    jmp $
    
BOOT_DRIVE: db 0
MSG_REAL_MODE: db "Started in 16-bit Real Mode", 0
MSG_PROTECTED_MODE: db "Successfully landed in 32-bit Protected Mode", 0
MSG_LOAD_KERNEL: db "Loading kernel into memory.", 0
NEWLINE: db 0x0D, 0x0A, 0

    times 510-($-$$) db 0
    dw 0xaa55