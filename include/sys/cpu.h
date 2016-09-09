/* TODO- IMPLEMENT FUNCTION 0x02 FROM INTEL 
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
/* FUNCTION - 0x00*/
#define ECX_SSE3                        (1 << 0)    // Streaming SIMD Extensions 3
#define ECX_PCLMULQDQ                   (1 << 1)    // PCLMULQDQ Instruction
#define ECX_DTES64                      (1 << 2)    // 64-Bit Debug Store Area
#define ECX_MONITOR                     (1 << 3)    // MONITOR/MWAIT
#define ECX_DS_CPL                      (1 << 4)    // CPL Qualified Debug Store
#define ECX_VMX                         (1 << 5)    // Virtual Machine Extensions
#define ECX_SMX                         (1 << 6)    // Safer Mode Extensions
#define ECX_EST                         (1 << 7)    // Enhanced SpeedStep Technology
#define ECX_TM2                         (1 << 8)    // Thermal Monitor 2
#define ECX_SSSE3                       (1 << 9)    // Supplemental Streaming SIMD Extensions 3
#define ECX_CNXT_ID                     (1 << 10)   // L1 Context ID
#define ECX_FMA                         (1 << 12)   // Fused Multiply Add
#define ECX_CX16                        (1 << 13)   // CMPXCHG16B Instruction
#define ECX_XTPR                        (1 << 14)   // xTPR Update Control
#define ECX_PDCM                        (1 << 15)   // Perf/Debug Capability MSR
#define ECX_PCID                        (1 << 17)   // Process-context Identifiers
#define ECX_DCA                         (1 << 18)   // Direct Cache Access
#define ECX_SSE41                       (1 << 19)   // Streaming SIMD Extensions 4.1
#define ECX_SSE42                       (1 << 20)   // Streaming SIMD Extensions 4.2
#define ECX_X2APIC                      (1 << 21)   // Extended xAPIC Support
#define ECX_MOVBE                       (1 << 22)   // MOVBE Instruction
#define ECX_POPCNT                      (1 << 23)   // POPCNT Instruction
#define ECX_TSC                         (1 << 24)   // Local APIC supports TSC Deadline
#define ECX_AESNI                       (1 << 25)   // AESNI Instruction
#define ECX_XSAVE                       (1 << 26)   // XSAVE/XSTOR States
#define ECX_OSXSAVE                     (1 << 27)   // OS Enabled Extended State Management
#define ECX_AVX                         (1 << 28)   // AVX Instructions
#define ECX_F16C                        (1 << 29)   // 16-bit Floating Point Instructions
#define ECX_RDRAND                      (1 << 30)   // RDRAND Instruction
/* FUNCTION - 0x01*/
#define EDX_FPU                         (1 << 0)    // Floating-Point Unit On-Chip
#define EDX_VME                         (1 << 1)    // Virtual 8086 Mode Extensions
#define EDX_DE                          (1 << 2)    // Debugging Extensions
#define EDX_PSE                         (1 << 3)    // Page Size Extension
#define EDX_TSC                         (1 << 4)    // Time Stamp Counter
#define EDX_MSR                         (1 << 5)    // Model Specific Registers
#define EDX_PAE                         (1 << 6)    // Physical Address Extension
#define EDX_MCE                         (1 << 7)    // Machine-Check Exception
#define EDX_CX8                         (1 << 8)    // CMPXCHG8 Instruction
#define EDX_APIC                        (1 << 9)    // APIC On-Chip
#define EDX_SEP                         (1 << 11)   // SYSENTER/SYSEXIT instructions
#define EDX_MTRR                        (1 << 12)   // Memory Type Range Registers
#define EDX_PGE                         (1 << 13)   // Page Global Bit
#define EDX_MCA                         (1 << 14)   // Machine-Check Architecture
#define EDX_CMOV                        (1 << 15)   // Conditional Move Instruction
#define EDX_PAT                         (1 << 16)   // Page Attribute Table
#define EDX_PSE36                       (1 << 17)   // 36-bit Page Size Extension
#define EDX_PSN                         (1 << 18)   // Processor Serial Number
#define EDX_CLFLUSH                     (1 << 19)   // CLFLUSH Instruction
#define EDX_DS                          (1 << 21)   // Debug Store
#define EDX_ACPI                        (1 << 22)   // Thermal Monitor and Software Clock Facilities
#define EDX_MMX                         (1 << 23)   // MMX Technology
#define EDX_FXSR                        (1 << 24)   // FXSAVE and FXSTOR Instructions
#define EDX_SSE                         (1 << 25)   // Streaming SIMD Extensions
#define EDX_SSE2                        (1 << 26)   // Streaming SIMD Extensions 2
#define EDX_SS                          (1 << 27)   // Self Snoop
#define EDX_HTT                         (1 << 28)   // Multi-Threading
#define EDX_TM                          (1 << 29)   // Thermal Monitor
#define EDX_PBE                         (1 << 31)   // Pending Break Enable


#define EDX_SYSCALL                     (1 << 11)   // SYSCALL/SYSRET
#define EDX_XD                          (1 << 20)   // Execute Disable Bit
#define EDX_1GB_PAGE                    (1 << 26)   // 1 GB Pages
#define EDX_RDTSCP                      (1 << 27)   // RDTSCP and IA32_TSC_AUX
#define EDX_64_BIT                      (1 << 29)   // 64-bit Architecture

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
	int8_t TSCOUNT;    
	int8_t F16C; 
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

struct PROCESSOR_SIGNATURE{
	int8_t EXTENDED_FAMILY;
	int8_t FAMILY_CODE;
	int8_t EXTENDED_MODEL;
	int8_t MODEL_NUMBER;
	int8_t STEPPING_ID;
	int8_t TYPE;

};

struct CPU_INSTRUCTION{
	int8_t TSC;
	int8_t FXSR;
	int8_t MMX;
	int8_t CLFLUSH;
	int8_t FMA;
	int8_t MONITOR;
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
	int8_t PCLMULQDQ;
	int8_t SEP;
	int8_t TBM;
	int8_t THREE_DNOW_PRE;
	int8_t SSE4A;
	int8_t THREE_DNOW_INSTR;
	int8_t RDTSCP;
};

struct CPU_MISC_INFO{
	int8_t LOCAL_APIC_ID;
	int8_t LOG_PRO_COUNT;
	int8_t CLFLUSH_SIZE;
	int8_t BRAND_ID;
};

struct PROCESSOR_SERIAL_NUMBER{
	uint64_t UPPERBITS;
	uint32_t LOWERBITS;
};

struct DETERMINSTIC_CACHE_PARAMETERS{
	int8_t NO_APCID;
	int16_t NO_MAXTHREAD;
	int16_t FAC;
	int8_t SICL;
	int8_t CACHE_TYPE;
	int16_t WOA;
	int16_t PARTITIONS;
	int16_t LINE_SIZE;
	int32_t SETS;
	int8_t INC_LC;
	int8_t WBINVD;
};

struct MONITOR{
	int16_t MIN_MONS_SIZE;
	int16_t MAX_MONS_SIZE;
	int8_t IBE;
	int8_t EMX;
	int8_t NO_C7_C4;
	int8_t NO_C6_C3;
	int8_t NO_C2;
	int8_t NO_C1;
	int8_t NO_C0;	
};

struct DIGTSPM{
	int8_t INTEL_TB;
	int8_t DIG_T_SENS;
	int8_t NO_INTR_THRES;
	int8_t HCFC;
	int8_t EFF_FREQ;
};

struct DCA {
	int32_t PLATFORM_DCA_CAP;
};

struct ARCH_PER_MON_FEATURES {
	int8_t ARCH_EVENTS;
	int8_t BITS_PC;
	int8_t NO_COUNTER;
	int8_t PERFMON_VER;
	int8_t BRANCH_MISS;
	int8_t BRANCH_INST;
	int8_t LL_CACHE_MISS;
	int8_t LL_CACHE_REF;
	int8_t EBX_REF_CYCLES;
	int8_t INSTR_RETD;
	int8_t CORE_CYCLES;
	int8_t BITS_FC;
	int8_t NO_FC;
};

struct PROCESSOR_TOPOLOGY {
	int8_t BITS_RS_APIC;
	int8_t NO_FC_LOG_PRO;
	int8_t LEVEL_TYPE;
	int8_t LEVEL_NUMBER;
	int32_t EXT_APIC_ID;	
};

struct XSAVE {
	int32_t LOWER_32_BITS_XCR0;
	int32_t MAX_BYTES_XCR0;
	int32_t MAX_BYTES_XSAVE;
	int32_t UPPER_32_BITS_XCR0;
	int32_t YMM_SAVE_STATE_OFFSET;
	int32_t LWP_SAVE_STATE_SIZE;
	int32_t LWP_SAVE_STATE_OFFSET;
};


// Extended Functions 

struct EXT_CPU_FEATURE{
	int8_t BMI;
	int8_t SYSCALL;
	int8_t XD_BIT;
	int8_t INTEL_64;
	int8_t LAHF_OR_SAHF;
};

struct PROCESSOR_NAME{
	int64_t FIRST_16;
	int64_t SECOND_16;
	int64_t THIRD_16;
};

struct EXT_L2_CACHE {
	int16_t L2_SIZE;
	int8_t L2_ASSOC;
	int8_t L2_LINE_SIZE;
};

struct ADV_POW_MANG{
	int8_t TSC_INVAR;
};

struct VIR_PHY_ADDR_SIZE{
	int8_t VIRT_ADDR_SIZE;
	int8_t PHY_ADDR_SIZE;
};

struct CPU_TOPOLOGY{
	/* 				STANDARD FUNCTIONS 			*/

	// Function 0x00 | Function 0x00
	char cpu_vendor_id[32];
	uint32_t highest_std_info;

	// Function 0x01 | Function 0x01
	struct PROCESSOR_SIGNATURE processor_signature;
	struct CPU_FEATURE  cpu_features;
	struct CPU_INSTRUCTION cpu_instructions;
	struct CPU_MISC_INFO cpu_mis_informtion;

	// Function 0x03 
	struct PROCESSOR_SERIAL_NUMBER psn;

	// Funciton 0x04
	struct DETERMINSTIC_CACHE_PARAMETERS determinstic_cache_params;

	// Function 0x05 | Function 0x05
	struct MONITOR monitor_mwait;

	// Function 0x06 | Function 0x06
	struct DIGTSPM dig_sensor_pm;

	// Function 0x09 
	struct DCA direct_cache;

	// Function 0x0A
	struct ARCH_PER_MON_FEATURES  arcpmf;

	// Function 0x0B
	struct PROCESSOR_TOPOLOGY processor_topology;

	// Function 0x0D | Function 0x0D
	struct XSAVE processor_ext_state_enum;


	/* 				EXTENDED FUNCTIONS 			*/

	uint32_t highest_ext_info;	

	// Function 0x8000_0001 | Function 0x0000_0007
	struct EXT_CPU_FEATURE extended_cpu_features;

	// Function 0x8000_0002, 0x8000_0003, 0x800_0004
	struct PROCESSOR_NAME processor_name;	

	// Function 0x8000_0006 
	struct EXT_L2_CACHE extended_L2_cache;

	// Function 0x8000_0007
	struct ADV_POW_MANG advance_power_management;

	// Function 0x8000_Z0008
	struct VIR_PHY_ADDR_SIZE virtual_physcial_address;


};


static inline void cpuid(uint32_t reg, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx)
{
    __asm__ volatile("cpuid"
        : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx)
        : "0" (reg));
}

void cpuid_get_vendor_string(char* where);

void cpu_init();

#endif
