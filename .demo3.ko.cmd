cmd_/root/linux/demo3.ko := ld -EL -r  -T ./scripts/module-common.lds --build-id  -T ./arch/arm/kernel/module.lds -o /root/linux/demo3.ko /root/linux/demo3.o /root/linux/demo3.mod.o ;  true
