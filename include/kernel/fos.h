#ifndef FOS_H
#define FOS_H
#include <stdint.h>
#include <stddef.h>

#if !defined(__cplusplus)
#include <stdbool.h>
#endif

 
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
#if !defined(__i386__)
#error "This kernel needs to be compiled with a ix86-elf compiler"
#endif


#include <sys/va_list.h>





char* ramdisk;
uint32_t ramdisk_top;



//------------------------------------------------STRNG.H

int32_t strlen(const char str[]);

void itoa(uint64_t i,uint32_t base, char* buf);

int8_t equals(char* first, char* second);

char *str_tok(char *str, const char *delim);

//------------------------------------------------CONV.H

void itoi(uint64_t i,uint32_t base, uint64_t* buf);

int atoi(char str[]);

char* join(char* buff[5]);

int pow(int base, int exponent);



//------------------------------------------------MULTIBOOT.H

/* multiboot.h - the header for Multiboot */
/* Copyright (C) 1999, 2001  Free Software Foundation, Inc.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

/* Macros. */

/* The magic number for the Multiboot header. */
#define MULTIBOOT_HEADER_MAGIC          0x1BADB002

/* The flags for the Multiboot header. */
#ifdef __ELF__
# define MULTIBOOT_HEADER_FLAGS         0x00000003
#else
# define MULTIBOOT_HEADER_FLAGS         0x00010003
#endif

/* The magic number passed by a Multiboot-compliant boot loader. */
#define MULTIBOOT_BOOTLOADER_MAGIC      0x2BADB002

/* The size of our stack (16KiB). */
#define STACK_SIZE                      0x4000

/* C symbol format. HAVE_ASM_USCORE is defined by configure. */
#ifdef HAVE_ASM_USCORE
# define EXT_C(sym)                     _ ## sym
#else
# define EXT_C(sym)                     sym
#endif

#ifndef ASM
/* Do not include here in boot.S. */

/* Types. */

/* The Multiboot header. */
typedef struct multiboot_header
{
unsigned long magic;
unsigned long flags;
unsigned long checksum;
unsigned long header_addr;
unsigned long load_addr;
unsigned long load_end_addr;
unsigned long bss_end_addr;
unsigned long entry_addr;
} multiboot_header_t;

/* The symbol table for a.out. */
typedef struct aout_symbol_table
{
unsigned long tabsize;
unsigned long strsize;
unsigned long addr;
unsigned long reserved;
} aout_symbol_table_t;

/* The section header table for ELF. */
typedef struct elf_section_header_table
{
unsigned long num;
unsigned long size;
unsigned long addr;
unsigned long shndx;
} elf_section_header_table_t;

/* The Multiboot information. */
typedef struct multiboot_info
{
unsigned long flags;
unsigned long mem_lower;
unsigned long mem_upper;
unsigned long boot_device;
unsigned long cmdline;
unsigned long mods_count;
unsigned long mods_addr;
union
{
 aout_symbol_table_t aout_sym;
 elf_section_header_table_t elf_sec;
} u;
unsigned long mmap_length;
unsigned long mmap_addr;
} multiboot_info_t;

/* The module structure. */
typedef struct module
{
unsigned long mod_start;
unsigned long mod_end;
unsigned long string;
unsigned long reserved;
} module_t;

/* The memory map. Be careful that the offset 0 is base_addr_low
but no size. */
typedef struct memory_map
{
unsigned long size;
unsigned long base_addr_low;
unsigned long base_addr_high;
unsigned long length_low;
unsigned long length_high;
unsigned long type;
} memory_map_t;

unsigned long long multiboot_check(multiboot_info_t* mbd, unsigned int magic);
#endif /* ! ASM */



//------------------------------------------------MALLOC.H

typedef struct {
	uint8_t status;
	uint32_t size;
} alloc_t;

 void mm_init(uint32_t end, unsigned long total_mem);
 void mm_print_out();

char* malloc(size_t size);

void free(void *mem);

void * memcpy(void * restrict dest, const void * restrict src, size_t count);

void *memset(void *dest, char val, size_t count);

 //void* memset16 (void *ptr, uint16_t value, size_t num);

//------------------------------------------------GDT.H

void gdt_set_gate(int num, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran);

void gdt_install();

//------------------------------------------------IDT.H

void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags);

void idt_install();

//------------------------------------------------ISRS.H

void isrs_install();

//------------------------------------------------IRQ.H

typedef void (*irq_handler_t) (struct regs*);

void irq_install_handler(int irq, irq_handler_t handler);

void irq_uninstall_handler(int);

void irq_install();

void irq_ack(int irq_no);

//------------------------------------------------TIMER.H
#define TIMER_IRQ 0

void timer_install();

void delay(int t);

void timer_phase(int hz);

void timer_handler(irq_handler_t * handler);

//------------------------------------------------POLL.H

uint64_t rdtsc();

uint64_t get_control_register(int number);

void poll_init();

//------------------------------------------------VGA.H
#define	VIDMEM 0x000B8000
#define VIDMEM_END 0x00BFFFF
#define VIDMEM_SIZE 32767
#define MAX_ROWS 24//767//639//24
#define MAX_COLUMNS 79//1023//479//79
#define ERROR_CODE -12321
#define PASS_CODE 32123

struct {
	char* vga_buffer;
	int vga_offset;
} vga_fb;
enum vga_color
{
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};


int32_t get_row(uint32_t p);

int32_t get_column(uint32_t p);

int32_t get_point(uint32_t r, uint32_t c);

void clear_screen();

char get_char_cell(int r, int c);

void draw_char(uint32_t p, char ch, uint8_t fg, uint8_t bg);

int32_t draw_str(char string[], int32_t r, int32_t c);

void write_char(char ascii, int fg, int bg);

void write_str(char* string, int fg, int bg);

void vga_init();

//------------------------------------------------IO.H

#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15
#define PIC1_PORT_A 0x20
#define PIC2_PORT_A 0xA0
#define PIC1_START_INTERRUPT 0x20
#define PIC2_START_INTERRUPT 0x28
#define PIC2_END_INTERRUPT PIC2_START_INTERRUPT + 7
#define PIC_ACK 0x20
#define KBD_DATA_PORT 0x60
#define PORT 0x40   /* COM1 */
#define KEY_DEVICE    0x60
#define KEY_PENDING   0x64
#define KEYBOARD_NOTICES 0
#define KEYBOARD_IRQ 1

struct {
    char buff;
} kbd;

typedef struct {
    char *frame_buffer;
    char buffer[100];
    int8_t offset;
    int cursor;
    char* cursor_value;
} console;

console term;

typedef struct {
	char* keyboard_buffer;
	int keyboard_offset;
	char* mouse_buffer;
} io_buffer;


io_buffer *io_buff;


static inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	__asm__ __volatile__( "in %1, %0":  "=a"(ret): "Nd"(port) );
	return ret;
}

static inline uint16_t ins(uint16_t port) {
	uint16_t ret;
	__asm__ __volatile__( "inl %1, %0":  "=a"(ret): "Nd"(port) );
	return ret;
}


static inline uint32_t inl(uint16_t port) {
	uint32_t ret;
	__asm__ __volatile__( "inl %%dx, %%eax":  "=a"(ret): "Nd"(port) );
	return ret;
}



static inline void outb(uint16_t port, uint8_t val) {
	__asm__ __volatile__( "out %1, %0" : : "Nd"(port), "a"(val) );
}

static inline void outs(uint16_t port, uint16_t val) {
	__asm__ __volatile__( "outl %1, %0" : : "Nd"(port), "a"(val) );
}


static inline void outl(uint16_t port, uint32_t val) {
	__asm__ __volatile__( "outl %%eax, %%dx" : : "Nd"(port), "a"(val) );
}

//typedef void (*irq_handler_t) (struct regs*);

void keyboard_handler(irq_handler_t handler);

void keyboard_install();

void keyboard_reset_ps2();

void keyboard_wait();

void move_cursor(int32_t pos);

void getASCII(unsigned char c);


char readb(unsigned char c);

//------------------------------------------------PRINT.H

size_t vasprintf(char * buf, const char *fmt, va_list args);

int kprintf(const char *fmt, ...);

int sprintf(const char *fmt, ...);

int dprintf(const char *fmt, ...);

//------------------------------------------------SERIAL.H

typedef struct {
	int COM_PORT;
	int BAUD_RATE;
	int8_t works;
} serial_buffer;

serial_buffer serial;

void serial_install();

int serial_received();

char read_serial();

int8_t is_transmit_empty();

void write_serial(char* a);

//------------------------------------------------PCSOUND.H

uint32_t play_note(char note, int octave, int time);
//Play sound using built in speaker
void play_sound(uint32_t nFrequence);

//make it shutup
void nosound();

void beep_it(int freq,int interval);

//------------------------------------------------PCI.H

struct __pci_driver;

typedef struct {
	uint32_t vendor;
	uint32_t device;
	uint16_t headerType;
	uint16_t class;
	uint16_t subClass;
	uint16_t func;
	
	struct __pci_driver *driver;
} pci_device;

typedef struct {
	uint32_t vendor;
	uint32_t device;
	uint32_t func;
} pci_device_id;

typedef struct __pci_driver {
	pci_device_id *table;
	char *name;
	uint8_t (*init_one)(pci_device *);
	uint8_t (*init_driver)(void);
	uint8_t (*exit_driver)(void);
} pci_driver;

void pci_install();

void pci_proc_dump();


//------------------------------------------------CPU.H

#define CHECK_RANGE_FOR_BIT(begin, count, reg, what) for(int i=begin;i<=count;i++){if(reg & (1<<i)) cpu.what[i].flag = 1;}
typedef struct {
		int8_t flag;
		char* name;
	}CPU_TABLE;

	typedef struct {
		int8_t cache_type[32];
		int8_t cache_level[32];
		int8_t cache_self_initializing[32];
		int8_t cache_fully_associative[32];
		uint32_t cache_sets[32];
		uint32_t cache_coherency_line_size[32];
		uint32_t cache_physical_line_partitions[32];
		uint32_t cache_ways_of_associativity[32];
		size_t cache_total_size[32];
	} __attribute__((packed)) det_cache_params_t;
	


typedef struct {
	/* 				STANDARD FUNCTIONS 			*/

	CPU_TABLE cpu_flags[64];

// struct CPU_INSTRUCTION{
// 	int8_t FXSR;
// 	int8_t MMX;
// 	int8_t CLFLUSH;
// 	int8_t FMA;
// 	int8_t CX8;
// 	int8_t CX16;
// 	int8_t MSR;
// 	int8_t SSE;
// 	int8_t SSE2;
// 	int8_t SSE3;
// 	int8_t SSSE3;
// 	int8_t SSE41;
// 	int8_t SSE42;
// 	int8_t AVX;
// 	int8_t RDRAND;
// 	int8_t CMOV;
// 	int8_t XSAVE; 
// 	int8_t OSXSAVE;
// 	int8_t MOVBE;
// 	int8_t POPCNT; 
// 	int8_t AESNI;
// 	int8_t PCLMUL;
// 	int8_t SEP;
// 	// Probably AMD Instructions
// 	int8_t TBM;
// 	int8_t THREE_DNOW_PRE;
// 	int8_t SSE4A;
// 	int8_t THREE_DNOW_INSTR;
// 	int8_t RDTSCP;
// };
// 

	// Function 0x00 | Function 0x00
	char cpu_vendor_string[12];
	char cpu_vendor_id[32];
	uint32_t highest_std_info;

	// Function 0x01 | Function 0x01
// struct PROCESSOR_SIGNATURE{
// 	int8_t EXTENDED_FAMILY_ID;
// 	int8_t FAMILY_CODE;
// 	int8_t EXTENDED_MODEL_ID;
// 	int8_t MODEL_NUMBER;
// 	int8_t STEPPING_ID;
// 	int8_t PROCESSOR_TYPE;
// };
	uint16_t processor_signature[6];

// struct CPU_MISC_INFO{
// 	int8_t LOCAL_APIC_ID;
// 	int8_t CLFLUSH_SIZE;
// 	int8_t BRAND_ID;
// 	// Probably AMD information
// 	int8_t LOG_PRO_COUNT;
// };
	int16_t cpu_misc_info[4];

	// Function 0x03 
// struct PROCESSOR_SERIAL_NUMBER{
// 	int32_t UPPER_BITS;
// 	int32_t LOWER_BITS;
// };
	int32_t processor_serial_number[2];

	// Funciton 0x04
// struct DET_CACHE_PARAMS{
// 	int8_t CACHE_TYPE;
// 	int8_t CACHE_LEVEL;
// 	int8_t SINIT_CACHE_LVL;
// 	int8_t FULLY_ASSOC_CACHE;
// 	int8_t MAX_IDS_LOG_PRO;
// 	int8_t MAX_IDS_PRO_CORES;
// 	int16_t SYS_COH_LINE_SIZE;
// 	int16_t PHY_LINE_PART;
// 	int16_t WAYS_OF_ASSOC;
// 	int32_t NO_OF_SETS;
// 	int8_t WBINVD_COMPACT;
// };

	det_cache_params_t det_cache_params;

	// Function 0x05 | Function 0x05
// struct MONITOR{
// 	int16_t MIN_MONS_SIZE;
// 	int16_t MAX_MONS_SIZE;
// 	int8_t EMX;
// 	int8_t INTR_BRK_EVENT;
// 	int32_t C0_C7;
// };
	int32_t monitor[5];

	// Function 0x06 | Function 0x06
// struct DIGTSPM{
// 	int16_t THERMAL_COMPACT;
// 	int8_t NO_INTR_THRESH;
// 	int8_t HW_FEEDBACK_PERF_ENERGY_COMPACT;
// };
	int16_t dig_sensor_pm[3];

	// Function 0x09
// struct DCA {
// 	int32_t PLATFORM_DCA_CAP;
// };
	int32_t direct_cache[1];

	// Function 0x0A
// struct ARCH_PER_MON_FEATURES {
// 	int8_t VERSION_ID;
// 	int8_t NO_GP_PERF_COUNTER;
// 	int8_t BITS_WIDTH_GP_PERF_COUNTER;
// 	int8_t LEN_EBX_BIT_VECTOR;
// 	int8_t CORE_CYCLE_COMPACT;
// 	int16_t FIXED_FUNC_COMPACT; 
// };
	int16_t arcpmf[6];

	// Function 0x0B
// struct PROCESSOR_TOPOLOGY {
// 	int8_t BITS_RS_APIC;
// 	int8_t NO_FC_LOG_PRO;
// 	int8_t LEVEL_NUMBER;
// 	int8_t LEVEL_TYPE;
// 	int32_t EXT_APIC_ID;	
// };
	int32_t processor_topology[5];

	// Function 0x0D | Function 0x0D
// struct XSAVE {
// 	int32_t LOWER_32_BITS_XCR0;
// 	int32_t MAX_BYTES_XCR0;
// 	int32_t MAX_BYTES_XSAVE;
// 	int32_t UPPER_32_BITS_XCR0;
// 	int32_t XSAVE_STATES_SIZE;
// 	int32_t LOWER_XSS_OFFSET;
// 	int8_t IA32_XSS;
// 	int32_t UPPER_XSS_OFFSET;
// 	int32_t SAVE_AREA_SIZE;
// 	int32_t BEGIN_XSAVE_OFFSET;
// 	int8_t BIT0_SUB_LEAF_CHECK;
// };
	int32_t processor_ext_state_enum[11];

	/* 				EXTENDED FUNCTIONS 			*/

	uint32_t highest_ext_info;	

	// Function 0x8000_0001 | Function 0x0000_0007
// struct EXT_CPU_FEATURE{
// 	int32_t EXT_FEATURES;
// };
	int32_t extended_cpu_features[4];

	// Function 0x8000_0002, 0x8000_0003, 0x800_0004
	char processor_name[41];	

	// Function 0x8000_0006
// struct EXT_L2_CACHE {
// 	int16_t L2_SIZE;
// 	int8_t L2_ASSOC;
// 	int8_t L2_LINE_SIZE;
// };
	int16_t extended_L2_cache[3];

	// Function 0x8000_0007
// struct ADV_POW_MANG{
// 	int8_t TSC_INVAR;
// };
	int8_t advance_power_management[1];

	// Function 0x8000_Z0008
// struct VIR_PHY_ADDR_SIZE{
// 	int8_t VIRT_ADDR_SIZE;
// 	int8_t PHY_ADDR_SIZE;
// };
	int8_t virtual_physcial_address[2];


} CPU_TOPOLOGY;


CPU_TOPOLOGY cpu;


typedef struct {
	char* arc;
	char* op_modes;
	char* byte_order;
	char* cpus;
	char* online_cpu_list;
	char* threads_per_core;
	char* cores_per_socket;
	char* sockets;
	char* NUMA_nodes;
	char* vendor_id;
	char* cpu_family;
	char* model;
	char* model_name;
	char* stepping;
	char* cpu_mhz;
	char* cpu_max_mhz;
	char* cpu_min_mhz;
	char* bogo_MIPS;
	char* virtualization;
	char* l1d_cache;
	char* l1i_cache;
	char* l2_cache;
	char* l3_cache;
	char* flags[100];
} LSCPU;

LSCPU cpu_t;
static inline void cpuid(uint32_t reg, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx)
{
    __asm__ __volatile__("cpuid"
        : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx)
        : "0" (reg));
}

void get_cpuid_string(int code, char* where);

void cpu_init();

void intel_init();

void amd_init();



//------------------------------------------------COMMANDS.H

void read_text(char* ptr, unsigned int count);

void reboot();

void exec_cmd(int n, char* buff[5]);

//------------------------------------------------SYSCALLS.H

size_t sys_write(int fd, const void *buf, size_t count);

//------------------------------------------------PONG.H

struct {
	int playing;
	int x;
	int y;
	int dx;
	int dy;
} ball;


typedef struct {
	int moved;
	int x;
	int y;
} pedal;
void process_controls(unsigned char scancode);

void play_pong();

//------------------------------------------------MOUSE.H

#define MOUSE_IRQ 12

void mouse_install();





//------------------FOS.H---------------------------------------------

#define IRQ_OFF { asm volatile ("cli"); }
#define IRQ_RES { asm volatile ("sti"); }
#define PAUSE { asm volatile ("hlt"); }
#define IRQS_ON_AND_PAUSE { asm volatile ("sti\nhlt\ncli"); }
#define STOP while (1) { PAUSE; }

long long cycles;
long long seconds;
long long minutes;
long long hours;
struct regs {
    uint32_t gs, fs, es, ds;      /* pushed the segs last */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */

    uint32_t int_no, err_code;    /* our 'push byte #' and ecodes do this */
    uint32_t eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

typedef struct{
	multiboot_info_t* mem_db;
	unsigned int magic;
	unsigned long total_mem;
	unsigned long unused_mem;
	unsigned long used_mem;
	unsigned long kernel_start;
	unsigned long kernel_end;
	uint32_t module_start;
	uint32_t module_end;
} memory;

memory memory_t;

void *memset(void *dest, char val, size_t count);

void * memmove(void * restrict dest, const void * restrict src, size_t count);

void kmain();

#endif


