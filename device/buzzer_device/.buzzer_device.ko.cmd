cmd_/home/pi/sys_teamproject/device/buzzer_device/buzzer_device.ko := ld -r  -EL -T ./scripts/module-common.lds -T ./arch/arm/kernel/module.lds  --build-id  -o /home/pi/sys_teamproject/device/buzzer_device/buzzer_device.ko /home/pi/sys_teamproject/device/buzzer_device/buzzer_device.o /home/pi/sys_teamproject/device/buzzer_device/buzzer_device.mod.o ;  true