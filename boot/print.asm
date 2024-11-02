; printfncs.asm

; register settings for the int 0x10 BIOS video services
; ah = 0x0E  ; Teletype output (display character)
; al = ASCII value of the character to display

; prints strings specified in bx
print_string:
    mov si, bx
proceed_string:
    lodsb           ; loads character at [si] to al and increments si
    cmp al, 0
    je exit_print
    mov ah, 0x0e
    int 0x10
    jmp proceed_string

; prints 16-bit hexadecimal number in dx
print_hex:
    mov si, HEXOUT
    add si, 2
    mov ax, 0xF000
    mov cl, 12
proceed_hex:
    mov bx, dx
    and bx, ax
    shr bx, cl
    shr ax, 4
    cmp bx, 9
    jle is_digit
    add bx, 0x37
    jmp write
is_digit:
    add bx, 0x30
write:
    mov [si], bx
    add si, 1
    cmp cl, 0
    je display
    sub cl, 4
    jmp proceed_hex
display:
    mov bx, HEXOUT
    call print_string
    jmp exit_print

exit_print:
    ret

HEXOUT: db '0x0000', 0







