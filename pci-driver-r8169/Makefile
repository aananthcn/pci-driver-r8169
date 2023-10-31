# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.

CONFIG_MODULE_SIG=n

ifneq ($(KERNELRELEASE),)
	obj-m := pci-driver-r8169.o

# Otherwise we were called directly from the command
# line; invoke the kernel build system.
else

	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules


clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

endif