# Constants declared for the Multi-boot specification header. 
.set ALIGN,    1<<0             				# align loaded modules on page boundaries
.set MEMINFO,  1<<1             				# provide memory map
.set VIDEOTABLE, 1<<2							# provide video table information
.set FLAGS,    ALIGN | MEMINFO | VIDEOTABLE  	# this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       				# 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) 				# checksum of above, to prove we are multiboot

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
.long 0, 0, 0, 0, 0
.long 1 										#set graphics mode
.long 0, 0, 32									#width, height, depth

.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384#2*1024*1024#16384 # 16 KiB
stack_top:

.section .text

.global _start


.type idt_load, @function
.type gdt_flush, @function
.type _start, @function
_start:
	cli
	mov $stack_top, %esp
	pushl %eax
	pushl %ebx
	call kmain
1:
	jmp 1b

# Flushes the old gdt set by GRUB

.global gdt_flush
.extern gp 

gdt_flush:
	lgdt gp
	mov $0x10,%ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	jmp $0x08,$flush2

flush2:
	ret

.global idt_load
.extern idtp
idt_load:
	lidt idtp
	ret

.macro ISR_NOERR x
	.global isr\x
	isr\x:
		cli
		pushl $0
		pushl $\x
		jmp isr_common
.endm

.macro ISR_ERR x
	.global  isr\x
	isr\x:
		cli
		pushl $\x
		jmp isr_common
.endm

.macro ISR_SYSCALL x
	.global isr\x
	isr\x:
		cli
		pushl $0
		pushl $\x
		jmp isr_common
.endm

.macro IRQ_ENTRY x, y
	.global  irq\x
	irq\x:
		cli
		pushl $0
		pushl $\y
		jmp irq_common
.endm

# All the ISR Exception functions
ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR   8
ISR_NOERR 9
ISR_ERR   10
ISR_NOERR 11
ISR_ERR   12
ISR_ERR   13
ISR_ERR   14
ISR_NOERR 15
ISR_NOERR 16
ISR_NOERR 17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31
ISR_SYSCALL 69

.extern fault_handler
.extern syscall


isr_common:
	pusha
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs

	movw $0x10, %ax 
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movl %esp, %eax
	pushl %eax
	mov $fault_handler, %eax
	call *%eax 
	pop %eax

	popl %gs
	popl %fs
	popl %es
	popl %ds
	popa
	addl $8, %esp   
	iret          


IRQ_ENTRY 0, 32
IRQ_ENTRY 1, 33
IRQ_ENTRY 2, 34
IRQ_ENTRY 3, 35
IRQ_ENTRY 4, 36
IRQ_ENTRY 5, 37
IRQ_ENTRY 6, 38
IRQ_ENTRY 7, 39
IRQ_ENTRY 8, 40
IRQ_ENTRY 9, 41
IRQ_ENTRY 10, 42
IRQ_ENTRY 11, 43
IRQ_ENTRY 12, 44
IRQ_ENTRY 13, 45
IRQ_ENTRY 14, 46
IRQ_ENTRY 15, 47

.extern irq_handler

irq_common:
    pusha
    push %ds
    push %es
    push %fs
    push %gs

    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movl %esp, %eax

    pushl %eax
    movl $irq_handler, %eax
    call *%eax
    popl %eax

    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    addl $8, %esp
    iret

.global tss_flush
tss_flush:
	mov $0x2B,%ax
	ltr %ax
	ret

.size _start, . - _start
