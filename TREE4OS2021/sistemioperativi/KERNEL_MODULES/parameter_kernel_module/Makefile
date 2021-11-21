# Now have a look at linux/drivers/char/Makefile for a real world example. As you can see, some things get hardwired into the kernel (obj-y) but where are all those obj-m gone? Those familiar with shell scripts will easily be able to spot them. For those not, the obj-$(CONFIG_FOO) entries you see everywhere expand into obj-y or obj-m, depending on whether the CONFIG_FOO variable has been set to y or m. While we are at it, those were exactly the kind of variables that you have set in the linux/.config file, the last time when you said make menuconfig or something like that

obj-m = parameter_module.o

KVERSION = $(shell uname -r)

#VIC: compilo come utente normale, non root, tanto lavoro in directory utente.

all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules

# VIC: invece occorre eseguire l'installazione dei moduli (non la compilazione)
#      come root perche' la directory in cui installare i moduli e' di proprieta' di root;

install:
        sudo make -C /lib/modules/$(KVERSION)/build M=$(MODULESRCDIR) modules_install


clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean


