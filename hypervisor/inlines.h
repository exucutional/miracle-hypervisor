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
