cmd_/home/pi/sys_teamproject/device/gas_device/gas_device.ko := ld -r  -EL -T ./scripts/module-common.lds -T ./arch/arm/kernel/module.lds  --build-id  -o /home/pi/sys_teamproject/device/gas_device/gas_device.ko /home/pi/sys_teamproject/device/gas_device/gas_device.o /home/pi/sys_teamproject/device/gas_device/gas_device.mod.o ;  true