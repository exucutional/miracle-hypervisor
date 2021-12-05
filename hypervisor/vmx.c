#include "defines.h"
#include "inlines.h"
#include "serial_print.h"

int vmx_support_check()
{
    DEBUG(serial_print_str("Checking VMX mode support ...\n", COM1));
    BYTE rcx_val;
    asm volatile (
        "cpuid\r\n"
        :"=c"(rcx_val):"a"(1):"edx", "ebx"
    );

    DEBUG(serial_print_str(" -> Check CPUID.1:ECX.VMX[bit 5] is 1 ... ", COM1));    
    if(!(rcx_val & (1UL << 5))) {                       /* CPUID.1:ECX.VMX[bit 5] must be 1 */
        DEBUG(serial_print_str("Failed\n", COM1));
        return 0;
    }
    else DEBUG(serial_print_str("OK\n", COM1));

    QWORD msr_rd_val = rdmsr(IA32_FEATURE_CONTROL);

    DEBUG(serial_print_str(" -> IA32_FEATURE_CONTROL = 0x5 ... ", COM1));
    if((BYTE)(msr_rd_val & 0x5) != 0x5){                /* Bit 0 is the lock bit,Bit 2 enables VMXON outside SMX operation*/
        DEBUG(serial_print_str("Failed\n", COM1));
        return 0;
    }
    else DEBUG(serial_print_str("OK\n", COM1));
    
    msr_rd_val = rdmsr(IA32_VMX_BASIC);

    DEBUG(serial_print_str(" -> IA32_VMX_BASIC bit 55 is 1 ... ", COM1));
    if(!(msr_rd_val & (1UL << 55))){                    /* Bit 55 is read as 1 if any VMX controls that default to 1 may be cleared to 0.*/
        DEBUG(serial_print_str("Failed. All the default1 controls are reserved and must be 1\n", COM1));
        //return 0;
    }
    else DEBUG(serial_print_str("OK\n", COM1));

    DEBUG(serial_print_str(" -> IA32_VMX_BASIC VMXON size area is greater than 4096 ... ", COM1));
    if(((msr_rd_val >> 32) & 0x1FFF) > 0x1000){         /* my realisation support only 4096 VMXON area*/
        DEBUG(serial_print_str("Failed\n", COM1));
        return 0;
    }
    else DEBUG(serial_print_str("OK\n", COM1));

    DEBUG(serial_print_str(" -> IA32_VMX_BASIC memory type is writeback ... ", COM1));
    if(((msr_rd_val >> 50) & 0xF) != CACHE_TP_WB){      /* Bits 53:50 report the memory type ( must be 6 - WB ) */
        DEBUG(serial_print_str("Failed\n", COM1));
        return 0;
    }
    else DEBUG(serial_print_str("OK\n", COM1));

    return 1;
}

void vmx_init()
{
    DEBUG(serial_print_str("Initializing VMX mode ... ", COM1));
    DEBUG(serial_print_str("Done\n", COM1));
}
