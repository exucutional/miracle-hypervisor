#pragma once

#ifndef NDEBUG
#define DEBUG(cmd) cmd;
#else
#define DEBUG(cmd) ;
#endif

#define TRUE                         1
#define FALSE                        0

#define HYPERVISOR_RAM_MAX           0x140000000
#define HYPERVISOR_BASE              0xffffffffffe02000
#define HYPERVISOR_SIZE              0x10000

// entry counts
#define PDE_CNT                      (HYPERVISOR_RAM_MAX / 0x200000)
#define PDPTE_CNT                    (PDE_CNT / 512)
#define PML4E_CNT                    1

// tables counts
#define PD_CNT                       (PDE_CNT / 512)
#define PDPT_CNT                     1
#define PML4_CNT                     1

#define STACK64_SIZE                 0x1000
#define STACK64_ADDR                 (HYPERVISOR_BASE + HYPERVISOR_SIZE + STACK64_SIZE)      

#define STACK64i_SIZE                0x1000
#define STACK64i_ADDR                (STACK64_ADDR + STACK64i_SIZE)              

#define GDT64_SIZE                   0x1000
#define GDT64_ADDR                   STACK64i_ADDR                                

#define IDT64_SIZE                   0x1000
#define IDT64_ADDR                   (GDT64_ADDR + GDT64_SIZE)                    

#define PML4_SIZE                    (PML4_CNT * 512 *8)
#define PML4_ADDR                    (IDT64_ADDR + IDT64_SIZE)

#define PDPT_SIZE                    (PDPT_CNT * 512 * 8)
#define PDPT_ADDR                    (PML4_ADDR + PML4_SIZE)

#define PD_SIZE                      (PD_CNT * 512 * 8)
#define PD_ADDR                      (PDPT_ADDR + PDPT_SIZE)

#define TSS_SIZE                     0x1000
#define TSS_ADDR                     (PD_ADDR + PD_SIZE)

#define VMXON_SIZE                   0x1000
#define VMXON_ADDR                   (TSS_ADDR + TSS_SIZE)

#define VMCS_SIZE                    0x1000
#define VMCS_ADDR                    (VMXON_ADDR + VMXON_SIZE)

#define IA32_APIC_BASE               0x1B
#define IA32_MTRR_PHYSBASE0          0x200
#define IA32_MTRR_PHYSMASK0          0x201
#define IA32_MTRR_DEF_TYPE           0x2FF
#define IA32_PAT                     0x277
#define IA32_EFER                    0xC0000080
#define IA32_FEATURE_CONTROL         0x3A
#define IA32_VMX_BASIC               0x480
#define IA32_VMX_MISC                0x485
#define IA32_VMX_CR0_FIXED0          0x486
#define IA32_VMX_CR0_FIXED1          0x487
#define IA32_VMX_CR4_FIXED0          0x488
#define IA32_VMX_CR4_FIXED1          0x489

#define IA32_VMX_TRUE_PINBASED_CTLS  0x48D
#define IA32_VMX_TRUE_PROCBASED_CTLS 0x48E
#define IA32_VMX_PROCBASED_CTLS2     0x48B
#define IA32_VMX_TRUE_EXIT_CTLS      0x48F
#define IA32_VMX_TRUE_ENTRY_CTLS     0x490

#define CACHE_TP_WB                  6
