; GDT
gdt_start:

; mandatory null descriptor
; starts at 0x0
gdt_null:
    dd 0x0    ; low 32 bits of the null descriptor
    dd 0x0    ; high 32 bits of the null descriptor

; code segment descriptor
; starts at 0x8
gdt_code:
    ; 1st flags = (present)1 (privilege)00 (descriptor type)1 -> 1001 b
    ; type flags = (code)1 (conforming)0 (readable)1 (accessed)0 -> 1010 b
    ; 2nd flags = (granularity)1 (32 - bit default)1 (64 - bit seg)0 (AVL)0 -> 1100 b
    dw 0xffff       ; segment limit (bits 0-15)
    dw 0x0          ; segment base address (bits 0-15)
    db 0x0          ; segment base address (bits 16-23)
    db 10011010b    ; 1st flags, type flags
    db 11001111b    ; 2nd flags, segment limit (bits 16 - 19)
    db 0x0          ; segment base address (bits 24-31)

; data segment descriptor
; starts at 0x10
gdt_data:
    ; 1st flags = same as gdt_code
    ; type flags = (code)0 (expand down)0 (writable)1 (accessed)0 -> 0010 b
    ; 2nd flags = same as gdt_code
    dw 0xffff       ; segment limit (bits 0-15)
    dw 0x0          ; segment base address (bits 0-15)
    db 0x0          ; segment base address (bits 16-23)
    db 10010010b    ; 1st flags, type flags
    db 11001111b    ; 2nd flags, segment limit (bits 16 - 19)
    db 0x0          ; segment base address (bits 24-31)

gdt_end:

gdt_descriptor: ; GDT descriptor
    dw gdt_end - gdt_start - 1
    dd gdt_start

; constants for the GDT segment descriptor offsets
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start