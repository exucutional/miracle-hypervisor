SUBDIRS    = bootboot/mkbootimg hypervisor
BOOTBOOT   = bootboot/mkbootimg/mkbootimg
BOOTCONFIG = hypervisor/bootboot.json
TARGET     = boot.bin
QEMUFLAGS  = -serial stdio -enable-kvm -cpu kvm64,+vmx -drive format=raw

all: $(SUBDIRS) $(TARGET)

qemu: all
	qemu-system-x86_64 $(QEMUFLAGS),file=$(TARGET)

bochs: all
	bochs

$(TARGET): $(SUBDIRS)
	$(BOOTBOOT) $(BOOTCONFIG) $(TARGET)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS) 
