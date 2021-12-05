#include "serial_print.h"
#include "inlines.h"

void serial_print_string(BYTE* p_str, PORT port)
{
    if (p_str == NULL)
        return;

    if (*p_str == 0)
        return;

    int i = 0;
    while (p_str[i])
    {
        while ((inb(port + 5) & 0x20) == 0);
        outb(port, p_str[i]);
        i++;
    }
    // carriage return
    while ((inb(port + 5) & 0x20) == 0);
    outb(port, 0xD);

    return;
}

void serial_print_number(QWORD num, PORT port)
{
    for (int i = 15; i>= 0; i--)
    {
        BYTE symbol = (num >> i * 4) & 0xF;

        if (symbol >= 0xA)
            symbol += 0x57;
        else
            symbol += 0x30;

        while ((inb(port + 5) & 0x20) == 0);
        outb(port, symbol);
    }

    while ((inb(port + 5) & 0x20) == 0);
    outb(port, 0xD);
    return;
}
