# Makefile – makefile of our first driver

# if KERNELRELEASE is not defined, we've been called directly from the command line.
# Invoke the kernel build system.
ifeq (${KERNELRELEASE},)
	KERNEL_SOURCE := /lib/modules/$(shell uname -r)/build/
	PWD := $(shell pwd)
default:
	${MAKE} -C ${KERNEL_SOURCE} M=${PWD} modules

install:
	${MAKE} -C ${KERNEL_SOURCE} M=${PWD} modules_install
	depmod

clean:
	${MAKE} -C ${KERNEL_SOURCE} M=${PWD} clean

# Otherwise KERNELRELEASE is defined; we've been invoked from the
# kernel build system and can use its language.
else
	obj-m := itron_mice.o
endif
