// TODO - IMPLEMENT DECLARATION AND INITIALIZATION OF STRUCTS IN CPU.H

#include <sys/cpu.h>
#define cpuid_simple(in, a, b, c, d) asm("cpuid": "=a" (a), "=b" (b), "=c" (c), "=d" (d) : "a" (in));

/*
struct CPU_FEATURE cpu.cpu_features;
struct CPU_INSTRUCTION cpu.cpu_instructions;
struct cpu.processor_signature cpu.processor_signature;
struct cpu.cpu_misc_info cpu.cpu_misc_info;
struct PROCESSOR_SERIAL_NUMBER processor_serial_number;
struct cpu.det_cache_params cpu.det_cache_params;
struct cpu.monitor cpu.monitor;
*/
CPU_TOPOLOGY cpu;


uint32_t highest_std_info, highest_ext_info;
uint32_t eax, ebx, ecx, edx,crap;

char *vendor_strings[] ={
	"AuthenticAMD",
	"AMDisbetter!",
	"GenuineIntel"
};



static void get_vendor_string(char* where) {
      cpuid(0, 0, (uint32_t *)(where + 0),
		      (uint32_t *)(where + 8), 
		      (uint32_t *)(where + 4));
      where[12] = '\0';
  }

static void get_processor_name(char* name){
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
	cpuid_simple(0,crap,ebx,crap,crap);
	switch(ebx){
		case 0x756e6547:	intel_init();break;
  		case 0x68747541:	amd_init();break;
  	}
  }

void intel_init(){
	cpuid(0x80000000, 
		&highest_ext_info,
		&ebx, &ecx, &edx);
	cpuid(0x00000001, 
		&highest_std_info,
		&ebx, &ecx, &edx);

	get_vendor_string(cpu.cpu_vendor_string);
	
	cpu.highest_std_info = highest_std_info;
	cpu.highest_ext_info = highest_ext_info;

	

	/** Processor Serial Number **/
	cpuid(0x03,&eax, &ebx, &ecx, &edx);
	cpu.processor_serial_number.LOWER_BITS = ecx;
	cpu.processor_serial_number.UPPER_BITS = edx;

	/** Deterministic Cache Parameters **/
	cpuid(0x04,&eax, &ebx, &ecx, &edx);
	cpu.det_cache_params.CACHE_TYPE = (eax & 0x1F);
	cpu.det_cache_params.CACHE_LEVEL = (eax & 0xE0);
	cpu.det_cache_params.SINIT_CACHE_LVL = (eax & 0x100);
	cpu.det_cache_params.FULLY_ASSOC_CACHE = (eax & 0x200);
	cpu.det_cache_params.MAX_IDS_LOG_PRO = (eax & 0x3FFC000);
	cpu.det_cache_params.MAX_IDS_PRO_CORES = (eax & 0xFC000000);
	cpu.det_cache_params.SYS_COH_LINE_SIZE = (ebx & 0xFFF);
	cpu.det_cache_params.PHY_LINE_PART = (ebx & 0x3FF000);
	cpu.det_cache_params.WAYS_OF_ASSOC = (ebx & 0xFFC00000);
	cpu.det_cache_params.NO_OF_SETS = ecx;
	cpu.det_cache_params.WBINVD_COMPACT = edx;


	/** cpu.monitor Features **/
	cpuid(0x05,&eax, &ebx, &ecx, &edx);
	cpu.monitor.MIN_MONS_SIZE = (eax & 0xFFFF);
	cpu.monitor.MAX_MONS_SIZE = (ebx & 0xFFFF);
	cpu.monitor.EMX = (ecx & 0x1);
	cpu.monitor.INTR_BRK_EVENT = (ecx & 0x2);
	cpu.monitor.C0_C7 = edx;

	/** Function 0x01 **/
	if (highest_std_info >= 0x01){
	    cpuid(0x01, &eax, &ebx, &ecx, &edx);

	    /** Processor Signature **/
	    cpuid(0x01, &eax, &ebx, &ecx, &edx);
	    cpu.processor_signature.STEPPING_ID = (eax & 0xF);
	    cpu.processor_signature.MODEL_NUMBER = (eax & 0xF0);
	    cpu.processor_signature.FAMILY_CODE = (eax & 0xF00);
	    cpu.processor_signature.PROCESSOR_TYPE = (eax & 0x6000);
	    cpu.processor_signature.EXTENDED_MODEL_ID = (eax & 0xF0000);
	    cpu.processor_signature.EXTENDED_FAMILY_ID = (eax & 0xFF00000);
	    /** CPU Features **/
	    if (edx & (1<<0) )		cpu.cpu_features.FPU = 1;
	    if (edx & (1<<1) )		cpu.cpu_features.VME = 1;
	    if (edx & (1<<2) )		cpu.cpu_features.DE = 1;
	    if (edx & (1<<3) )		cpu.cpu_features.PSE = 1;
	    if (edx & (1<<5) )		cpu.cpu_features.TSC = 1;
	    if (edx & (1<<6) )		cpu.cpu_features.PAE = 1;
	    if (edx & (1<<7) )		cpu.cpu_features.MCE = 1;
	    if (edx & (1<<9) )		cpu.cpu_features.APIC = 1;
	    if (edx & (1<<12) )		cpu.cpu_features.MTRR = 1;
	    if (edx & (1<<13) )		cpu.cpu_features.PGE = 1;
	    if (edx & (1<<14) )		cpu.cpu_features.MCA = 1;

	    if (edx & (1<<16) )		cpu.cpu_features.PAT = 1;
	    if (edx & (1<<17) )		cpu.cpu_features.PSE36 = 1;
	    if (edx & (1<<18) )		cpu.cpu_features.PSN = 1;
	    if (edx & (1<<21) )		cpu.cpu_features.DS = 1;
	    if (edx & (1<<22) )		cpu.cpu_features.ACPI = 1;
	    if (edx & (1<<27) )		cpu.cpu_features.SS = 1;
	    if (edx & (1<<28) )		cpu.cpu_features.HTT = 1;
	    if (edx & (1<<29) )		cpu.cpu_features.TM = 1;
	    if (edx & (1<<31) )		cpu.cpu_features.PBE = 1;

	    if (ecx & (1<<2) )		cpu.cpu_features.DTES64 = 1;
	    if (ecx & (1<<3) )		cpu.cpu_features.MONITOR = 1;
	    if (ecx & (1<<4) )		cpu.cpu_features.DS_CPL = 1;
	    if (ecx & (1<<5) )		cpu.cpu_features.VMX = 1;
	    if (ecx & (1<<6) )		cpu.cpu_features.SMX = 1;
	    if (ecx & (1<<7) )		cpu.cpu_features.EST = 1;
	    if (ecx & (1<<8) )		cpu.cpu_features.TM2 = 1;
	    if (ecx & (1<<10) )		cpu.cpu_features.CNXT_ID = 1;
	    if (ecx & (1<<14) )		cpu.cpu_features.XTPR = 1;
	    if (ecx & (1<<15) )		cpu.cpu_features.PDCM = 1;

	    if (ecx & (1<<17) )		cpu.cpu_features.PCID = 1;
	    if (ecx & (1<<18) )		cpu.cpu_features.DCA = 1;
	    if (ecx & (1<<21) )		cpu.cpu_features.X2APIC = 1;
	    if (ecx & (1<<29) )		cpu.cpu_features.F16C = 1;

	    /** CPU Instructions **/
	    if (edx & (1<<5) )		cpu.cpu_instructions.MSR = 1;
	    if (edx & (1<<8) )		cpu.cpu_instructions.CX8 = 1;
	    if (edx & (1<<11) )		cpu.cpu_instructions.SEP = 1;
	    if (edx & (1<<15) )		cpu.cpu_instructions.CMOV = 1;
	    if (edx & (1<<19) )		cpu.cpu_instructions.CLFLUSH = 1;
	    if (edx & (1<<23) )		cpu.cpu_instructions.MMX = 1;
	    if (edx & (1<<24) )		cpu.cpu_instructions.FXSR = 1;
	    if (edx & (1<<25) )		cpu.cpu_instructions.SSE = 1;
	    if (edx & (1<<26) )		cpu.cpu_instructions.SSE2 = 1;

	    if (ecx & (1<<0) )		cpu.cpu_instructions.SSE3 = 1;
	    if (ecx & (1<<1) )		cpu.cpu_instructions.PCLMUL = 1;
	    if (ecx & (1<<9) )		cpu.cpu_instructions.SSSE3 = 1;
	    if (ecx & (1<<12) )		cpu.cpu_instructions.FMA = 1;
	    if (ecx & (1<<13) )		cpu.cpu_instructions.CX16 = 1;
	    if (ecx & (1<<19) )		cpu.cpu_instructions.SSE41 = 1;
	    if (ecx & (1<<20) )		cpu.cpu_instructions.SSE42 = 1;
	    if (ecx & (1<<22) )		cpu.cpu_instructions.MOVBE = 1;
	    if (ecx & (1<<23) )		cpu.cpu_instructions.POPCNT = 1;
	    if (ecx & (1<<25) )		cpu.cpu_instructions.AESNI = 1;
	    if (ecx & (1<<26) )		cpu.cpu_instructions.XSAVE = 1;
	    if (ecx & (1<<27) )		cpu.cpu_instructions.OSXSAVE = 1;
	    if (ecx & (1<<28) )		cpu.cpu_instructions.AVX = 1;
	    if (ecx & (1<<30) )		cpu.cpu_instructions.RDRAND = 1; 
	    
	    /** CPU Miscellaneous Information **/
	    cpu.cpu_misc_info.BRAND_ID = (ebx & 0xFF);
	    cpu.cpu_misc_info.CLFLUSH_SIZE = (ebx & 0xF00);
	    cpu.cpu_misc_info.LOCAL_APIC_ID = (ebx & 0xFF000000);

	  if(highest_ext_info >= 0x80000004)
	  	get_processor_name(cpu.processor_name);
	}
}


void amd_init(){
	// TODO - Implment AMD things.
}
