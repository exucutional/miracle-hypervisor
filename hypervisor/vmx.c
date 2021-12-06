#include "defines.h"
#include "inlines.h"
#include "serial_print.h"
#include "vmx.h"

extern vmm_t vmm;

int vmx_support_check()
{
    DEBUG(serial_print_str("Checking VMX mode support ...\n", COM1));
    BYTE rcx_val;
    asm volatile (
        "cpuid\r\n"
        :"=c"(rcx_val):"a"(1):"edx", "ebx"
    );

    DEBUG(serial_print_str(" -> CPUID.1:ECX.VMX[bit 5] is 1 ... ", COM1));
    if (!(rcx_val & (1UL << 5)))
    {/* CPUID.1:ECX.VMX[bit 5] must be 1 */
        DEBUG(serial_print_str("Failed\n", COM1));
        return FALSE;
    }
    else DEBUG(serial_print_str("OK\n", COM1));

    QWORD msr_rd_val = rdmsr(IA32_FEATURE_CONTROL);

    DEBUG(serial_print_str(" -> IA32_FEATURE_CONTROL = 0x5 ... ", COM1));
    if ((BYTE)(msr_rd_val & 0x5) != 0x5)
    {/* Bit 0 is the lock bit,Bit 2 enables VMXON outside SMX operation*/
        DEBUG(serial_print_str("Failed\n", COM1));
        return FALSE;
    }
    else DEBUG(serial_print_str("OK\n", COM1));
    
    msr_rd_val = rdmsr(IA32_VMX_BASIC);

    DEBUG(serial_print_str(" -> IA32_VMX_BASIC bit 55 is 1 ... ", COM1));
    if (!(msr_rd_val & (1UL << 55)))
    {/* Bit 55 is read as 1 if any VMX controls that default to 1 may be cleared to 0.*/
        DEBUG(serial_print_str("Failed. All the default1 controls are reserved and must be 1\n", COM1));
        //return 0;
    }
    else DEBUG(serial_print_str("OK\n", COM1));

    DEBUG(serial_print_str(" -> IA32_VMX_BASIC VMXON size area is greater than 4096 ... ", COM1));
    if (((msr_rd_val >> 32) & 0x1FFF) > 0x1000)
    {/* my realisation support only 4096 VMXON area*/
        DEBUG(serial_print_str("Failed\n", COM1));
        return FALSE;
    }
    else DEBUG(serial_print_str("OK\n", COM1));

    DEBUG(serial_print_str(" -> IA32_VMX_BASIC memory type is writeback ... ", COM1));
    if (((msr_rd_val >> 50) & 0xF) != CACHE_TP_WB)
    {/* Bits 53:50 report the memory type ( must be 6 - WB ) */
        DEBUG(serial_print_str("Failed\n", COM1));
        return FALSE;
    }
    else DEBUG(serial_print_str("OK\n", COM1));

    return TRUE;
}

int vmx_init()
{
    DEBUG(serial_print_str("Initializing VMX mode ...\n", COM1));
    //vmm_t vmm;
    //DWORD* ptrVMXON_addr = (DWORD*)VMXON_ADDR;
    DWORD* ptrVMXON_addr = vmm.vmxon_region;
    QWORD msr_rd_val = rdmsr(IA32_VMX_BASIC);
    
    // Initialize the version identifier in the VMXON region (the first 31 bits)
    DEBUG(serial_print_str(" -> Initializing the version identifier in the VMXON region at addr 0x", COM1));
    DEBUG(serial_print_num((QWORD)ptrVMXON_addr, COM1));
    DEBUG(serial_print_str(" ... ", COM1));
    ptrVMXON_addr[0] = (DWORD)(msr_rd_val & 0xFFFFFFFF);    
    DEBUG(serial_print_str("OK\n", COM1));

    // Ensure the current processor operating mode meets the required CR0 fixed bits
    DEBUG(serial_print_str(" -> Setting CR0 bits according to the IA32_VMX_CR0_FIXED0/1 ... ", COM1));
    msr_rd_val = rdmsr(IA32_VMX_CR0_FIXED0);
    QWORD cr0_fixed0 = msr_rd_val & 0xFFFFFFFF;    

    msr_rd_val = rdmsr(IA32_VMX_CR0_FIXED1);
    QWORD cr0_fixed1 = msr_rd_val & 0xFFFFFFFF;

    QWORD cr0_val = rdCR0();
    cr0_val = (cr0_val & cr0_fixed1) | cr0_fixed0;
    wrCR0(cr0_val);
    DEBUG(serial_print_str("OK\n", COM1));

    //Ensure the resultant CR4 value supports all the CR4 fixed bits
    DEBUG(serial_print_str(" -> Setting CR4 bits according to the IA32_VMX_CR4_FIXED0/1 ... ", COM1));
    msr_rd_val = rdmsr(IA32_VMX_CR4_FIXED0);
    QWORD cr4_fixed0 = msr_rd_val & 0xFFFFFFFF;    
    
    msr_rd_val = rdmsr(IA32_VMX_CR4_FIXED1);
    QWORD cr4_fixed1 = msr_rd_val & 0xFFFFFFFF;
    
    QWORD cr4_val = rdCR4();
    cr4_val =  (cr4_val & cr4_fixed1) | cr4_fixed0;
    wrCR4(cr4_val);
    DEBUG(serial_print_str("OK\n", COM1));

    DEBUG(serial_print_str(" -> Enabling VMX operation by setting CR4.VMXE[bit 13] to 1 ... ", COM1));
    cr4_val = cr4_val | (1UL << 13);                                // Enable VMX operation by setting CR4.VMXE = 1
    wrCR4(cr4_val);
    DEBUG(serial_print_str("OK\n", COM1));
    
    // Execute VMXON with the physical address of the VMXON region as the operand.
    DEBUG(serial_print_str(" -> Executing VMXON at addr 0x", COM1));
    DEBUG(serial_print_num((QWORD)ptrVMXON_addr, COM1));
    DEBUG(serial_print_str(" ... ", COM1));
    if(!vmxon(vmm.vmxon_region))
    {
        DEBUG(serial_print_str("Failed\n", COM1));
        return FALSE;
    }
    DEBUG(serial_print_str("OK\n", COM1));
    

    // Clear VMCS area
    DEBUG(serial_print_str(" -> Clearing VMCS area at address 0x", COM1));
    //QWORD *ptrVMCS_addr = (QWORD*)VMCS_ADDR;
    QWORD* ptrVMCS_addr = vmm.vmcs_region;
    DEBUG(serial_print_num((QWORD)ptrVMCS_addr, COM1));
    DEBUG(serial_print_str(" ... ", COM1));
    for(int i = 0; i < 4096 / 8; i++)
        ptrVMCS_addr[i] = 0;

    DEBUG(serial_print_str("OK\n", COM1));

    // Initialize the version identifier in the VMCS (first 31 bits)
    DEBUG(serial_print_str(" -> Initializing the version identifier in the VMCS(0..31 bits) ... ", COM1));
    msr_rd_val = rdmsr(IA32_VMX_BASIC);
    *(DWORD*)(vmm.vmcs_region) = (DWORD)(msr_rd_val & 0xFFFFFFFF);
    DEBUG(serial_print_str("OK\n", COM1));

    // Execute the VMCLEAR
    DEBUG(serial_print_str(" -> Executing VMCLEAR at addr 0x", COM1));
    DEBUG(serial_print_num((QWORD)ptrVMCS_addr, COM1));
    DEBUG(serial_print_str(" ... ", COM1));
    if(!vmclear(vmm.vmcs_region))
    {
        DEBUG(serial_print_str("Failed\n", COM1));
        return FALSE;
    }
    DEBUG(serial_print_str("OK\n", COM1));
    
    // Execute the VMPTRLD
    DEBUG(serial_print_str(" -> Executing VMPTRLD ... ", COM1));
    if(!vmptrld(vmm.vmcs_region))
    {
        return FALSE;
    }
    DEBUG(serial_print_str("OK\n", COM1));
    
    return TRUE;
}
