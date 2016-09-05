# Constants declared for the Multi-boot specification header. 
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .text
.global _start
.global gdt_flush
.extern gp
.type _gdt_flush, @function
.type _start, @function
_start:
	mov $stack_top, %esp

	call kmain
	gdt_flush:
    		lgdt gp
		#gdt_reg:
		#	.word 0x0800
		#	.long 0x00000800
    		mov %ax, 0x10
    		mov %ds, %ax
    		mov %es, %ax
    		mov %fs, %ax
    		mov %gs, %ax
    		mov %ss, %ax
    		jmp $0x08,$flush2
flush2:
    ret
	cli
1:	hlt
	jmp 1b

.size _start, . - _start
