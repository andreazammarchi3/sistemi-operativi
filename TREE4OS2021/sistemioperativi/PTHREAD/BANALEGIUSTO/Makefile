# MakefileBanali

CFLAGS=-ansi -Wpedantic -Wall -D_THREAD_SAFE -D_REENTRANT -D_POSIX_C_SOURCE=200112L
LFLAGS=-lpthread

all: banale_giusto.exe

banale_giusto.exe: banale_giusto.o
	gcc ${CFLAGS} -o banale_giusto.exe banale_giusto.o ${LFLAGS}

banale_giusto.o: banale_giusto.c
	gcc -c ${CFLAGS} banale_giusto.c

.PHONY:	clean

clean:
	rm -f banale_giusto.exe banale_giusto.o

