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

void elf_load_header(
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
		sprintf("\nYes it is an ELF. The header value is:\n");
		for (int i = 0; i < 16; ++i)
		{
			elf32_header.elf_ident[i] = *(char*)(ptr_to_header + i);
			sprintf("\t%x\n", elf32_header.elf_ident[i]);
		}
	}
	else
		sprintf("\nSorry this is not an ELF\n");
	
}


void elf_install()
{
	/**
	 * This is just testing code to verify is ELF works.
	 */
	elf_load_header(ramdisk);
}