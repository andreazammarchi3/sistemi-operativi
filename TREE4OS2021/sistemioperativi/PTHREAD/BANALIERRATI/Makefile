# MakefileBanali

CFLAGS=-ansi -Wpedantic -Wall -D_THREAD_SAFE -D_REENTRANT -D_POSIX_C_SOURCE=200112L
LFLAGS=-lpthread

all: banale_moltoerrato.exe banale_errato.exe

banale_moltoerrato.exe: banale_moltoerrato.o
	gcc ${CFLAGS} -o banale_moltoerrato.exe banale_moltoerrato.o ${LFLAGS}

banale_moltoerrato.o: banale_moltoerrato.c
	gcc -c ${CFLAGS} banale_moltoerrato.c

banale_errato.exe: banale_errato.o
	gcc ${CFLAGS} -o banale_errato.exe banale_errato.o ${LFLAGS}

banale_errato.o: banale_errato.c
	gcc -c ${CFLAGS} banale_errato.c

.PHONY:	clean

clean:
	rm -f banale_moltoerrato.exe banale_errato.exe banale_moltoerrato.o banale_errato.o

