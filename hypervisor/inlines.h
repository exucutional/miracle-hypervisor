#pragma once

#include "types.h"
#include "serial_print.h"

static inline void outb(PORT port, BYTE byte)
{
    asm volatile ("outb %0, %1" : : "a"(byte), "Nd"(port));
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
