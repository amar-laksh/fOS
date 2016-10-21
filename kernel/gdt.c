#include <gdt.h>


static void write_tss(int32_t, uint16_t, uint32_t);
typedef struct tss_entry {
	uint32_t	prev_tss;
	uint32_t	esp0;
	uint32_t	ss0;
	uint32_t	esp1;
	uint32_t	ss1;
	uint32_t	esp2;
	uint32_t	ss2;
	uint32_t	cr3;
	uint32_t	eip;
	uint32_t	eflags;
	uint32_t	eax;
	uint32_t	ecx;
	uint32_t	edx;
	uint32_t	ebx;
	uint32_t	esp;
	uint32_t	ebp;
	uint32_t	esi;
	uint32_t	edi;
	uint32_t	es;
	uint32_t	cs;
	uint32_t	ss;
	uint32_t	ds;
	uint32_t	fs;
	uint32_t	gs;
	uint32_t	ldt;
	uint16_t	trap;
	uint16_t	iomap_base;
} __attribute__ ((packed)) tss_entry_t;



struct gdt_entry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

struct gdt_entry gdt[6];
struct gdt_ptr gp;
tss_entry_t tss_entry;


extern void gdt_flush();
extern void tss_flush();

/* Setup a descriptor in the Global Descriptor Table */
void gdt_set_gate(int num, uint64_t base, uint64_t limit,
		uint8_t access, uint8_t gran)
{
    /* Setup the descriptor base address */
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

void gdt_install() {
	/* GDT pointer and limits */
	gp.limit = (sizeof(struct gdt_entry) * 6) - 1;
	gp.base = (uint32_t)&gdt;
	/* NULL */
	gdt_set_gate(0, 0, 0, 0, 0);
	/* Code segment */
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	/* Data segment */
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	/* User code */
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	/* User data */
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
	write_tss(5, 0x10, 0x0);
	/* Go go go */
	gdt_flush();
	tss_flush();
	dprintf("GDT installed at:	0x%x\n",gp.base);
}

/**
 * Write a TSS (we only do this once)
 */
static void write_tss(
		int32_t num,
		uint16_t ss0,
		uint32_t esp0
		) {
	uintptr_t base  = (uintptr_t)&tss_entry;
	uintptr_t limit = base + sizeof(tss_entry);

	/* Add the TSS descriptor to the GDT */
	gdt_set_gate(num, base, limit, 0xE9, 0x00);

	memset(&tss_entry, 0x0, sizeof(tss_entry));

	tss_entry.ss0    = ss0;
	tss_entry.esp0   = esp0;
	/* Zero out the descriptors */
	tss_entry.cs     = 0x0b;
	tss_entry.ss     =
		tss_entry.ds =
		tss_entry.es =
		tss_entry.fs =
		tss_entry.gs = 0x13;
	tss_entry.iomap_base = sizeof(tss_entry);
}

/**
 * Set the kernel stack.
 *
 * @param stack Pointer to a the stack pointer for the kernel.
 */
void
set_kernel_stack(
		uintptr_t stack
		) {
	tss_entry.esp0 = stack;
}

