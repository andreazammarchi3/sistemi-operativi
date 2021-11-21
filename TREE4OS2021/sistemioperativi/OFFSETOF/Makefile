CFLAGS=-ansi -Wall -Wpedantic 

all:		offsetof_struttura.exe offsetof_packed.exe


offsetof_struttura.exe:		offsetof_struttura.o
				gcc ${CFLAGS} -o offsetof_struttura.exe offsetof_struttura.o

offsetof_struttura.o:		offsetof_struttura.c
				gcc -c ${CFLAGS} offsetof_struttura.c

offsetof_packed.exe:		offsetof_packed.o
				gcc ${CFLAGS} -o offsetof_packed.exe offsetof_packed.o

offsetof_packed.o:		offsetof_packed.c
				gcc -c ${CFLAGS} offsetof_packed.c

.PHONY:			clean

clean:		
			rm -f offsetof_packed.o offsetof_packed.exe offsetof_struttura.o offsetof_struttura.exe

