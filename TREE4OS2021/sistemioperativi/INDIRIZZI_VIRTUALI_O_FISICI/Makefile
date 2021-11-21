CFLAGS=-ansi -Wall -Wpedantic

all:	indirizzi.exe


indirizzi.exe:	indirizzi.o
		gcc -o indirizzi.exe ${CFLAGS} indirizzi.o 

indirizzi.o:		indirizzi.s
		gcc -c ${CFLAGS} indirizzi.s

indirizzi.s:		indirizzi.c
		gcc -S ${CFLAGS} indirizzi.c


.PHONY:		clean


clean:	
		rm indirizzi.exe indirizzi.o indirizzi.s

