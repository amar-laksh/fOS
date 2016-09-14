// TODO - IMPLEMENT DECLARATION AND INITIALIZATION OF STRUCTS IN CPU.H
// TODO - IMPLEMENT CACHE AND TLB IMPLEMENTATION

#include <sys/cpu.h>
#define cpuid_simple(in, a, b, c, d) asm("cpuid": "=a" (a), "=b" (b), "=c" (c), "=d" (d) : "a" (in));


struct CPU_FEATURE cpu_features;
struct CPU_INSTRUCTION cpu_instructions;
struct PROCESSOR_SIGNATURE processor_signature;
struct CPU_MISC_INFO cpu_misc_info;
struct PROCESSOR_SERIAL_NUMBER processor_serial_number;
struct DET_CACHE_PARAMS det_cache_params;
struct MONITOR monitor;
CPU_TOPOLOGY cpu;


uint32_t highest_std_info, highest_ext_info;
uint32_t eax, ebx, ecx, edx,crap;
  
char cpu_name[41];
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


	/** Processor Signature **/
	cpuid(0x01, &eax, &ebx, &ecx, &edx);
	processor_signature.STEPPING_ID = (eax & 0xF);
	processor_signature.MODEL_NUMBER = (eax & 0xF0);
	processor_signature.FAMILY_CODE = (eax & 0xF00);
	processor_signature.PROCESSOR_TYPE = (eax & 0x6000);
	processor_signature.EXTENDED_MODEL_ID = (eax & 0xF0000);
	processor_signature.EXTENDED_FAMILY_ID = (eax & 0xFF00000);


	/** CPU Miscellaneous Information **/
	cpu_misc_info.BRAND_ID = (ebx & 0xFF);
	cpu_misc_info.CLFLUSH_SIZE = (ebx & 0xF00);
	cpu_misc_info.LOCAL_APIC_ID = (ebx & 0xFF000000);


	/** Processor Serial Number **/
	cpuid(0x03,&eax, &ebx, &ecx, &edx);
	processor_serial_number.LOWER_BITS = ecx;
	processor_serial_number.UPPER_BITS = edx;

	/** Deterministic Cache Parameters **/
	cpuid(0x04,&eax, &ebx, &ecx, &edx);
	det_cache_params.CACHE_TYPE = (eax & 0x1F);
	det_cache_params.CACHE_LEVEL = (eax & 0xE0);
	det_cache_params.SINIT_CACHE_LVL = (eax & 0x100);
	det_cache_params.FULLY_ASSOC_CACHE = (eax & 0x200);
	det_cache_params.MAX_IDS_LOG_PRO = (eax & 0x3FFC000);
	det_cache_params.MAX_IDS_PRO_CORES = (eax & 0xFC000000);
	det_cache_params.SYS_COH_LINE_SIZE = (ebx & 0xFFF);
	det_cache_params.PHY_LINE_PART = (ebx & 0x3FF000);
	det_cache_params.WAYS_OF_ASSOC = (ebx & 0xFFC00000);
	det_cache_params.NO_OF_SETS = ecx;
	det_cache_params.WBINVD_COMPACT = edx;


	/** MONITOR Features **/
	cpuid(0x05,&eax, &ebx, &ecx, &edx);
	monitor.MIN_MONS_SIZE = (eax & 0xFFFF);
	monitor.MAX_MONS_SIZE = (ebx & 0xFFFF);
	monitor.EMX = (ecx & 0x1);
	monitor.INTR_BRK_EVENT = (ecx & 0x2);
	monitor.C0_C7 = edx;

	/** CPU Instructions and Features **/
	if (highest_std_info >= 0x01){

		/** Getting CPU Features **/
	    cpuid(0x01, &eax, &ebx, &ecx, &edx); 

	    if (edx & (1<<0) )		cpu_features.FPU = 1;
	    if (edx & (1<<1) )		cpu_features.VME = 1;
	    if (edx & (1<<2) )		cpu_features.DE = 1;
	    if (edx & (1<<3) )		cpu_features.PSE = 1;
	    if (edx & (1<<5) )		cpu_features.TSC = 1;
	    if (edx & (1<<6) )		cpu_features.PAE = 1;
	    if (edx & (1<<7) )		cpu_features.MCE = 1;
	    if (edx & (1<<9) )		cpu_features.APIC = 1;
	    if (edx & (1<<12) )		cpu_features.MTRR = 1;
	    if (edx & (1<<13) )		cpu_features.PGE = 1;
	    if (edx & (1<<14) )		cpu_features.MCA = 1;

	    if (edx & (1<<16) )		cpu_features.PAT = 1;
	    if (edx & (1<<17) )		cpu_features.PSE36 = 1;
	    if (edx & (1<<18) )		cpu_features.PSN = 1;
	    if (edx & (1<<21) )		cpu_features.DS = 1;
	    if (edx & (1<<22) )		cpu_features.ACPI = 1;
	    if (edx & (1<<27) )		cpu_features.SS = 1;
	    if (edx & (1<<28) )		cpu_features.HTT = 1;
	    if (edx & (1<<29) )		cpu_features.TM = 1;
	    if (edx & (1<<31) )		cpu_features.PBE = 1;

	    if (ecx & (1<<2) )		cpu_features.DTES64 = 1;
	    if (ecx & (1<<3) )		cpu_features.MONITOR = 1;
	    if (ecx & (1<<4) )		cpu_features.DS_CPL = 1;
	    if (ecx & (1<<5) )		cpu_features.VMX = 1;
	    if (ecx & (1<<6) )		cpu_features.SMX = 1;
	    if (ecx & (1<<7) )		cpu_features.EST = 1;
	    if (ecx & (1<<8) )		cpu_features.TM2 = 1;
	    if (ecx & (1<<10) )		cpu_features.CNXT_ID = 1;
	    if (ecx & (1<<14) )		cpu_features.XTPR = 1;
	    if (ecx & (1<<15) )		cpu_features.PDCM = 1;

	    if (ecx & (1<<17) )		cpu_features.PCID = 1;
	    if (ecx & (1<<18) )		cpu_features.DCA = 1;
	    if (ecx & (1<<21) )		cpu_features.X2APIC = 1;
	    if (ecx & (1<<29) )		cpu_features.F16C = 1;

	    /** Getting CPU Instructions **/
	    if (edx & (1<<5) )		cpu_instructions.MSR = 1;
	    if (edx & (1<<8) )		cpu_instructions.CX8 = 1;
	    if (edx & (1<<11) )		cpu_instructions.SEP = 1;
	    if (edx & (1<<15) )		cpu_instructions.CMOV = 1;
	    if (edx & (1<<19) )		cpu_instructions.CLFLUSH = 1;
	    if (edx & (1<<23) )		cpu_instructions.MMX = 1;
	    if (edx & (1<<24) )		cpu_instructions.FXSR = 1;
	    if (edx & (1<<25) )		cpu_instructions.SSE = 1;
	    if (edx & (1<<26) )		cpu_instructions.SSE2 = 1;

	    if (ecx & (1<<0) )		cpu_instructions.SSE3 = 1;
	    if (ecx & (1<<1) )		cpu_instructions.PCLMUL = 1;
	    if (ecx & (1<<9) )		cpu_instructions.SSSE3 = 1;
	    if (ecx & (1<<12) )		cpu_instructions.FMA = 1;
	    if (ecx & (1<<13) )		cpu_instructions.CX16 = 1;
	    if (ecx & (1<<19) )		cpu_instructions.SSE41 = 1;
	    if (ecx & (1<<20) )		cpu_instructions.SSE42 = 1;
	    if (ecx & (1<<22) )		cpu_instructions.MOVBE = 1;
	    if (ecx & (1<<23) )		cpu_instructions.POPCNT = 1;
	    if (ecx & (1<<25) )		cpu_instructions.AESNI = 1;
	    if (ecx & (1<<26) )		cpu_instructions.XSAVE = 1;
	    if (ecx & (1<<27) )		cpu_instructions.OSXSAVE = 1;
	    if (ecx & (1<<28) )		cpu_instructions.AVX = 1;
	    if (ecx & (1<<30) )		cpu_instructions.RDRAND = 1; 

	  }
	  struct CPU_FEATURE ptr = cpu_features;

	  for(int i=0;i<4;i++){
	  	if(i==3){
	  		if(&ptr == 1){
	  			write_str("PAE feature is supported!");
	  		}
	  	}
	  }

	  if(highest_ext_info >= 0x80000004){
	    get_processor_name(cpu_name);
	    write_str("CPU name:");
	    write_str(cpu_name);
	  }
}


void amd_init(vendor_magic_no){
	// TODO - Implment AMD things.
	write_str("AMD welcomes you");
}
