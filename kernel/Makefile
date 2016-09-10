INCLUDEDIR  = ../include
SYSDIR  = ../sys
STDLIBDIR = ../stdlib

INCLUDE    := $(shell find $(INCLUDEDIR) -name '*.c')
SYS	   := $(shell find $(SYSDIR) -name '*.c')
STDLIB     := $(shell find $(STDLIBDIR) -name '*.c')

OBJECTS = $(patsubst %.c,$(INCLUDEDIR)/%.o,$(INCLUDE)) $(patsubst %.c,$(SYSDIR)/%.o,$(SYS)) $(patsubst %.c,$(STDLIBDIR)/%.o,$(STDLIB)) boot.o fos.o gdt.o idt.o isrs.o irq.o timer.o

CC = i686-elf-gcc
CFLAGS = -I../include -std=gnu99 -ffreestanding -O2 -Wall -Wextra -c
LDFLAGS = -T linker.ld -o fos.elf -ffreestanding -O2 -nostdlib 
AS = i686-elf-as

all: kernel.elf

os.iso: kernel.elf
	mkdir -p iso/boot/grub
	cp fos.elf iso/boot/fos.elf
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o fos.iso iso
kernel.elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -lgcc

run: os.iso
	qemu-system-i386 -cdrom fos.iso

debug: os.iso
	objcopy --only-keep-debug fos.elf fos.sym
	qemu-system-i386 -s -kernel fos.elf

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.s
	$(AS) $< -o $@

clean:
	rm -rf *. ../sys/*.o ../stdlib/*.o ../include/*.o *.o ./boot/fos.bin ./boot/fos.elf ./*.iso ./*.elf
