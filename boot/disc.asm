; disc.asm

; register settings for the int 0x13 BIOS disk read operation
; ah = 0x02
; al = number of sectors to read
; ch = cylinder number 
; cl = sector number
; dh = head number
; dl = drive number (physical drive from which BIOS will read data)

disc_load:
    push dx

    mov ah, 0x02    ; BIOS read sector function
    mov al, dh      ; number of sectors to be read (dh sectors)
    mov ch, 0x00    ; cylinder 0
    mov dh, 0x00    ; head 0 (0-indexed)
    mov cl, 0x02    ; start reading from sector number 2 (1-indexed)

    int 0x13

    jc disc_error

    pop dx
    cmp dh, al      ; al contains the number of sectors successfully read
    jne disc_error
    ret

disc_error:
    mov bx, DISC_ERROR_MESSAGE
    call print_string
    jmp $

DISC_ERROR_MESSAGE: db "Disk read error!", 0
