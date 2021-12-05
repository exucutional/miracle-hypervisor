ASM = nasm

ASMFLAGS = -f elf64 -g -F dwarf

OBJPATH = ./obj

SRCASM = boot.asm

OBJASM = $(SRCASM:%.asm=$(OBJPATH)/%.o)

all: $(OBJPATH) $(OBJPATH)/boot.o

$(OBJPATH):
	@mkdir $@

$(OBJPATH)/%.o: %.asm
	@echo "COMPILING:"
	$(ASM) $(ASMFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -rf $(OBJPATH)
