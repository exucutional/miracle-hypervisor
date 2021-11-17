ENTRY(BootEntry)
KERNEL_LMA = 0x00007c00;
KERNEL_VMA = 0xC0000000;
SECTIONS
{
    . = KERNEL_LMA;

    .boot.text :
    {
        boot/obj/boot.o (.boot.text)
        *(.boot.text)
        . = ALIGN(4096);
    }

    . += KERNEL_VMA;

    .text : AT(ADDR(.text) - KERNEL_VMA)
    {
        _code = .;
        *(EXCLUDE_FILE(*boot/obj/boot.o) .text)
        *(.rodata*)
        . = ALIGN(4096);
    }

   .data : AT(ADDR(.data) - KERNEL_VMA)
   {
        _data = .;
        *(.data)
        . = ALIGN(4096);
   }

   .eh_frame : AT(ADDR(.eh_frame) - KERNEL_VMA)
   {
       _ehframe = .;
       *(.eh_frame)
        . = ALIGN(4096);
   }

   .bss : AT(ADDR(.bss) - KERNEL_VMA)
   {
       _bss = .;
       *(.bss)
        *(COMMON)
       . = ALIGN(4096);
   }

   .reloc : AT(ADDR(.eh_frame) - KERNEL_VMA)
   {
       _reloc = .;
       *(.reloc)
       . = ALIGN(4096);
   }

   .xdata : AT(ADDR(.xdata) - KERNEL_VMA)
   {
       _xdata = .;
       *(.xdata)
       . = ALIGN(4096);
   }

   .pdata : AT(ADDR(.pdata) - KERNEL_VMA)
   {
       _pdata = .;
       *(.pdata)
       . = ALIGN(4096);
   }

   _end = .;

   /DISCARD/ :
   {
        *(.comment)
   }
}