#pragma once

#include "types.h"
#include "serial_print.h"

static inline void outb(PORT port, BYTE byte)
{
    asm volatile ("outb %0, %1"::"a"(byte), "Nd"(port));
}

static inline BYTE inb(PORT port)
{
    BYTE byte;
    asm volatile ("inb %1, %0"
                  : "=a"(byte)
                  : "Nd"(port));
    return byte;
}

static inline QWORD rdmsr(DWORD msr)
{
    DWORD low, high;
    asm volatile (
        "rdmsr"
        : "=a"(low), "=d"(high)
        : "c"(msr)
    );
    return ((QWORD)high << 32) | low;
}

static inline void wrmsr(DWORD msr, QWORD value)
{
    DWORD low = value & 0xFFFFFFFF;
    DWORD high = value >> 32;
    asm volatile (
        "wrmsr"
        :
        : "c"(msr), "a"(low), "d"(high)
    );
}

static inline void wrCR0(QWORD val)
{
    asm volatile ( "movq %0, %%CR0"::"r"(val) );
}

static inline QWORD rdCR0()
{
    QWORD ret;
    asm volatile ( "movq %%CR0, %0":"=r"(ret));
    return ret;
}

static inline void wrCR4(QWORD val)
{
    asm volatile ( "movq %0, %%CR4"::"r"(val) );
}


static inline QWORD rdCR4()
{
    QWORD ret;
    asm volatile ( "movq %%CR4, %0":"=r"(ret));
    return ret;
}

static inline bool vmxon(void* addr)
{
    QWORD flags;
    asm volatile( "vmxon %1\r\n" 
                  "pushf\n\t"
                  "pop %0"
                  : "=g"(flags)
                  : "m"(addr));
    
    return flags & ((1UL << 1) |         //CF
                    (1UL << 6));         //ZF
}

static inline bool vmclear(void* addr)
{
    QWORD flags;
    asm volatile( "vmclear %1\r\n" 
                  "pushf\n\t"
                  "pop %0"
                  : "=g"(flags)
                  : "m"(addr));
    
    return flags & ((1UL << 1) |         //CF
                    (1UL << 6));         //ZF
}

static inline bool vmptrld(void* addr)
{
    QWORD flags;
    asm volatile( "vmptrld %1\r\n" 
                  "pushf\n\t"
                  "pop %0"
                  : "=g"(flags)
                  : "m"(addr));
    
    return flags & ((1UL << 1) |         //CF
                    (1UL << 6));         //ZF
}
