KERNELDIR = /lib/modules/4.19.42-v7+/build

obj-m := led_device.o

PWD := $(shell pwd)

add :
	make -C $(KERNELDIR) M=$(PWD) modules

clean :
	make -C $(KERNELDIR) M=$(PWD) clean

