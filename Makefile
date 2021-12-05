SUBDIRS    = bootboot/mkbootimg hypervisor
BOOTBOOT   = bootboot/mkbootimg/mkbootimg
BOOTCONFIG = hypervisor/bootboot.json
TARGET     = boot.bin
QEMUFLAGS  = -no-reboot -no-shutdown -drive format=raw

all: $(SUBDIRS) $(TARGET)

qemu:
	qemu-system-x86_64 $(QEMUFLAGS),file=$(TARGET)

bochs:
	bochs

$(TARGET): $(SUBDIRS)
	$(BOOTBOOT) $(BOOTCONFIG) $(TARGET)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS) 
