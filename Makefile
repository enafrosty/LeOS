
CC = i686-elf-gcc
AS = nasm
LD = i686-elf-ld

CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
ASFLAGS = -f elf32
BOOT_ASFLAGS = -f bin
LDFLAGS = -T linker.ld -O2 -nostdlib

QEMU = qemu-system-i386
QEMU_FLAGS = -cdrom $(ISO) \
			 -m 128M \
			 -rtc base=localtime \
			 -vga std \
			 -monitor stdio \
			 -boot d

KERNEL_SOURCES = kernel/kernel.c
GUI_SOURCES = gui/window.c gui/desktop.c
FS_SOURCES = fs/fs.c
LIB_SOURCES = lib/string.c
BOOT_SOURCE = boot/boot.asm

KERNEL_OBJECTS = $(KERNEL_SOURCES:.c=.o)
GUI_OBJECTS = $(GUI_SOURCES:.c=.o)
FS_OBJECTS = $(FS_SOURCES:.c=.o)
LIB_OBJECTS = $(LIB_SOURCES:.c=.o)
BOOT_BINARY = boot/boot.bin

KERNEL = kernel.bin
ISO = leos.iso

.PHONY: all clean run run-debug

all: $(ISO)

$(BOOT_BINARY): $(BOOT_SOURCE)
	$(AS) $(BOOT_ASFLAGS) $< -o $@

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) -I./lib

$(KERNEL): $(KERNEL_OBJECTS) $(GUI_OBJECTS) $(FS_OBJECTS) $(LIB_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(ISO): $(KERNEL)
	mkdir -p iso/boot/grub
	cp $(KERNEL) iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o $@ iso

clean:
	rm -f $(KERNEL_OBJECTS) $(GUI_OBJECTS) $(FS_OBJECTS) $(LIB_OBJECTS)
	rm -f $(BOOT_BINARY) $(KERNEL) $(ISO)
	rm -rf iso

run: $(ISO)
	$(QEMU) $(QEMU_FLAGS)

run-debug: $(ISO)
	$(QEMU) $(QEMU_FLAGS) -s -S 