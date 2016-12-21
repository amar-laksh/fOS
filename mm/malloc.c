// TODO - Implement multiboot_info with GRUB to detect memory
#include <mm/malloc.h>
#include <sys/print.h>

uint32_t last_alloc = 0;
uint32_t heap_end = 0;
uint32_t heap_begin = 0;
uint32_t memory_used = 0;

void mm_init(uint32_t kernel_end, unsigned long total_mem){
	last_alloc = kernel_end + 0x1000;
	heap_begin = last_alloc;
	heap_end = 0x400000;  
	memset((char *)heap_begin, 0, heap_end - heap_begin);
	//kprintf("Kernel heap starts at 0x%x\n", last_alloc);
}
void mm_print_out(){
	kprintf("Kernel heap starts at 0x%x\n", last_alloc);
	kprintf("Memory used: %x bytes\n", memory_used);
	kprintf("Memory free: %x bytes\n", heap_end 
										- heap_begin 
										- memory_used);
	kprintf("Heap size: %x bytes\n", heap_end - heap_begin);
	kprintf("Heap start: 0x%x\n", heap_begin);
	kprintf("Heap end: 0x%x\n", heap_end);
}

void free(void *mem){
	alloc_t *alloc = (mem - sizeof(alloc_t));
	memory_used -= alloc->size + sizeof(alloc_t);
	alloc->status = 0;
}


char* malloc(size_t size){
	if(!size) return 0;

	/* Loop through blocks and find a block sized the same or bigger */
	uint8_t *mem = (uint8_t *)heap_begin;
	while((uint32_t)mem < last_alloc)
	{
		alloc_t *a = (alloc_t *)mem;
		if(!a->size)
			goto nalloc;
		if(a->status) {
			mem += a->size;
			mem += sizeof(alloc_t);
			mem += 4;
			continue;
		}
		if(a->size >= size){
			a->status = 1;

			kprintf("RE:Allocated %d bytes from 0x%x to 0x%x\n", size, mem + sizeof(alloc_t), mem + sizeof(alloc_t) + size);
			memset(mem + sizeof(alloc_t), 0, size);
			memory_used += size + sizeof(alloc_t);
			return (char *)(mem + sizeof(alloc_t));
		}
		mem += a->size;
		mem += sizeof(alloc_t);
		mem += 4;
	}

	nalloc:;
	if(last_alloc+size+sizeof(alloc_t) >= heap_end){
		sprintf("Cannot allocate %d bytes! Out of memory.\n", size);
	}
	alloc_t *alloc = (alloc_t *)last_alloc;
	alloc->status = 1;
	alloc->size = size;

	last_alloc += size;
	last_alloc += sizeof(alloc_t);
	last_alloc += 4;
	//kprintf("Allocated %d bytes from 0x%x to 0x%x\n", size, (uint32_t)alloc + sizeof(alloc_t), last_alloc);
	memory_used += size + 4 + sizeof(alloc_t);
	memset((char *)((uint32_t)alloc + sizeof(alloc_t)), 0, size);
	return (char *)((uint32_t)alloc + sizeof(alloc_t));
}
