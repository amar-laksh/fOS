/* TODO- IMPLEMENT FUNCTION 0x02 FROM INTEL 
*  TODO- RE-IMPLEMENT: merge 1 Bit fields into higher data-types 
*  TODO- Implmenet AMD parts some time later.
*  TODO- WRITE AMD FUNCTION COMMENTS IN FORM-
*		INTEL_FUNCITON | AMD_FUNCTION
*
*	.E.G.- Function 0x01 | Function 0x0D
*
*/

#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include <sys/vga.h>
#include <sys/io.h>
#include <stdlib/string.h>
/* FUNCTION - 0x00*/

struct CPU_FEATURE{
	int8_t FPU;
	int8_t VME;
	int8_t DE; 
	int8_t PSE;
	int8_t PAE;
	int8_t MCE;
	int8_t APIC;
	int8_t MTRR;
	int8_t PGE; 
	int8_t MCA; 
	int8_t PAT; 
	int8_t PSE36;
	int8_t PSN;  
	int8_t DS;     
	int8_t ACPI;    
	int8_t SS;   
	int8_t HTT;  
	int8_t TM;   
	int8_t PBE;  
	int8_t DTES64;
	int8_t MONITOR; 
	int8_t DS_CPL; 
	int8_t VMX;    
	int8_t SMX;    
	int8_t EST;    
	int8_t TM2;      
	int8_t CNXT_ID;     
	int8_t XTPR;  
	int8_t PDCM;  
	int8_t PCID;  
	int8_t DCA;   
	int8_t X2APIC; 
	int8_t TSC;    
	int8_t F16C;
	// Probably AMD features 
	int8_t TOPO_EXT;
	int8_t NODE_ID;
	int8_t LWP;
	int8_t WDT;
	int8_t SKINT;
	int8_t XOP;
	int8_t IBS;
	int8_t OSVW;
	int8_t MIS_SSE;
	int8_t ABM;
	int8_t ALT_MOV_CR8;
	int8_t XAPIC_SPACE;
	int8_t SVM;
	int8_t CMP_LEGACY;
	int8_t THREE_DNOW_EXT;
	int8_t LM;
	int8_t FFXSR;
	int8_t MMX_EXT;

};



struct CPU_INSTRUCTION{
	int8_t FXSR;
	int8_t MMX;
	int8_t CLFLUSH;
	int8_t FMA;
	int8_t CX8;
	int8_t CX16;
	int8_t MSR;
	int8_t SSE;
	int8_t SSE2;
	int8_t SSE3;
	int8_t SSSE3;
	int8_t SSE41;
	int8_t SSE42;
	int8_t AVX;
	int8_t RDRAND;
	int8_t CMOV;
	int8_t XSAVE; 
	int8_t OSXSAVE;
	int8_t MOVBE;
	int8_t POPCNT; 
	int8_t AESNI;
	int8_t PCLMUL;
	int8_t SEP;
	// Probably AMD Instructions
	int8_t TBM;
	int8_t THREE_DNOW_PRE;
	int8_t SSE4A;
	int8_t THREE_DNOW_INSTR;
	int8_t RDTSCP;
};

typedef struct {
	/* 				STANDARD FUNCTIONS 			*/

	// Function 0x00 | Function 0x00
	char cpu_vendor_string[12];
	char cpu_vendor_id[32];
	uint32_t highest_std_info;

	// Function 0x01 | Function 0x01
// struct PROCESSOR_SIGNATURE{
// 	int8_t EXTENDED_FAMILY_ID;
// 	int8_t FAMILY_CODE;
// 	int8_t EXTENDED_MODEL_ID;
// 	int8_t MODEL_NUMBER;
// 	int8_t STEPPING_ID;
// 	int8_t PROCESSOR_TYPE;
// };
	int16_t processor_signature[6];
	
	struct CPU_FEATURE  cpu_features;

	struct CPU_INSTRUCTION cpu_instructions;
// struct CPU_MISC_INFO{
// 	int8_t LOCAL_APIC_ID;
// 	int8_t CLFLUSH_SIZE;
// 	int8_t BRAND_ID;
// 	// Probably AMD information
// 	int8_t LOG_PRO_COUNT;
// };
	int16_t cpu_misc_info[4];

	// Function 0x03 
// struct PROCESSOR_SERIAL_NUMBER{
// 	int32_t UPPER_BITS;
// 	int32_t LOWER_BITS;
// };
	int32_t processor_serial_number[2];

	// Funciton 0x04
// struct DET_CACHE_PARAMS{
// 	int8_t CACHE_TYPE;
// 	int8_t CACHE_LEVEL;
// 	int8_t SINIT_CACHE_LVL;
// 	int8_t FULLY_ASSOC_CACHE;
// 	int8_t MAX_IDS_LOG_PRO;
// 	int8_t MAX_IDS_PRO_CORES;
// 	int16_t SYS_COH_LINE_SIZE;
// 	int16_t PHY_LINE_PART;
// 	int16_t WAYS_OF_ASSOC;
// 	int32_t NO_OF_SETS;
// 	int8_t WBINVD_COMPACT;
// };
	int32_t det_cache_params[11];

	// Function 0x05 | Function 0x05
// struct MONITOR{
// 	int16_t MIN_MONS_SIZE;
// 	int16_t MAX_MONS_SIZE;
// 	int8_t EMX;
// 	int8_t INTR_BRK_EVENT;
// 	int32_t C0_C7;
// };
	int32_t monitor[5];

	// Function 0x06 | Function 0x06
// struct DIGTSPM{
// 	int16_t THERMAL_COMPACT;
// 	int8_t NO_INTR_THRESH;
// 	int8_t HW_FEEDBACK_PERF_ENERGY_COMPACT;
// };
	int16_t dig_sensor_pm[3];

	// Function 0x09
// struct DCA {
// 	int32_t PLATFORM_DCA_CAP;
// };
	int32_t direct_cache[1];

	// Function 0x0A
// struct ARCH_PER_MON_FEATURES {
// 	int8_t VERSION_ID;
// 	int8_t NO_GP_PERF_COUNTER;
// 	int8_t BITS_WIDTH_GP_PERF_COUNTER;
// 	int8_t LEN_EBX_BIT_VECTOR;
// 	int8_t CORE_CYCLE_COMPACT;
// 	int16_t FIXED_FUNC_COMPACT; 
// };
	int16_t arcpmf[6];

	// Function 0x0B
// struct PROCESSOR_TOPOLOGY {
// 	int8_t BITS_RS_APIC;
// 	int8_t NO_FC_LOG_PRO;
// 	int8_t LEVEL_NUMBER;
// 	int8_t LEVEL_TYPE;
// 	int32_t EXT_APIC_ID;	
// };
	int32_t processor_topology[5];

	// Function 0x0D | Function 0x0D
// struct XSAVE {
// 	int32_t LOWER_32_BITS_XCR0;
// 	int32_t MAX_BYTES_XCR0;
// 	int32_t MAX_BYTES_XSAVE;
// 	int32_t UPPER_32_BITS_XCR0;
// 	int32_t XSAVE_STATES_SIZE;
// 	int32_t LOWER_XSS_OFFSET;
// 	int8_t IA32_XSS;
// 	int32_t UPPER_XSS_OFFSET;
// 	int32_t SAVE_AREA_SIZE;
// 	int32_t BEGIN_XSAVE_OFFSET;
// 	int8_t BIT0_SUB_LEAF_CHECK;
// };
	int32_t processor_ext_state_enum[11];

	/* 				EXTENDED FUNCTIONS 			*/

	uint32_t highest_ext_info;	

	// Function 0x8000_0001 | Function 0x0000_0007
// struct EXT_CPU_FEATURE{
// 	int32_t EXT_FEATURES;
// };
	int32_t extended_cpu_features[4];

	// Function 0x8000_0002, 0x8000_0003, 0x800_0004
	char processor_name[41];	

	// Function 0x8000_0006
// struct EXT_L2_CACHE {
// 	int16_t L2_SIZE;
// 	int8_t L2_ASSOC;
// 	int8_t L2_LINE_SIZE;
// };
	int16_t extended_L2_cache[3];

	// Function 0x8000_0007
// struct ADV_POW_MANG{
// 	int8_t TSC_INVAR;
// };
	int8_t advance_power_management[1];

	// Function 0x8000_Z0008
// struct VIR_PHY_ADDR_SIZE{
// 	int8_t VIRT_ADDR_SIZE;
// 	int8_t PHY_ADDR_SIZE;
// };
	int8_t virtual_physcial_address[2];


} CPU_TOPOLOGY;


CPU_TOPOLOGY cpu;



typedef struct {
	char* arc;
	char* op_modes;
	char* byte_order;
	char* cpus;
	char* online_cpu_list;
	char* threads_per_core;
	char* cores_per_socket;
	char* sockets;
	char* NUMA_nodes;
	char* vendor_id;
	char* cpu_family;
	char* model;
	char* model_name;
	char* stepping;
	char* cpu_mhz;
	char* cpu_max_mhz;
	char* cpu_min_mhz;
	char* bogo_MIPS;
	char* virtualization;
	char* l1d_cache;
	char* l1i_cache;
	char* l2_cache;
	char* l3_cache;
	char* flags[100];
} LSCPU;

LSCPU cpu_t;
static inline void cpuid(uint32_t reg, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx)
{
    __asm__ __volatile__("cpuid"
        : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx)
        : "0" (reg));
}

void cpu_init();
void intel_init();
void amd_init();

#endif
