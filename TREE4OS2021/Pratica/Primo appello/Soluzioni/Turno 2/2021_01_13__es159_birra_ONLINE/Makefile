CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_BSD_SOURCE  -D_POSIX_C_SOURCE=200112L

LIBRARIES=-lpthread

all: 	birra.exe

birra.exe: birra.o DBGpthread.o
	gcc ${CFLAGS} -o birra.exe birra.o DBGpthread.o ${LIBRARIES}

birra.o: birra.c DBGpthread.h printerror.h
	gcc -c ${CFLAGS} birra.c

DBGpthread.o: DBGpthread.c printerror.h
	gcc ${CFLAGS} -c DBGpthread.c

.PHONY:	clean

clean:
	-rm -f birra.o DBGpthread.o birra.exe


