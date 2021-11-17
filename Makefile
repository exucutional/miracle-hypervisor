LD = x86_64-w64-mingw32-gcc

LDFLAGS = -ffreestanding -nostdlib -lgcc

LINKSCRIPT = link.ld

SUBDIRS = boot hypervisor

TARGET = boot.bin

all: $(SUBDIRS) $(TARGET)

run: all
	qemu-system-x86_64 -hda $(TARGET)

$(SUBDIRS):
	$(MAKE) -C $@

OBJ = $(wildcard $(addsuffix /obj/*.o,$(SUBDIRS)))

$(TARGET): $(OBJ)
	@echo "LINKING:"
	$(LD) $(LDFLAGS) -T $(LINKSCRIPT) $(OBJ) -o $@

.PHONY: $(SUBDIRS) clean

clean:
	rm -rf $(OBJPATH)
