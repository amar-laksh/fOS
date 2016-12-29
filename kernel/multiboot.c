// TODO - USE MULTIBOOT 2 INSTEAD OF MULTIBOOT 1
#include <kernel/fos.h>

#define CHECK_FLAG(flag,bit) ((flag) & (1<<bit))

unsigned long long multiboot_check(multiboot_info_t* mbd, unsigned int magic){
    unsigned long long total_mem_low = 0;
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
		int i;
		kprintf ("mods_count = %d, mods_addr = 0x%x\n"
			,(int) mbd->mods_count, (uint32_t) mbd->mods_addr);
      for (i = 0; i < mbd->mods_count; ++i ) {
        uint32_t module_start = *((uint32_t*)mbd->mods_addr + 8 * i);
        uint32_t module_end   = *(uint32_t*)(mbd->mods_addr + 8 * i + 4);
        kprintf("Module %d is at 0x%x:0x%x\n", i+1, module_start, module_end);
        ramdisk = (char*)0x07000000;
        memory_t.module_start = (uint32_t)ramdisk;
        memory_t.module_end = ((uint32_t)ramdisk + (module_end - module_start));
        memmove(ramdisk, (char *)module_start, (module_end - module_start));
        kprintf("Relocating... Now the module %d is at 0x%x:0x%x\n", i+1, ramdisk, (ramdisk+8*i+4));
        // char* ptr = module_start;
        // // char*  p = str_tok(ptr,"\n");
        //   // while(p != NULL){
        //   //   kprintf("%s \n", p);
        //   //   p = str_tok(NULL, "\n");
        //   // }
        // read_text(ptr,(module_end - module_start)); 
        // STOP;
        //typedef void (*call_module_t)(void);
        //call_module_t start_program = (call_module_t)module_start;
        //start_program();
      }
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

    mmap = (memory_map_t *) mbd->mmap_addr;
    while((unsigned long)mmap < mbd->mmap_addr + mbd->mmap_length){
      kprintf (" size = 0x%x, base_addr = 0x%x,"
         " length = 0x%x, type = 0x%x\n"
         ,(unsigned) mmap->size
         ,mmap->base_addr_low
         //,mmap->base_addr_high
         ,mmap->length_low
         //,mmap->length_high
         ,(unsigned) mmap->type);
      
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
  return total_mem_low;
}