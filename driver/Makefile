# Makefile for x735_drv

KVER := $(shell uname -r)
KDIR := /lib/modules/$(KVER)/build
PWD := $(shell pwd)
INSTALL_DIR := /lib/modules/$(KVER)/misc
VERBOSE := 0

cmd_prefix := @
ifneq ($(VERBOSE),0)
cmd_prefix :=
endif

all: modules

modules: x735_drv.ko

x735_drv.ko: FORCE
	$(cmd_prefix)$(MAKE) ARCH=arm64 -C $(KDIR) M=$(PWD) KBUILD_VERBOSE=$(VERBOSE) x735_drv.ko

clean:
	$(cmd_prefix)$(MAKE) ARCH=arm64 -C $(KDIR) M=$(PWD) KBUILD_VERBOSE=$(VERBOSE) clean

install:
	$(cmd_prefix)if [ `grep -e '^x735_drv' /proc/modules | wc -l` -ne 0 ]; then \
	modprobe -r x735_drv; \
	fi
	$(cmd_prefix)install -D -v -m 644 x735_drv.ko $(INSTALL_DIR)/x735_drv.ko
	$(cmd_prefix)depmod -a $(KVER)
	$(cmd_prefix)modprobe x735_drv
	$(cmd_prefix)cp ../etc/poweroff.sh /usr/local/bin/
	$(cmd_prefix)echo "x735_drv" >> /etc/modules-load.d/modules.conf 

uninstall:
	$(cmd_prefix)if [ `grep -e '^x735_drv' /proc/modules | wc -l` -ne 0 ]; then \
	modprobe -r x735_drv; \
	fi
	$(cmd_prefix)rm -fv $(INSTALL_DIR)/x735_drv.ko
	$(cmd_prefix)depmod -a $(KVER)
	$(cmd_prefix)rm -fv /usr/local/bin/poweroff.sh
	$(cmd_prefix)sed -i -e "/^x735_drv/d" /etc/modules-load.d/modules.conf 

FORCE:

.PHONY: clean install uninstall FORCE
