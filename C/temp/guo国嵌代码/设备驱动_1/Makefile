ifneq ($(KERNELRELEASE),)

obj-m := memdev.o

else
	
KDIR := /home/David/mini2440/Kernel/linux-2.6.29
all:
	make -C $(KDIR) M=$(PWD) modules ARCH=arm CROSS_COMPILE=arm-linux-
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers

endif
