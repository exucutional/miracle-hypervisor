%define FREE_SPACE 0x9000

BITS 16
SECTION .boot.text
global BootEntry
; Entry Point

BootEntry:
    xor ax, ax
    mov ss, ax                          ; Set up stack segment register
    mov sp, BootEntry                   ; Set up stack pointer register

    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    cld

    call CheckLongMode
    jc .NoLongMode

    mov edi, FREE_SPACE                 ; Point edi to a free space bracket
    jmp SwitchToLongMode

BITS 64
.Long:
    hlt
    jmp .Long

BITS 16

.NoLongMode:
    mov si, NoLongMode
    call BiosPrint

.Die:
    hlt
    jmp .Die

%include "longmode.inc"
BITS 16

NoLongMode db "ERROR: CPU does not support long mode.", 0x0A, 0x0D, 0

; Check whether CPU supports long mode or not
; Set carry flag if CPU doens't support long mode
CheckLongMode:
    pushfd                              ; Get EAFLAGS in EAX register
    pop eax

    mov ecx, eax                        ; Enter virtual mode by setting bit 17
    xor eax, 0x200000
    push eax
    popfd

    pushfd                              ; Check whether bit 21 is set or not. If EAX now contains 0, CPUID isn't supported.
    pop eax
    xor eax, ecx
    shr eax, 21
    and eax, 1
    push ecx
    popfd

    test eax, eax
    jz .NoLongMode

    mov eax, 0x80000000
    cpuid

    cmp eax, 0x80000001                 ; Check whether extended function 0x80000001 is available are not.
    jb .NoLongMode                      ; If not, long mode not supported.

    mov eax, 0x80000001
    cpuid

    test edx, 1 << 29                   ; Test if the LongMode-bit, is set or not.
    jz .NoLongMode                      ; If not Long mode not supported.

    ret

.NoLongMode:
    stc
    ret

; BIOS printing
; es:si    Address of ASCIIZ string to print.
BiosPrint:
    pushad
.BiosPrintLoop:
    lodsb                             ; Load the value at [@es:@si] in @al.
    test al, al                       ; If AL is the terminator character, stop printing.
    je .BiosPrintDone                  	
    mov ah, 0x0E	
    int 0x10
    jmp .BiosPrintLoop                    ; Loop till the null character not found.
 
.BiosPrintDone:
    popad                             ; Pop all general purpose registers to save them.
    ret

; Padding
times 510 - ($-$$) db 0
dw 0xAA55
