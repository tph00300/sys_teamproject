KERNELDIR = /lib/modules/4.14.98-v7+/build

obj-m := gas_device.o

PWD := $(shell pwd)

all :
	make -C $(KERNELDIR) M=$(PWD) modules

clean :
	make -C $(KERNELDIR) M=$(PWD) clean
