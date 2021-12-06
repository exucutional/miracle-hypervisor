SUBDIRS    = bootboot/mkbootimg hypervisor
BOOTBOOT   = bootboot/mkbootimg/mkbootimg
BOOTCONFIG = hypervisor/bootboot.json
TARGET     = boot.img
QEMUFLAGS  = -serial stdio -no-reboot -s -S -no-shutdown -enable-kvm -cpu kvm64,+vmx -D qemu-log.txt -d out_asm,in_asm,int,exec,pcall,cpu_reset,guest_errors,op -drive format=raw

all: $(SUBDIRS) $(TARGET)

qemu: all
	qemu-system-x86_64 $(QEMUFLAGS),file=$(TARGET)

bochs:all
	bochs

$(TARGET): $(SUBDIRS)
	$(BOOTBOOT) $(BOOTCONFIG) $(TARGET)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS) 
