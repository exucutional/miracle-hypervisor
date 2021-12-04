#pragma once

#include "types.h"

#define PORT unsigned int
#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

void serial_print_string(BYTE* str_p, PORT port);
void serial_print_number(QWORD num, PORT port);
