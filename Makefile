ifneq ($(KERNELRELEASE),)
obj-m := naiwa_n.o
naiwa_n-y := main.o
else
KDIR ?= /lib/modules/$(shell uname -r)/build
MODULE_DIR := $(CURDIR)
PYTHON ?= python3

.PHONY: all modules clean compile_commands check-kdir help

all: modules

check-kdir:
	@test -f "$(KDIR)/Makefile" || { \
		echo "Kernel build files not found in $(KDIR). Install matching kernel headers or set KDIR=/path/to/kernel/build." >&2; \
		exit 1; \
	}

modules: check-kdir
	$(MAKE) -C "$(KDIR)" M="$(MODULE_DIR)" modules

clean: check-kdir
	$(MAKE) -C "$(KDIR)" M="$(MODULE_DIR)" clean
	$(RM) compile_commands.json

compile_commands: modules
	$(PYTHON) "$(KDIR)/scripts/clang-tools/gen_compile_commands.py" -d "$(MODULE_DIR)" -o "$(MODULE_DIR)/compile_commands.json"

help:
	@echo "make                  Build naiwa_n.ko using Kbuild"
	@echo "make KDIR=/path       Build against another prepared kernel tree"
	@echo "make clean            Remove module build files"
	@echo "make compile_commands Generate IDE compilation database after building"
endif
