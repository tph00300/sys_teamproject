KERNELDIR = /lib/modules/4.14.98-v7+/build

obj-m := buzzer_device.o -lbcm2835

PWD := $(shell pwd)

all :
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean
