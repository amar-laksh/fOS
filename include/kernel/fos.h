
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

#define LOG 552013




char* ramdisk;
uint32_t ramdisk_top;
int button_on;
int button_left;
int button_right;
int printed;

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
	int array_offset;
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

void paint_screen();

char get_char_cell(int r, int c);

void draw_char(uint32_t p, char ch, uint8_t fg, uint8_t bg);

int32_t draw_str(char string[], int32_t r, int32_t c);

void write_char(char ascii, int fg, int bg);

void write_str(char* string, int fg, int bg);

void vga_init();

//------------------------------------------------IO.H
#include <sys/io_list.h>
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
    enum vga_color color_fg_value;
    enum vga_color color_bg_value;
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
	uint8_t headerType;
	uint8_t class;
	uint8_t subClass;
	uint16_t func;
	uint32_t BAR0;
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

#define CHECK_RANGE_FOR_BIT(begin, count, reg, what) for(int i=begin;i<=count;i++)\
														{if(reg & (1<<i)) cpu.what[i].flag = 1;}
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


	// Function 0x00 | Function 0x00
	char cpu_vendor_string[12];
	char cpu_vendor_id[32];
	uint32_t highest_std_info;

	// Function 0x01 | Function 0x01
	uint16_t processor_signature[6];

	int16_t cpu_misc_info[4];

	// Function 0x03 
	int32_t processor_serial_number[2];

	// Funciton 0x04
	det_cache_params_t det_cache_params;

	// Function 0x05 | Function 0x05
	int32_t monitor[5];

	// Function 0x06 | Function 0x06
	int16_t dig_sensor_pm[3];

	// Function 0x09
	int32_t direct_cache[1];

	// Function 0x0A
	int16_t arcpmf[6];

	// Function 0x0B
	int32_t processor_topology[5];

	// Function 0x0D | Function 0x0D
	int32_t processor_ext_state_enum[11];

	/* 				EXTENDED FUNCTIONS 			*/

	uint32_t highest_ext_info;	

	// Function 0x8000_0001 | Function 0x0000_0007
	int32_t extended_cpu_features[4];

	// Function 0x8000_0002, 0x8000_0003, 0x800_0004
	char processor_name[41];	

	// Function 0x8000_0006
	int16_t extended_L2_cache[3];

	// Function 0x8000_0007
	int8_t advance_power_management[1];

	// Function 0x8000_Z0008
	int8_t virtual_physcial_address[2];


} CPU_TOPOLOGY;


CPU_TOPOLOGY cpu;



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

//------------------------------------------------VIM.H

void vim();

//------------------------------------------------MOUSE.H

#define MOUSE_IRQ 12

void mouse_install();

//------------------ELF.H---------------------------------------------

typedef uint32_t ELF32_Word;
typedef uint32_t ELF32_Addr;
typedef uint32_t ELF32_Off;
typedef uint32_t ELF32_Sword;
typedef uint16_t ELF32_Half;

#define ELFMAG0   0x7f
#define ELFMAG1   'E'
#define ELFMAG2   'L'
#define ELFMAG3   'F'
#define EI_IDENT	16


typedef struct {
	unsigned char	elf_ident[EI_IDENT];
	ELF32_Half		elf_type;
	ELF32_Half		elf_machine;
	ELF32_Word		elf_version;
	ELF32_Addr		elf_entry;
	ELF32_Off		elf_phoff;
	ELF32_Off		elf_shoff;
	ELF32_Word		elf_flags;
	ELF32_Half		elf_ehsize;
	ELF32_Half		elf_phentsize;
	ELF32_Half		elf_phnum;
	ELF32_Half		elf_shentsize;
	ELF32_Half		elf_shnum;
	ELF32_Half		elf_shstrndx;
} ELF32_Header;

typedef struct {
	ELF32_Word		p_type;
	ELF32_Off		p_offset;
	ELF32_Addr		p_vaddr;
	ELF32_Addr		p_paddr;
	ELF32_Word		p_filesz;
	ELF32_Word		p_memsz;
	ELF32_Word		p_flags;
	ELF32_Word		p_align;
} ELF32_Phdr;

typedef struct {
	ELF32_Word	sh_name;
	char*		sh_text_name;
	ELF32_Word	sh_type;
	ELF32_Word	sh_flags;
	ELF32_Addr	sh_addr;
	ELF32_Off	sh_offset;
	ELF32_Word	sh_size;
	ELF32_Word	sh_link;
	ELF32_Word	sh_info;
	ELF32_Word	sh_addralign;
	ELF32_Word	sh_entsize;
} ELF32_Shdr;

ELF32_Header elf32_header;
ELF32_Shdr elf32_shdr;
ELF32_Phdr elf32_phdr;




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


typedef struct {
	uint32_t module_start;
	uint32_t module_end;
} MEMORY_MODULE;



typedef struct{
	multiboot_info_t* mem_db;
	unsigned int magic;
	unsigned long total_mem;
	unsigned long unused_mem;
	unsigned long used_mem;
	unsigned long kernel_start;
	unsigned long kernel_end;
	MEMORY_MODULE modules[];
} memory;

memory memory_t;

void *memset(void *dest, char val, size_t count);

void * memmove(void * restrict dest, const void * restrict src, size_t count);

void kmain();

#endif


