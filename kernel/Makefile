INCLUDEDIR  = ../include
SYSDIR  = ../sys
STDLIBDIR = ../stdlib
DRIVERSDIR = ../drivers
KERNELDIR = ./
MEMDIR = ../mm
TESTDIR = ../test

INCLUDE    := $(shell find $(INCLUDEDIR) -name '*.c')
SYS	   := $(shell find $(SYSDIR) -name '*.c')
STDLIB     := $(shell find $(STDLIBDIR) -name '*.c')
DRIVERS    := $(shell find $(DRIVERSDIR) -name '*.c')
KERNEL     := $(shell find $(KERNELDIR) -name '*.c')
MEM 	   := $(shell find $(MEMDIR) -name '*.c')
TEST	   := $(shell find $(TESTDIR) -name '*.c')

OBJECTS = $(patsubst %.c,$(INCLUDEDIR)/%.o,$(INCLUDE)) $(patsubst %.c,$(SYSDIR)/%.o,$(SYS)) $(patsubst %.c,$(STDLIBDIR)/%.o,$(STDLIB)) $(patsubst %.c,$(DRIVERSDIR)/%.o,$(DRIVERS)) $(patsubst %.c,$(KERNELDIR)/%.o,$(KERNEL)) $(patsubst %.c,$(MEMDIR)/%.o,$(MEM)) boot.o

CC =  /root/cross/bin/i686-elf-gcc
CFLAGS = -I../include -std=gnu99 -ffreestanding -O2 -Wall -Wextra -c
LDFLAGS = -T linker.ld -o fos.elf -ffreestanding -O2 -nostdlib 
AS =  /root/cross/bin/i686-elf-as

all: kernel.elf

test:
	 $(CC) $(TEST) -std=gnu99  -O2 -Wall -Wextra -lgcc

clean:
	rm -rf *. ../drivers/*.o ../mm/*.o ../sys/*.o ../stdlib/*.o ../include/*.o *.o ./boot/fos.bin ./boot/fos.elf ./*.iso ./*.elf

os.iso: kernel.elf
	mkdir -p iso/boot/grub
	cp fos.elf iso/boot/fos.elf
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o fos.iso iso
	rm -rf *. ../drivers/*.o ../mm/*.o ../sys/*.o ../stdlib/*.o ../include/*.o *.o ./boot/fos.bin ./boot/fos.elf ./*.elf


bootable:
	genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o fos_bootable.iso iso
kernel.elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -lgcc

run: os.iso
	qemu-system-i386 -m 2048 -D /tmp/current.log -display sdl -serial stdio -cdrom fos.iso

debug: os.iso
	qemu-system-i386 -s -S fos.iso

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.s
	$(AS) $< -o $@


