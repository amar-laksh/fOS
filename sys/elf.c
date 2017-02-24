#include <kernel/fos.h>

/**
 * Steps to parse ELF files-
 * 	1.	Get the ELF header
 * 	2.	Do ELF Identification using elf_ident
 * 	3.	Parse out all the section info
 * 		a. Get the Section Headers
 * 		b. Do Section identification using sh_type
 * 	5. HACK for now - Execute the instructions in .text
 * 	4.	Use String table to build symbol table
 *  5.	Get the Relocation worked up
 */

void elf_parse_header(
					void* ptr_to_header
					)
{
	char* elf_identification;
	elf_identification = ptr_to_header;
	int status	=	*(elf_identification) == ELFMAG0
							?
					*(elf_identification+1) == ELFMAG1
							?
					*(elf_identification+2) == ELFMAG2
							?
					*(elf_identification+3) == ELFMAG3
							?
							1
					:0
					:0
					:0
					:0;

	if(status == 1){
		for (int i = 0; i < 16; ++i){
			elf32_header.elf_ident[i] = *(char*)(ptr_to_header + i);
		}
		elf32_header.elf_type = *(int*)(ptr_to_header+16);
		elf32_header.elf_machine = *(int*)(ptr_to_header+18);
		elf32_header.elf_version = *(int*)(ptr_to_header+20);
		elf32_header.elf_entry = *(int*)(ptr_to_header+24);
		elf32_header.elf_phoff = *(int*)(ptr_to_header+28);
		elf32_header.elf_shoff = *(int*)(ptr_to_header+32);
		elf32_header.elf_flags = *(int*)(ptr_to_header+36);
		elf32_header.elf_ehsize = *(int*)(ptr_to_header+40);
		elf32_header.elf_phentsize = *(int*)(ptr_to_header+42);
		elf32_header.elf_phnum = *(int*)(ptr_to_header+44);
		elf32_header.elf_shentsize = *(int*)(ptr_to_header+46);
		elf32_header.elf_shnum = *(int*)(ptr_to_header+48);
		elf32_header.elf_shstrndx = *(int*)(ptr_to_header+50);
	}
	else
		sprintf("\nSorry this is not an ELF\n");
	
}


ELF32_Shdr elf_get_section	(
						ELF32_Shdr shdrs_array[]
						, const char* section_name
						)
{
	ELF32_Shdr ptr;
	for (int i = 0; i < elf32_header.elf_shnum; ++i){
		if(equals(shdrs_array[i].sh_text_name, section_name) == 0){
			ptr = shdrs_array[i];
		}
	}
	return ptr;
}

void elf_dump_info	(
					ELF32_Shdr elf32_shdr[]
					, ELF32_Phdr elf32_phdr[]
					)
{
	sprintf("\nELF information:\n");
	sprintf("\nELF Header\n");
	sprintf("ELF identification info:\n");
	for (int i = 0; i < 16; ++i){
		sprintf(" %x", elf32_header.elf_ident[i]);
	}
	sprintf("\n");
	sprintf("ELF File is at:0x%x\n", ramdisk);
	sprintf("ELF File type: %d\n", elf32_header.elf_type);
	sprintf("ELF Machine type: %d\n", elf32_header.elf_machine);
	sprintf("ELF Version: %d\n", elf32_header.elf_version);
	sprintf("ELF Entry point: 0x%x\n", elf32_header.elf_entry);
	sprintf("ELF Program Header offset: 0x%x\n", elf32_header.elf_phoff);
	sprintf("ELF Section Header offset: 0x%x\n", elf32_header.elf_shoff);
	sprintf("ELF Flags: %d\n", elf32_header.elf_flags);
	sprintf("ELF Header size: %d\n", elf32_header.elf_ehsize);
	sprintf("ELF Program header entry size: %d\n", elf32_header.elf_phentsize);
	sprintf("ELF No of entries in Program header : %d\n", elf32_header.elf_phnum);
	sprintf("ELF Section header entry size: %d\n", elf32_header.elf_shentsize);
	sprintf("ELF No of entries in Section header: %d\n", elf32_header.elf_shnum);
	sprintf("ELF Section header table index: 0x%x\n", elf32_header.elf_shstrndx);


	sprintf("\nELF Program Headers\n\n");
	sprintf("Type\t  Offset\t  VirtAddr\t  PhyAddr\t  FileSiz\t  MemSiz\t  Flg\t  Align\n");
	for (int i = 0; i < elf32_header.elf_phnum; ++i){
		sprintf("%d\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t%d\t0x%x\n"
			, elf32_phdr[i].p_type
			, elf32_phdr[i].p_offset
			, elf32_phdr[i].p_vaddr
			, elf32_phdr[i].p_paddr
			, elf32_phdr[i].p_filesz
			, elf32_phdr[i].p_memsz
			, elf32_phdr[i].p_flags
			, elf32_phdr[i].p_align);
	}
	sprintf("\n\n\n");

	sprintf("\nELF Section Headers\n\n");
	sprintf("Name\t\tType\t  Addr  \t  Off\t\tSize\t\tES\t\tFlg\tLk\tInf\tAl\n");
	for (int i = 0; i < elf32_header.elf_shnum; ++i){
		sprintf("%s\t\t%d\t%x\t%x\t%x\t%x\t%d\t%d\t%d\t%d\n"
			, elf32_shdr[i].sh_text_name
			, elf32_shdr[i].sh_type
			, elf32_shdr[i].sh_addr
			, elf32_shdr[i].sh_offset
			, elf32_shdr[i].sh_size
			, elf32_shdr[i].sh_entsize
			, elf32_shdr[i].sh_flags
			, elf32_shdr[i].sh_link
			, elf32_shdr[i].sh_info
			, elf32_shdr[i].sh_addralign);
	}
}

void elf_install()
{
	elf_parse_header(ramdisk_top);
	ELF32_Phdr phdrs_array[elf32_header.elf_phnum];
	ELF32_Shdr shdrs_array[elf32_header.elf_shnum]; 


	// Accumulating Program headers
	for (int i = 0; i < elf32_header.elf_phnum; ++i){
		void* ptr_to_header = (ramdisk+(elf32_header.elf_phoff + (elf32_header.elf_phentsize*i)));
		phdrs_array[i].p_type = *(int*)(ptr_to_header);
		phdrs_array[i].p_offset = *(int*)(ptr_to_header+4);
		phdrs_array[i].p_vaddr = *(int*)(ptr_to_header+8);
		phdrs_array[i].p_paddr = *(int*)(ptr_to_header+12);
		phdrs_array[i].p_filesz = *(int*)(ptr_to_header+16);
		phdrs_array[i].p_memsz = *(int*)(ptr_to_header+20);
		phdrs_array[i].p_flags = *(int*)(ptr_to_header+24);
		phdrs_array[i].p_align = *(int*)(ptr_to_header+28);
	}

	// Accumulating Section headers
	for (int i = 0; i < elf32_header.elf_shnum; ++i){
		void* ptr_to_header = (ramdisk+(elf32_header.elf_shoff + (elf32_header.elf_shentsize*i)));
		shdrs_array[i].sh_name = *(int*)(ptr_to_header);
		shdrs_array[i].sh_type = *(int*)(ptr_to_header+4);
		shdrs_array[i].sh_flags = *(int*)(ptr_to_header+8);
		shdrs_array[i].sh_addr = *(int*)(ptr_to_header+12);
		shdrs_array[i].sh_offset = *(int*)(ptr_to_header+16);
		shdrs_array[i].sh_size = *(int*)(ptr_to_header+20);
		shdrs_array[i].sh_link = *(int*)(ptr_to_header+24);
		shdrs_array[i].sh_info = *(int*)(ptr_to_header+28);
		shdrs_array[i].sh_addralign = *(int*)(ptr_to_header+32);
		shdrs_array[i].sh_entsize = *(int*)(ptr_to_header+36);
	}

	void* _shstrtable = ramdisk+shdrs_array[elf32_header.elf_shstrndx].sh_offset;
	// Parsing the string table
	for (int i = 0; i < elf32_header.elf_shnum; ++i){
		shdrs_array[i].sh_text_name = _shstrtable+shdrs_array[i].sh_name;
	}

	// ELF32_Shdr section = elf_get_section(shdrs_array, ".text");
	// void* ptr = (ramdisk + section.sh_offset);
	// sprintf("section is at: 0x%x\n", ptr);
	// for (int i = 0; i < section.sh_size; i+=4){
	// 	sprintf("%x  ", *(int*)(ptr+i));
	// }
	//elf_dump_info(shdrs_array, phdrs_array);
}