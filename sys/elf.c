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


void elf_parse_phdrs(
					void* ptr_to_header,
					ELF32_Phdr elf32_phdr
					)
{
	elf32_phdr.p_type = *(int*)(ptr_to_header);
	elf32_phdr.p_offset = *(int*)(ptr_to_header+4);
	elf32_phdr.p_vaddr = *(int*)(ptr_to_header+8);
	elf32_phdr.p_paddr = *(int*)(ptr_to_header+12);
	elf32_phdr.p_filesz = *(int*)(ptr_to_header+16);
	elf32_phdr.p_memsz = *(int*)(ptr_to_header+20);
	elf32_phdr.p_flags = *(int*)(ptr_to_header+24);
	elf32_phdr.p_align = *(int*)(ptr_to_header+28);
}


void elf_parse_shdrs(
					void* ptr_to_header,
					ELF32_Shdr elf32_shdr
					)
{
	elf32_shdr.sh_name = *(int*)(ptr_to_header);
	elf32_shdr.sh_type = *(int*)(ptr_to_header+4);
	elf32_shdr.sh_flags = *(int*)(ptr_to_header+8);
	elf32_shdr.sh_addr = *(int*)(ptr_to_header+12);
	elf32_shdr.sh_offset = *(int*)(ptr_to_header+16);
	elf32_shdr.sh_size = *(int*)(ptr_to_header+20);
	elf32_shdr.sh_link = *(int*)(ptr_to_header+24);
	elf32_shdr.sh_info = *(int*)(ptr_to_header+28);
	elf32_shdr.sh_addralign = *(int*)(ptr_to_header+32);
	elf32_shdr.sh_entsize = *(int*)(ptr_to_header+36);
}


void elf_dump_info	(
					ELF32_Shdr elf32_shdr[]
					, ELF32_Phdr elf32_phdr[]
					)
{
	sprintf("\nELF information:\n");
	sprintf("\nELF Header\n");
	sprintf("ELF identification info:");
	for (int i = 0; i < 16; ++i){
		sprintf(" %x", elf32_header.elf_ident[i]);
	}
	sprintf("\n");

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
	sprintf("ELF Section header table index: %d\n", elf32_header.elf_shstrndx);


	sprintf("\nELF Program Headers\n\n");

	for (int i = 0; i < elf32_header.elf_phnum; ++i){
		sprintf("ELF Segment Type: %d\n", elf32_phdr[i].p_type);
		sprintf("ELF Segment offset: 0x%x\n", elf32_phdr[i].p_offset);
		sprintf("ELF Segment Virtual Address offset: 0x%x\n", elf32_phdr[i].p_vaddr);
		sprintf("ELF Segment Physical Address offset: 0x%x\n", elf32_phdr[i].p_paddr);
		sprintf("ELF Segment file size: %d\n", elf32_phdr[i].p_filesz);
		sprintf("ELF Segment memory size: %d\n", elf32_phdr[i].p_memsz);
		sprintf("ELF Segment flags: %d\n", elf32_phdr[i].p_flags);
		sprintf("ELF Segment align: %d\n", elf32_phdr[i].p_align);
		sprintf("\n\n\n");
	}

	sprintf("\nELF Section Header\n\n");

	for (int i = 0; i < elf32_header.elf_shnum; ++i){
		sprintf("ELF Section Name: %d\n", elf32_shdr[i].sh_name);
		sprintf("ELF Section Type: %d\n", elf32_shdr[i].sh_type);
		sprintf("ELF Section Flags: %d\n", elf32_shdr[i].sh_flags);
		sprintf("ELF Section Address: 0x%x\n", elf32_shdr[i].sh_addr);
		sprintf("ELF Section Offset: 0x%x\n", elf32_shdr[i].sh_offset);
		sprintf("ELF Section Size: %d\n", elf32_shdr[i].sh_size);
		sprintf("ELF Section Link: 0x%x\n", elf32_shdr[i].sh_link);
		sprintf("ELF Section Information: %d\n", elf32_shdr[i].sh_info);
		sprintf("ELF Section Address Alginment: %d\n", elf32_shdr[i].sh_addralign);
		sprintf("ELF Section Entry size: %d\n", elf32_shdr[i].sh_entsize);
		sprintf("\n\n\n");
	}
}

void elf_install()
{
	elf_parse_header(ramdisk);
	ELF32_Phdr phdrs_array[elf32_header.elf_phnum];
	ELF32_Shdr shdrs_array[elf32_header.elf_shnum];

	for (int i = 0; i < elf32_header.elf_phnum; ++i){
		elf_parse_phdrs(ramdisk
						+(elf32_header.elf_phoff
						+(elf32_header.elf_phentsize*i))
						, phdrs_array[i]);
	}

	for (int i = 0; i < elf32_header.elf_shnum; ++i){
		elf_parse_shdrs(ramdisk
						+(elf32_header.elf_shoff
						+(elf32_header.elf_shentsize*i))
						, shdrs_array[i]);
	}
	elf_dump_info(phdrs_array, shdrs_array);
}