CFLAGS=-ansi -Wall -Wpedantic 

all:		sizeof_struttura.exe sizeof_vettore.exe sizeof_packed.exe


sizeof_struttura.exe:		sizeof_struttura.o
				gcc ${CFLAGS} -o sizeof_struttura.exe sizeof_struttura.o

sizeof_struttura.o:		sizeof_struttura.c
				gcc -c ${CFLAGS} sizeof_struttura.c

sizeof_vettore.exe:		sizeof_vettore.o
				gcc ${CFLAGS} -o sizeof_vettore.exe sizeof_vettore.o

sizeof_vettore.o:		sizeof_vettore.c
				gcc -c ${CFLAGS} sizeof_vettore.c

sizeof_packed.exe:		sizeof_packed.o
				gcc ${CFLAGS} -o sizeof_packed.exe sizeof_packed.o

sizeof_packed.o:		sizeof_packed.c
				gcc -c ${CFLAGS} sizeof_packed.c

.PHONY:			clean

clean:		
			rm -f sizeof_packed.o sizeof_packed.exe sizeof_vettore.o sizeof_vettore.exe sizeof_struttura.o sizeof_struttura.exe

