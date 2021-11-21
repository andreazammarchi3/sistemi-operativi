CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_BSD_SOURCE  -D_POSIX_C_SOURCE=200112L

LIBRARIES=-lpthread

all: 	bisce.exe

bisce.exe: bisce.o DBGpthread.o
	gcc ${CFLAGS} -o bisce.exe bisce.o DBGpthread.o ${LIBRARIES}

bisce.o: bisce.c DBGpthread.h printerror.h
	gcc -c ${CFLAGS} bisce.c

DBGpthread.o: DBGpthread.c printerror.h
	gcc ${CFLAGS} -c DBGpthread.c

.PHONY:	clean

clean:
	-rm -f bisce.o DBGpthread.o bisce.exe


