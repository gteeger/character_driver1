cmd_/root/linux/demo3_backup.ko := ld -EL -r  -T ./scripts/module-common.lds --build-id  -T ./arch/arm/kernel/module.lds -o /root/linux/demo3_backup.ko /root/linux/demo3_backup.o /root/linux/demo3_backup.mod.o ;  true
