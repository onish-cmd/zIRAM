# Name of module
obj-m += main.o

# Path to the kernel build directory
KDIR := /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

# Tells kbuild to use Clang and LLVM tools
LLVM_FLAGS := LLVM=1 CC=clang

all:
	$(MAKE) -C $(KDIR) M=$(PWD) $(LLVM_FLAGS) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

# Help command to check if clang is being detected correctly
check:
	clang --version
	ls $(KDIR)
