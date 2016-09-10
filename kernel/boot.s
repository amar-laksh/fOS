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

.type idt_load, @function
.type gdt_flush, @function
.type _start, @function
_start:
	mov $stack_top, %esp
	call kmain
1:
	cli
	jmp 1b

# Flushes the old gdt set by GRUB

.global gdt_flush
.extern gp 
gdt_flush:
	lgdt gp
	mov %ax, 0x10
	mov %ds, %ax
	mov %es, %ax
	mov %fs, %ax
	mov %gs, %ax
	mov %ss, %ax
	jmp $0x08,$flush2

flush2:
	ret

.global idt_load
.extern idtp
idt_load:
	lidt idtp
	ret

# All the ISR Exception functions
.global isr0
.global isr1
.global isr2
.global isr3
.global isr4
.global isr5
.global isr6
.global isr7
.global isr8
.global isr9
.global isr10
.global isr11
.global isr12
.global isr13
.global isr14
.global isr15
.global isr16
.global isr17
.global isr18
.global isr19
.global isr20
.global isr21
.global isr22
.global isr23
.global isr24
.global isr25
.global isr26
.global isr27
.global isr28
.global isr29
.global isr30
.global isr31


#	0: Divide By Zero Exception
isr0:
	cli
	push 0x0
	push 0x0
	jmp isr_common

#	1: Debug Exception
isr1:
	cli
	push 0x0
	push 0x1
	jmp isr_common

#	2: Non-Maskable Interrupt Exception
isr2:
	cli
	push 0x0
	push 0x2
	jmp isr_common

#	3: Breakpoint Exception
isr3:
	cli
	push 0x0
	push 0x3
	jmp isr_common

#	4: Overflow Exception
isr4:
	cli
	push 0x0
	push 0x4
	jmp isr_common

#	5: Out-Of-Bounds Exception
isr5:
	cli
	push 0x0
	push 0x5
	jmp isr_common

#	6: Invalid Opcode
isr6:
	cli
	push 0x0
	push 0x6
	jmp isr_common

#	7: No Co-processor Exception(No Floating-point)
isr7:
	cli
	push 0x0
	push 0x7
	jmp isr_common

#	8: Double Fault Exception
isr8:
	cli
	push 0x8
	jmp isr_common

#	9: Co-processor Segment Overrun Exception
isr9:
	cli
	push 0x0
	push 0x9
	jmp isr_common

#	10: Invalid TSS Exception
isr10:
	cli
	push 0xA
	jmp isr_common

#	11: Segment Not Present Exception
isr11:
	cli
	push 0xB
	jmp isr_common

#	12: Stack-Segment Fault
isr12:
	cli
	push 0xC
	jmp isr_common

#	13: General Protection Fault
isr13:
	cli
	push 0xD
	jmp isr_common

#	14: Page Fault
isr14:
	cli
	push 0xE
	jmp isr_common

#	15: INTEL RESERVED 
isr15:
	cli
	push 0x0
	push 0xF
	jmp isr_common

#	16: x87 FPU Math Fault
isr16:
	cli
	push 0x0
	push 0x10
	jmp isr_common

#	17: Alignment Check Exception
isr17:
	cli
	push 0x11
	jmp isr_common

#	18: Machine Check Exception
isr18:
	cli
	push 0x0
	push 0x12
	jmp isr_common

#	19: SIMD Floating-Point Exception
isr19:
	cli
	push 0x0
	push 0x13
	jmp isr_common

#	20: Virtualization Exception
isr20:
	cli
	push 0x0
	push 0x14
	jmp isr_common

#	21: INTEL RESERVED
isr21:
	cli
	push 0x0
	push 0x15
	jmp isr_common

#	22: INTEL RESERVED
isr22:
	cli
	push 0x0
	push 0x16
	jmp isr_common

#	23: INTEL RESERVED
isr23:
	cli
	push 0x0
	push 0x17
	jmp isr_common

#	24: INTEL RESERVED
isr24:
	cli
	push 0x0
	push 0x18
	jmp isr_common

#	25: INTEL RESERVED
isr25:
	cli
	push 0x0
	push 0x19
	jmp isr_common

#	26: INTEL RESERVED
isr26:
	cli
	push 0x0
	push 0x1A
	jmp isr_common


#	27: INTEL RESERVED
isr27:
	cli
	push 0x0
	push 0x1B
	jmp isr_common

#	28: INTEL RESERVED
isr28:
	cli
	push 0x0
	push 0x1C
	jmp isr_common

#	29: INTEL RESERVED
isr29:
	cli
	push 0x0
	push 0x1D
	jmp isr_common

#	30: INTEL RESERVED
isr30:
	cli
	push 0x0
	push 0x1E
	jmp isr_common

#	31: INTEL RESERVED
isr31:
	cli
	push 0x0
	push 0x1F
	jmp isr_common


.extern fault_handler

isr_common:
	pusha
	push %ds
	push %es
	push %fs
	push %gs
	mov %ax, 0x10   # Load the Kernel Data Segment descriptor!
	mov %ds, %ax
	mov %es, %ax
	mov %fs, %ax
	mov %gs, %ax
	mov %eax, %esp   # Push us the stack
	push %eax
	mov %eax, fault_handler
	call *%eax       # A special call, preserves the 'eip' register
	pop %eax
	pop %gs
	pop %fs
	pop %es
	pop %ds
	popa
	add %esp, 8     # Cleans up the pushed error code and pushed ISR number
	iret           # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!


.global irq0
.global irq1
.global irq2
.global irq3
.global irq4
.global irq5
.global irq6
.global irq7
.global irq8
.global irq9
.global irq10
.global irq11
.global irq12
.global irq13
.global irq14
.global irq15


# 32: IRQ0
irq0:
    cli
    push 0x0
    push 20
    jmp irq_common_stub

# 33: IRQ1
irq1:
    cli
    push 0x0
    push 0x21
    jmp irq_common_stub

# 34: IRQ2
irq2:
    cli
    push 0x0
    push 0x22
    jmp irq_common_stub

# 35: IRQ3
irq3:
    cli
    push 0x0
    push 0x23
    jmp irq_common_stub

# 36: IRQ4
irq4:
    cli
    push 0x0
    push 0x24
    jmp irq_common_stub

# 37: IRQ5
irq5:
    cli
    push 0x0
    push 0x25
    jmp irq_common_stub

# 38: IRQ6
irq6:
    cli
    push 0x0
    push 0x26
    jmp irq_common_stub

# 39: IRQ7
irq7:
    cli
    push 0x0
    push 0x27
    jmp irq_common_stub

# 40: IRQ8
irq8:
    cli
    push 0x0
    push 0x28
    jmp irq_common_stub

# 41: IRQ9
irq9:
    cli
    push 0x0
    push 0x29
    jmp irq_common_stub

# 42: IRQ10
irq10:
    cli
    push 0x0
    push 0x2A
    jmp irq_common_stub

# 43: IRQ11
irq11:
    cli
    push 0x0
    push 0x2B
    jmp irq_common_stub

# 44: IRQ12
irq12:
    cli
    push 0x0
    push 0x2C
    jmp irq_common_stub

# 45: IRQ13
irq13:
    cli
    push 0x0
    push 0x2D
    jmp irq_common_stub

# 46: IRQ14
irq14:
    cli
    push 0x0
    push 0x2E
    jmp irq_common_stub

# 47: IRQ15
irq15:
    cli
    push 0x0
    push 0x2F
    jmp irq_common_stub

.extern irq_handler

irq_common_stub:
    pusha
    push %ds
    push %es
    push %fs
    push %gs

    mov %ax, 0x10
    mov %ds, %ax
    mov %es, %ax
    mov %fs, %ax
    mov %gs, %ax
    mov %eax, %esp

    push %eax
    mov %eax, irq_handler
    call *%eax
    pop %eax

    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add %esp, 8
    iret





.size _start, . - _start
