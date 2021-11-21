CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_BSD_SOURCE  -D_POSIX_C_SOURCE=200112L

LIBRARIES=-lpthread

all: 	mondocane.exe

mondocane.exe: mondocane.o DBGpthread.o
	gcc ${CFLAGS} -o mondocane.exe mondocane.o DBGpthread.o ${LIBRARIES}

mondocane.o: mondocane.c DBGpthread.h printerror.h
	gcc -c ${CFLAGS} mondocane.c

DBGpthread.o: DBGpthread.c printerror.h
	gcc ${CFLAGS} -c DBGpthread.c

.PHONY:	clean

clean:
	-rm -f mondocane.o DBGpthread.o mondocane.exe


