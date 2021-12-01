LD = x86_64-linux-gnu-gcc

LDFLAGS = -ffreestanding -nostdlib -lgcc -no-pie

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
