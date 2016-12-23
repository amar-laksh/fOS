// TODO - SOLVE MESSED UP FEATURES TO ARRAY CONVERSION
#include <kernel/fos.h>
#define cpuid_simple(in, a, b, c, d) asm("cpuid": "=a" (a), "=b" (b), "=c" (c), "=d" (d) : "a" (in));

CPU_TABLE cpu_fs[52] =
	{
		{0,"FPU"},
		{0,"VME"},
		{0,"DE"},
		{0,"PSE"},
		{0,"PAE"},
		{0,"MCE"},
		{0,"APIC"},
		{0,"MTRR"},
		{0,"PGE"},
		{0,"MCA"},
		{0,"PAT"},
		{0,"PSE36"},
		{0,"PSN"},
		{0,"DS"},
		{0,"ACPI"},
		{0,"SS"},
		{0,"HTT"},
		{0,"TM"},
		{0,"PBE"},
		{0,"DTES64"},
		{0,"MONITOR"},
		{0,"DS_CPL"},
		{0,"VMX"},
		{0,"SMX"},
		{0,"EST"},
		{0,"TM2"},
		{0,"CNXT_ID"},
		{0,"XTPR"},
		{0,"PDCM"},
		{0,"PCID"},
		{0,"DCA"},
		{0,"X2APIC"},
		{0,"TSC"},
		{0,"F16C"},
		{0,"TOPO_EXT"},
		{0,"NODE_ID"},
		{0,"LWP"},
		{0,"WDT"},
		{0,"SKINT"},
		{0,"XOP"},
		{0,"IBS"},
		{0,"OSVW"},
		{0,"MIS_SSE"},
		{0,"ABM"},
		{0,"ALT_MOV_CR8"},
		{0,"XAPIC_SPACE"},
		{0,"SVM"},
		{0,"CMP_LEGACY"},
		{0,"THREE_DNOW_EXT"},
		{0,"LM"},
		{0,"FFXSR"},
		{0,"MMX_EXT"}
	};

CPU_TABLE cpu_ins[23] = {
	{0,"MSR"},
	{0,"CX8"},
	{0,"SEP"},
	{0,"CMOV"},
	{0,"CLFLUSH"},
	{0,"MMX"},
	{0,"FFXSR"},
	{0,"SSE"},
	{0,"SSE2"},
	{0,"SSE3"},
	{0,"PCLMUL"},
	{0,"SSSE3"},
	{0,"FMA"},
	{0,"CX16"},
	{0,"SSE41"},
	{0,"SSE42"},
	{0,"MOVBE"},
	{0,"POPCNT"},
	{0,"AESNI"},
	{0,"XSAVE"},
	{0,"OSXSAVE"},
	{0,"AVX"},
	{0,"RDRAND"}
};

uint32_t highest_std_info, highest_ext_info;
uint32_t eax, ebx, ecx, edx,crap;

char* vendor_strings[3] ={
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
	for (int i = 0; i < 52; ++i)
	{
		cpu.cpu_features[i] = cpu_fs[i];
	}

	for (int i = 0; i < 23; ++i)
	{
		cpu.cpu_instructions[i] = cpu_ins[i];
	}
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
	cpu.processor_serial_number[0] = edx;
	cpu.processor_serial_number[1] = ecx;
	
	/** Deterministic Cache Parameters **/
	cpuid(0x04,&eax, &ebx, &ecx, &edx);
	cpu.det_cache_params[0] = (eax & 0x1F);
	cpu.det_cache_params[1] = (eax & 0xE0);
	cpu.det_cache_params[2] = (eax & 0x100);
	cpu.det_cache_params[3] = (eax & 0x200);
	cpu.det_cache_params[4] = (eax & 0x3FFC000);
	cpu.det_cache_params[5] = (eax & 0xFC000000);
	cpu.det_cache_params[6] = (ebx & 0xFFF);
	cpu.det_cache_params[7] = (ebx & 0x3FF000);
	cpu.det_cache_params[8] = (ebx & 0xFFC00000);
	cpu.det_cache_params[9] = ecx;
	cpu.det_cache_params[10] = edx;

	/** cpu.monitor Features **/
	cpuid(0x05,&eax, &ebx, &ecx, &edx);
	cpu.monitor[0] = (eax & 0xFFFF);
	cpu.monitor[1] = (ebx & 0xFFFF);
	cpu.monitor[2] = (ecx & 0x1);
	cpu.monitor[3] = (ecx & 0x2);
	cpu.monitor[4] = edx;
	/** Function 0x01 **/
	if (highest_std_info >= 0x01){
	    cpuid(0x01, &eax, &ebx, &ecx, &edx);

	    /** Processor Signature **/
	    cpuid(0x01, &eax, &ebx, &ecx, &edx);
	    cpu.processor_signature[0] = (eax & 0xFF00000);
	    cpu.processor_signature[1] = (eax & 0xF00);
	    cpu.processor_signature[2] = (eax & 0xF0000);
	    cpu.processor_signature[3] = (eax & 0xF0);
	    cpu.processor_signature[4] = (eax & 0xF);
	    cpu.processor_signature[5] = (eax & 0x6000);
	    
	    /** CPU Features **/

	    CHECK_RANGE_FOR_BIT(0,3, edx, cpu_features);
	    CHECK_RANGE_FOR_BIT(5,7, edx, cpu_features);
	    CHECK_RANGE_FOR_BIT(9,9, edx, cpu_features);
	    CHECK_RANGE_FOR_BIT(12,14, edx, cpu_features);
	    CHECK_RANGE_FOR_BIT(16,18, edx, cpu_features);
	    CHECK_RANGE_FOR_BIT(21,22, edx, cpu_features);
	    CHECK_RANGE_FOR_BIT(27,29, edx, cpu_features);
	    CHECK_RANGE_FOR_BIT(31,31, edx, cpu_features);
	    CHECK_RANGE_FOR_BIT(21,22, edx, cpu_features);


	    CHECK_RANGE_FOR_BIT(2,8, ecx, cpu_features);
	    CHECK_RANGE_FOR_BIT(10,10, ecx, cpu_features);
	    CHECK_RANGE_FOR_BIT(14,15, ecx, cpu_features);
	    CHECK_RANGE_FOR_BIT(17,18, ecx, cpu_features);
	    CHECK_RANGE_FOR_BIT(21,21, edx, cpu_features);
	    CHECK_RANGE_FOR_BIT(29,29, edx, cpu_features);

	    /** CPU Instructions **/
	    CHECK_RANGE_FOR_BIT(5,5, edx, cpu_instructions);
	    CHECK_RANGE_FOR_BIT(8,8, edx, cpu_instructions);
	    CHECK_RANGE_FOR_BIT(11,11, edx, cpu_instructions);
	    CHECK_RANGE_FOR_BIT(15,15, edx, cpu_instructions);
	    CHECK_RANGE_FOR_BIT(19,19, edx, cpu_instructions);
	    CHECK_RANGE_FOR_BIT(23,26, edx, cpu_instructions);

	    CHECK_RANGE_FOR_BIT(0,1, ecx, cpu_instructions);
	    CHECK_RANGE_FOR_BIT(9,9, ecx, cpu_instructions);
	    CHECK_RANGE_FOR_BIT(12,13, ecx, cpu_instructions);
	    CHECK_RANGE_FOR_BIT(19,20, ecx, cpu_instructions);
	    CHECK_RANGE_FOR_BIT(22,23, ecx, cpu_instructions);
	    CHECK_RANGE_FOR_BIT(25,28, ecx, cpu_instructions);
	    CHECK_RANGE_FOR_BIT(30,30, ecx, cpu_instructions);
	    
	    /** CPU Miscellaneous Information **/
	    cpu.cpu_misc_info[0] = (ebx & 0xFF000000);
	    cpu.cpu_misc_info[1] = (ebx & 0xF00);
	    cpu.cpu_misc_info[2] = (ebx & 0xFF);

	    /* Extended Function CPUID Information */
	    cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
	    if(edx & (1<<29) ) 
	    	cpu.extended_cpu_features[3] |= 0x020000000;
	    
	    if(highest_ext_info >= 0x80000004)
	    	get_processor_name(cpu.processor_name);
	}
}

void amd_init(){
	// TODO - Implment AMD things.
	return;
}
