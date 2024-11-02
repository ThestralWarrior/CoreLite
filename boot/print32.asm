; print32.asm
; BIOS-less 32-bit string printing

[bits 32]

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

; string to be printed must be pointed to by ebx
print_string_32:
    pusha
    mov edx, VIDEO_MEMORY
proceed_string_32:
    mov al, [ebx]
    mov ah, WHITE_ON_BLACK
    cmp al, 0
    je exit_print32
    mov [edx], ax
    add ebx, 1
    add edx, 2
    jmp proceed_string_32
exit_print32:
    popa
    ret
