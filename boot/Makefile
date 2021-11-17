ASM = nasm

ASMFLAGS = -f win64

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
