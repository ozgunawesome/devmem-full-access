
obj-m := devmem-full-access.o
devmem-full-access-objs := module.o
PWD := $(shell pwd)
KDIR := /lib/modules/$(shell uname -r)/build
WARN := -W -Wall -Wstrict-prototypes -Wmissing-prototypes
INCLUDE := ${KDIR}/include
CCFLAGS := -O3 -DMODULE -D__KERNEL__ ${WARN} ${INCLUDE}
KVERSION := $(shell uname -r)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	strip --strip-unneeded *.ko
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
