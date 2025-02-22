ARCH=arm
CROSS_COMPILE=/root/luckfox/luckfox-pico/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin/arm-rockchip830-linux-uclibcgnueabihf-
export ARCH CROSS_COMPILE

KERN_DIR = /root/luckfox/luckfox-pico/sysdrv/source/kernel
KBUILD_OUTPUT := $(abspath $(dir $(lastword $(KERN_DIR))))/objs_kernel
name = dht11
PWD ?= $(shell pwd)
BUILD_DIR = $(PWD)/build

all:
	make O=$(KBUILD_OUTPUT) -C $(KERN_DIR) M=$(PWD) modules
	$(CROSS_COMPILE)gcc $(name)_test.c -o $(name)
	echo $(PWD)
	
clean:
	rm -f *.ko *.o *.mod *.mod.o *.mod.c *.symvers *.order *.cmd
  # make -C $(KERN_DIR) M=$(PWD) modules clean
  # rm -rf modules.order
  # rm -f $(name)_test
obj-m += $(name)_drv.o