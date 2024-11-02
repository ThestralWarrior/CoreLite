# Variables
BOOT_DIR      := boot
KERNEL_DIR    := kernel
DRIVER_DIR	  := drivers
CPU_DIR		  := cpu
BUILD_DIR     := build

BOOT_ASM      := $(BOOT_DIR)/boot.asm
KERNEL_ASM    := $(BOOT_DIR)/kernel_entry.asm
KERNEL_SRCS   := $(wildcard $(KERNEL_DIR)/*.c)
CPU_SRCS	  := $(wildcard $(CPU_DIR)/*.c)
CPU_ASM_SRCS := $(wildcard $(CPU_DIR)/*.asm)
DRIVER_SRCS   := $(wildcard $(DRIVER_DIR)/*.c)

BOOT_BIN      := $(BUILD_DIR)/boot.bin
KERNEL_BIN    := $(BUILD_DIR)/kernel.bin
KERNEL_OBJS   := $(patsubst $(KERNEL_DIR)/%.c,$(BUILD_DIR)/%.o,$(KERNEL_SRCS)) $(patsubst $(DRIVER_DIR)/%.c,$(BUILD_DIR)/%.o,$(DRIVER_SRCS)) $(patsubst $(CPU_DIR)/%.c,$(BUILD_DIR)/%.o,$(CPU_SRCS)) $(patsubst $(CPU_DIR)/%.asm,$(BUILD_DIR)/%.o,$(CPU_ASM_SRCS)) $(BUILD_DIR)/kernel_entry.o
OS_IMAGE      := $(BUILD_DIR)/os-image

# Tools
NASM          := nasm
GCC           := i686-elf-gcc
LD            := i686-elf-ld
QEMU          := qemu-system-x86_64
NDISASM       := ndisasm

# Flags
GCC_FLAGS     := -ffreestanding -c
NASM_FLAGS_ELF:= -f elf
NASM_FLAGS_BIN:= -f bin -I $(BOOT_DIR)
LD_FLAGS      := -Ttext 0x1000 --oformat binary

# Targets
all: $(OS_IMAGE)

run: all
	$(QEMU) -fda $(OS_IMAGE)

$(OS_IMAGE): $(BOOT_BIN) $(KERNEL_BIN)
	cat $^ > $@

$(KERNEL_BIN): $(KERNEL_OBJS)
	$(LD) -o $@ $(LD_FLAGS) $^

$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c
	$(GCC) $(GCC_FLAGS) $< -o $@

$(BUILD_DIR)/%.o: $(DRIVER_DIR)/%.c
	$(GCC) $(GCC_FLAGS) $< -o $@

$(BUILD_DIR)/%.o: $(CPU_DIR)/%.c
	$(GCC) $(GCC_FLAGS) $< -o $@

$(BUILD_DIR)/%.o: $(CPU_DIR)/%.asm
	$(NASM) $(NASM_FLAGS_ELF) $< -o $@

$(BUILD_DIR)/kernel_entry.o: $(KERNEL_ASM)
	$(NASM) $(NASM_FLAGS_ELF) $< -o $@

$(BOOT_BIN): $(BOOT_ASM)
	$(NASM) $(NASM_FLAGS_BIN) $< -o $@

clean:
	rm -rf $(BUILD_DIR)/*

misc/kernel.dis: $(KERNEL_BIN)
	$(NDISASM) -b 32 $< > $@
