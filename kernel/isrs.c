#include <kernel/fos.h>

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr69();

static irq_handler_t isrs_routines[33] = { NULL };

void isrs_install_handler	(
							int isrs
							, irq_handler_t handler
							)
{
	isrs_routines[isrs] = handler;
}

void isrs_uninstall_handler	(
							int isrs
							)
{
	isrs_routines[isrs] = 0;
}


char *exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};



void isrs_install(){
	idt_set_gate(0,(unsigned)isr0,0x08,0x8E);
	idt_set_gate(1,(unsigned)isr1,0x08,0x8E);
	idt_set_gate(2,(unsigned)isr2,0x08,0x8E);
	idt_set_gate(3,(unsigned)isr3,0x08,0x8E);
	idt_set_gate(4,(unsigned)isr4,0x08,0x8E);
	idt_set_gate(5,(unsigned)isr5,0x08,0x8E);
	idt_set_gate(6,(unsigned)isr6,0x08,0x8E);
	idt_set_gate(7,(unsigned)isr7,0x08,0x8E);
	idt_set_gate(8,(unsigned)isr8,0x08,0x8E);
	idt_set_gate(9,(unsigned)isr9,0x08,0x8E);
	idt_set_gate(10,(unsigned)isr10,0x08,0x8E);
	idt_set_gate(11,(unsigned)isr11,0x08,0x8E);
	idt_set_gate(12,(unsigned)isr12,0x08,0x8E);
	idt_set_gate(13,(unsigned)isr13,0x08,0x8E);
	idt_set_gate(14,(unsigned)isr14,0x08,0x8E);
	idt_set_gate(15,(unsigned)isr15,0x08,0x8E);
	idt_set_gate(16,(unsigned)isr16,0x08,0x8E);
	idt_set_gate(17,(unsigned)isr17,0x08,0x8E);
	idt_set_gate(18,(unsigned)isr18,0x08,0x8E);
	idt_set_gate(19,(unsigned)isr19,0x08,0x8E);
	idt_set_gate(20,(unsigned)isr20,0x08,0x8E);
	idt_set_gate(21,(unsigned)isr21,0x08,0x8E);
	idt_set_gate(22,(unsigned)isr22,0x08,0x8E);
	idt_set_gate(23,(unsigned)isr23,0x08,0x8E);
	idt_set_gate(24,(unsigned)isr24,0x08,0x8E);
	idt_set_gate(25,(unsigned)isr25,0x08,0x8E);
	idt_set_gate(26,(unsigned)isr26,0x08,0x8E);
	idt_set_gate(27,(unsigned)isr27,0x08,0x8E);
	idt_set_gate(28,(unsigned)isr28,0x08,0x8E);
	idt_set_gate(29,(unsigned)isr29,0x08,0x8E);
	idt_set_gate(30,(unsigned)isr30,0x08,0x8E);
	idt_set_gate(31,(unsigned)isr31,0x08,0x8E);
	idt_set_gate(69,(unsigned)isr69,0x08,0x8E);
}

void syscall_handler(
					struct regs* r
					)
{
	sprintf("The parameters:\n%d\n%d\n%d\n%d\n"
		,r->eax
		,r->ebx
		,r->ecx
		,r->edx);
	if(r->eax == 11){
		sys_write(r->ebx, r->ecx, r->edx);
	}
}

void fault_handler	(
					struct regs *r
					)
{
	if (r->int_no == 69){
		IRQ_OFF;
		syscall_handler(r);
		IRQ_RES;
	}
	else{
		if (r->int_no == 8) {
			STOP;
		}
		if (r->int_no >= 32) {
			STOP;
		}
		IRQ_OFF;
		void (*handler)(struct regs *r);
		handler = (void*)isrs_routines[r->int_no];
		if (handler) {
			handler(r);
		} else {
			clear_screen();
			draw_str("Unhandled exception: ",0,1);
			draw_str(exception_messages[r->int_no],0,30);
			STOP;
		}
		IRQ_RES;
	}
	return;
}
