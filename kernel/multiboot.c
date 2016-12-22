#include <kernel/fos.h>

#define CHECK_FLAG(flag,bit) ((flag) & (1<<bit))
int multiboot_check(multiboot_info_t* mbd, unsigned int magic){
   	if (magic != MULTIBOOT_BOOTLOADER_MAGIC){
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

	if (CHECK_FLAG (mbd->flags, 1))
		kprintf ("boot_device = 0x%x\n"
			, (unsigned) mbd->boot_device);
	
	if (CHECK_FLAG (mbd->flags, 2))
		kprintf ("cmdline = %s\n"
			, (char *) mbd->cmdline);

	if (CHECK_FLAG (mbd->flags, 3)){
		module_t *mod;
		int i;
		kprintf ("mods_count = %d, mods_addr = 0x%x\n"
			,(int) mbd->mods_count, (int) mbd->mods_addr);

		for (i = 0
			, mod = (module_t *) mbd->mods_addr
          		;i < mbd->mods_count;i++
            , mod++)
            kprintf(" mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n"
                 ,(unsigned) mod->mod_start
                 ,(unsigned) mod->mod_end
                 ,(char *) mod->string);
 	}

    if (CHECK_FLAG (mbd->flags, 4) && CHECK_FLAG (mbd->flags, 5)){
    	kprintf("Both bits 4 and 5 are set.\n");
    	return -1;
    }

   if (CHECK_FLAG (mbd->flags, 4)){
   	aout_symbol_table_t *multiboot_aout_sym = &(mbd->u.aout_sym);
   	kprintf("multiboot_aout_symbol_table: tabsize = 0x%0x, "
           "strsize = 0x%x, addr = 0x%x\n"
           ,(unsigned) multiboot_aout_sym->tabsize
           ,(unsigned) multiboot_aout_sym->strsize
           ,(unsigned) multiboot_aout_sym->addr);
   }

	if (CHECK_FLAG (mbd->flags, 5)){
		elf_section_header_table_t *multiboot_elf_sec 
		= &(mbd->u.elf_sec);
		kprintf("multiboot_elf_sec: num = %d, size = 0x%x,"
	            " addr = 0x%x, shndx = 0x%x\n"
	            ,(unsigned) multiboot_elf_sec->num
	            , (unsigned) multiboot_elf_sec->size
	            ,(unsigned) multiboot_elf_sec->addr
	            , (unsigned) multiboot_elf_sec->shndx);
	}

	if (CHECK_FLAG (mbd->flags, 6)){
		memory_map_t *mmap;
		kprintf ("mmap_addr = 0x%x, mmap_length = 0x%x\n"
           ,(unsigned) mbd->mmap_addr
           ,(unsigned) mbd->mmap_length);

		for (mmap = (memory_map_t *) mbd->mmap_addr
        	;(unsigned long) mmap < mbd->mmap_addr + mbd->mmap_length
        	;mmap = (memory_map_t *) ((unsigned long) mmap
        		+ mmap->size + sizeof (mmap->size)))

			kprintf (" size = 0x%x, base_addr = 0x%x%x,"
             " length = 0x%x%x, type = 0x%x\n"
             ,(unsigned) mmap->size
             ,mmap->base_addr_low
             ,mmap->base_addr_high
             ,mmap->length_low
             ,mmap->length_high
             ,(unsigned) mmap->type);
	}
  return 0;
}