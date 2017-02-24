// TODO - TO USE MULTIPLE MODULES CREATE INDEX POINTER ARRAY TO EACH IN RAMDISK
#include <kernel/fos.h>

#define CHECK_FLAG(flag,bit) ((flag) & (1<<bit))

unsigned long long multiboot_check(
																		multiboot_info_t* mbd
																		, unsigned int magic
																	)
{
		unsigned long long total_mem_low = 0;
		
		if(magic != MULTIBOOT_BOOTLOADER_MAGIC){
			kprintf("Invalid magic number: 0x%x\n"
								, (unsigned) magic);
			
			return -1;
		}
		
		kprintf("\n\nFlags:%x\n\n", mbd->flags);
		
		if(CHECK_FLAG(mbd->flags,0)){
			kprintf("Lower memory:%d KB\n"
								,mbd->mem_lower);
			kprintf("Upper memory:%d MB\n"
								,mbd->mem_upper/1024);
		}

	if(CHECK_FLAG (mbd->flags, 1))
		kprintf ("boot_device = 0x%x\n"
			, (unsigned) mbd->boot_device);
	
	if(CHECK_FLAG (mbd->flags, 2))
		kprintf ("cmdline = %s\n"
			, (char *) mbd->cmdline);

	if(CHECK_FLAG (mbd->flags, 6)){
		kprintf ("mmap_addr = 0x%x, mmap_length = 0x%x\n"
			,(unsigned) mbd->mmap_addr
			,(unsigned) mbd->mmap_length
			);
		memory_map_t *mmap;
		mmap = (memory_map_t *) mbd->mmap_addr;
		while((unsigned long)mmap < mbd->mmap_addr + mbd->mmap_length){
			kprintf (" size = 0x%x, base_addr = 0x%x,"
								" length = 0x%x, type = 0x%x\n"
								,(unsigned) mmap->size
								,mmap->base_addr_low
								//,mmap->base_addr_high
								,mmap->length_low
								//,mmap->length_high
								,(unsigned) mmap->type
							);
/**
* Here we collect the safe zones in the memory for the ramdisk
* The ramdisk is saved in the last safe zone with ramdisk starting
* from the middle of it, to the end of the zone.
*/
			if(mmap->type ==  0x1){
				ramdisk = (char*)(mmap->length_low/2); 
				ramdisk_top = mmap->length_low - 0x100000;
			}
			total_mem_low += mmap->length_low;
			mmap = (memory_map_t *)((unsigned long) mmap 
							+ mmap->size + sizeof(mmap->size));

		}
		total_mem_low /= 1024;
		total_mem_low /= 1024;
		total_mem_low += 1;
		kprintf("My best guess of the memory is:\nTotal:%d MiB\n"
										,total_mem_low);
	}

	if(CHECK_FLAG (mbd->flags, 3)){
		int i;
		kprintf ("mods_count = %d, mods_addr = 0x%x\n"
			,(int) mbd->mods_count, (uint32_t) mbd->mods_addr);

		kprintf("Allocating a ramdisk and clearing it for use...\n");
		memset(ramdisk,0,(ramdisk_top - (unsigned long)ramdisk));

			for (i = 0; i <= mbd->mods_count; i+=2) {
				uint32_t module_start = *(uint32_t*)(mbd->mods_addr + 8 * i);
				uint32_t module_end   = *(uint32_t*)(mbd->mods_addr + 8 * i + 4);
				kprintf("Module %d is at 0x%x:0x%x and ramdisk is at: 0x%x\n"
								, i+1
								, module_start, module_end
								, ramdisk
								);
				/**
				 * We need to relocate all the modules to the ram disk
				 */
				memory_t.module_start = (uint32_t)ramdisk;
				
				memory_t.module_end = (uint32_t)ramdisk 
															+ (module_end- module_start);
				memmove (
								ramdisk
								, (char *)module_start
								, (module_end - module_start)
								);
				
				kprintf("Relocating... Now the module %d is at 0x%x:0x%x\n"
								, i+1
								, ramdisk
								, memory_t.module_end
								);
				kprintf("The ramdisk now starts is at 0x%x:0x%x for I/O\n"
								,ramdisk_top, memory_t.module_end);
				ramdisk = memory_t.module_end;
			}
		}

		if(CHECK_FLAG (mbd->flags, 4) && CHECK_FLAG (mbd->flags, 5)){
			kprintf("Both bits 4 and 5 are set.\n");
			return -1;
		}

	 if(CHECK_FLAG (mbd->flags, 4)){
		aout_symbol_table_t *multiboot_aout_sym = &(mbd->u.aout_sym);
		kprintf("multiboot_aout_symbol_table: tabsize = 0x%0x, "
					 "strsize = 0x%x, addr = 0x%x\n"
					 ,(unsigned) multiboot_aout_sym->tabsize
					 ,(unsigned) multiboot_aout_sym->strsize
					 ,(unsigned) multiboot_aout_sym->addr);
	 }

	if(CHECK_FLAG (mbd->flags, 5)){
		elf_section_header_table_t *multiboot_elf_sec 
		= &(mbd->u.elf_sec);
		kprintf("multiboot_elf_sec: num = %d, size = 0x%x,"
							" addr = 0x%x, shndx = 0x%x\n"
							,(unsigned) multiboot_elf_sec->num
							, (unsigned) multiboot_elf_sec->size
							,(unsigned) multiboot_elf_sec->addr
							, (unsigned) multiboot_elf_sec->shndx);
	}
	return total_mem_low;
}