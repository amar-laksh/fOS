// TODO - IMPLEMENT DECLARATION AND INITIALIZATION OF STRUCTS IN CPU.H
#include <cpuid.h>
#include <cpu.h>
#include <vga.h>
#include <io.h>
#include <stdint.h>
#include <string.h>


void cpuid_get_vendor_string(char* where) {
      cpuid(0, 0, (uint32_t *)(where + 0),
		      (uint32_t *)(where + 8), 
		      (uint32_t *)(where + 4));
      where[12] = '\0';
  }

void cpuid_get_processor_name(char* name){
     cpuid(0x80000002, (uint32_t *)(name +  0), 
		     (uint32_t *)(name +  4), 
		     (uint32_t *)(name +  8), 
		     (uint32_t *)(name + 12));

    cpuid(0x80000003, (uint32_t *)(name + 16), 
		    (uint32_t *)(name + 20), 
		    (uint32_t *)(name + 24), 
		    (uint32_t *)(name + 28));

    cpuid(0x80000004, (uint32_t *)(name + 32), 
		    (uint32_t *)(name + 36), 
		    (uint32_t *)(name + 40), 
		    (uint32_t *)(name + 44));

    // Processor name is right justified with leading spaces
    const char *p = name;
    while (*p == ' ') ++p;
    name[41] = '\0';
}

void cpu_init(){
// TODO - ADD ALL THE STRUCTS AND VALUES
  struct CPU_INSTRUCTION cpu_instr = {
	  				(1<<4)
  				};
  uint32_t highest_std_info, highest_ext_info;
  
  uint32_t eax, ebx, ecx, edx;
  
  char where[12], cpu_name[41];

  cpuid(0x80000000, &highest_ext_info,
		  &ebx, &ecx, &edx);
  cpuid(0x00000001, &highest_std_info,
		  &ebx, &ecx, &edx);

  write_str("Initializing CPU Topology scan...\r");
  write_str("Vendor String:");
  cpuid_get_vendor_string(where);
  write_str(where);
  write_str("\r");

  if (highest_std_info >= 0x01){
    cpuid(0x01, &eax, &ebx, &ecx, &edx);

    write_str("Features:");

    if (edx & EDX_PSN)      write_str(" PSN");
    if (edx & EDX_HTT)      write_str(" HTT");
    if (edx & EDX_PSE)      write_str(" PSE");
    if (edx & EDX_PAE)      write_str(" PAE");
    if (edx & EDX_APIC)     write_str(" APIC");
    if (edx & EDX_MTRR)     write_str(" MTRR");

    write_str("\r");

    write_str("Instructions:");
    if (edx && cpu_instr.TSC) write_str(" TSC");
    //if (edx & EDX_TSC)      write_str(" TSC");
    if (edx & EDX_MSR)      write_str(" MSR");
    if (edx & EDX_SSE)      write_str(" SSE");
    if (edx & EDX_SSE2)     write_str(" SSE2");
    if (ecx & ECX_SSE3)     write_str(" SSE3");
    if (ecx & ECX_SSSE3)    write_str(" SSSE3");
    if (ecx & ECX_SSE41)    write_str(" SSE41");
    if (ecx & ECX_SSE42)    write_str(" SSE42");
    if (ecx & ECX_AVX)      write_str(" AVX");
    if (ecx & ECX_F16C)     write_str(" F16C");
    if (ecx & ECX_RDRAND)   write_str(" RDRAND");

  }

  write_str("\r");

  if (highest_ext_info >= 0x80000001){
    cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
    if (edx & EDX_64_BIT) write_str("64-bit Architecture");
    else write_str("32-bit Architecture");
  }

  write_str("\r");

  if(highest_ext_info >= 0x80000004){
    cpuid_get_processor_name(cpu_name);
    write_str("CPU name:");
    write_str(cpu_name);
  }
}
