# 使用内核编译系统
obj-m += DKernel.o

# 当前模块源码目录
PWD := $(shell pwd)

# 内核源码路径（GitHub Action 中会设置为仓库根目录，这里可用相对路径）
KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
