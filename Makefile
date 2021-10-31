ASM = nasm
CC = gcc
LD = gcc

CFLAGS = 
ASMFLAGS = -f bin
LDFLAGS =

OBJPATH = ./obj

SRCC =
SRCASM = boot.asm

OBJC = $(SRCC:%.c=$(OBJPATH)/%.o)
OBJASM = $(SRCASM:%.asm=$(OBJPATH)/%.o)
HEADERS = 

TARGET = boot.bin

all: boot/boot.bin

run: all
	qemu-system-i386 -hda boot/boot.bin

$(OBJPATH):
	@mkdir $@

$(TARGET): $(OBJC) $(OBJASM)
	@echo "LINKING:"
	$(LD) $(LDFLAGS) $(OBJC) $(OBJASM) -o  $@

$(OBJPATH)/%.o: %.c $(HEADERS)
	@echo "COMPILING:"
	$(CC) $(CFLAGS) $< -o $@

$(OBJPATH)/%.o: %.asm
	@echo "COMPILING:"
	$(ASM) $(AFLAGS) $< -o $@

%.bin: %.asm
	@echo "COMPILING:"
	$(ASM) $(AFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -rf $(OBJPATH)
