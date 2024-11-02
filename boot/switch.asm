[bits 16]
switch_to_pm:
    cli                     ; disable interrupts
    lgdt [gdt_descriptor]   ; load the gdt descriptor
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax            ; enable 32-bit protected mode
    jmp CODE_SEG:init_pm    ; set cs segment register to CODE_SEG and jump to init_pm offset from the base address specified by the code segment,
                            ; this is a far jump,
                            ; forces the CPU to flush its cache of real mode instructions before 32-bit protected mode instructions can be executed
[bits 32]
init_pm:
    ; set the segment registers to DATA_SEG
    mov ax, DATA_SEG
    mov es, ax
    mov ds, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ; initialize the stack
    mov ebp, 0x90000    ; stack position is right at the top of the free space
    mov esp, ebp

    call begin_pm

