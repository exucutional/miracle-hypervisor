CC         = x86_64-linux-gnu-gcc
LD         = x86_64-linux-gnu-ld
SP         = x86_64-linux-gnu-strip
RE         = x86_64-linux-gnu-readelf
CFLAGS     = -g -ffreestanding -nostdlib -nostdinc -fno-common -mcmodel=small -mno-red-zone -mno-mmx -mno-sse -mno-sse -Wall -Wextra -fno-stack-protector -c -I ../bootboot/dist/
LDFLAGS    = -nostdlib -nostartfiles -T link.ld
STRIPFLAGS = -s -K mmio -K fb -K bootboot -K environment -K initstack
OBJPATH    = ./obj
SRCC       = kernel.c serial_print.c
OBJC       = $(SRCC:%.c=$(OBJPATH)/%.o)
OBJ        = $(OBJC) $(OBJPATH)/font.o
HEADERS    = serial_print.h types.h
TARGET     = hypervisor.elf
DUMP       = hypervisor.txt

all: $(OBJPATH) $(OBJC) $(OBJPATH)/font.o $(TARGET)

$(OBJPATH):
	@mkdir $@

$(OBJPATH)/%.o: %.c $(HEADERS)
	@echo "COMPILING:"
	$(CC) $(CFLAGS) $< -o $@

$(OBJPATH)/font.o: font.psf
	$(LD) -r -b binary -o obj/font.o font.psf

$(TARGET): $(OBJ) $(HEADERS)
	@echo "LINKING:"
	$(LD) $(LDFLAGS) $(OBJ) -o $@
	@echo "STRIPPING"
	$(SP) $(STRIPFLAGS) $@
	@echo "READING ELF -> $(DUMP)"
	$(RE) -hls $(TARGET) > $(DUMP)

.PHONY: clean

clean:
	rm -rf $(OBJPATH)
	rm $(TARGET) $(DUMP)
