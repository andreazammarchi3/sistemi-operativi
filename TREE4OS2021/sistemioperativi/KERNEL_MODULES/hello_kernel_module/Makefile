#VIC: questo Makefile deve stare nella directory dell'utente
#     che contiene i sorgenti del modulo da compilare.
#     L'utente non e' root.
#     L'utente esegue il comando make proprio stando nella directory
#     che contiene i sorgenti del modulo da compilare.

# Now have a look at linux/drivers/char/Makefile for a real world example. As you can see, some things get hardwired into the kernel (obj-y) but where are all those obj-m gone? Those familiar with shell scripts will easily be able to spot them. For those not, the obj-$(CONFIG_FOO) entries you see everywhere expand into obj-y or obj-m, depending on whether the CONFIG_FOO variable has been set to y or m. While we are at it, those were exactly the kind of variables that you have set in the linux/.config file, the last time when you said make menuconfig or something like that

obj-m = hello_module.o

KVERSION = $(shell uname -r)
MODULESRCDIR = $(shell pwd)

#VIC: notare che per compilare il nostro modulo esterno 
#     si usa il makefile del kernel corrente
#     quello che si trova nella directory /lib/modules/4.10.1/build
#     che e' un link simbolico alla directory dei sorgenti del kernel
#     /lib/modules/4.10.1/build --> /usr/src/linux-4.10.1/
#     Infatti, l'opzione -C ordina di eseguire il comando make -C etcetc
#     stando nella directory specificata dall'argomento successivo al -C

#VIC: notare che l'opzione M=$(MODULESRCDIR) indica al Makefile del kernel
#     in quale directory si trovano i moduli da compilare.
#     Infatti, poiche' l'opzione -C fa eseguire il make in una directory diversa
#     bisogna dire al make dove si trovano i sorgenti del modulo da compilare.
#     Qui si ipotizza che il comando make venga inizialmente lanciato
#     dall'utente stando nella directory che contiene i sorgenti del modulo.
#     Con questa ipotesi, l'assegnamento MODULESRCDIR = $(shell pwd)
#     mette nella variabile del make MODULESRCDIR la directory corrente,
#     che e' quella in cui si trovano i sorgenti del modulo.

# VIC: NOTA BENE: ricordarsi di lanciare il make esterno come semplice utente
#      poiche' se si esegue come root o con sudo allora
#      la directory ottenuta in MODULESRCDIR sara' vuota e
#      cio' causera' la compilazione ed installazione
#      di tutti i moduli del kernel
#      invece del solo modulo specificato nella variabile obj-m

#VIC: compilo come utente normale, non root, tanto lavoro in directory utente.

all:
	make -C /lib/modules/$(KVERSION)/build M=$(MODULESRCDIR) modules

# VIC: invece occorre eseguire l'installazione dei moduli (non la compilazione)
#      come root perche' la directory in cui installare i moduli e' di proprieta' di root;

install:
	sudo make -C /lib/modules/$(KVERSION)/build M=$(MODULESRCDIR) modules_install


clean:
	make -C /lib/modules/$(KVERSION)/build M=$(MODULESRCDIR) clean


