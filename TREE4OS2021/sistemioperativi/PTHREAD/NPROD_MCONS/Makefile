CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L

LIBRARIES=-lpthread

FINITEQUEUE_SOURCE_DIR=../../COMMON/FINITE_QUEUE/


all: 	NProdMConsKbuffer.exe

NProdMConsKbuffer.exe: NProdMConsKbuffer.o DBGpthread.o finiteTailQueue.o
	gcc ${CFLAGS} -o NProdMConsKbuffer.exe NProdMConsKbuffer.o DBGpthread.o finiteTailQueue.o ${LIBRARIES}

NProdMConsKbuffer.o: NProdMConsKbuffer.c DBGpthread.h printerror.h
	gcc -c ${CFLAGS} -I${FINITEQUEUE_SOURCE_DIR} NProdMConsKbuffer.c

DBGpthread.o: DBGpthread.c printerror.h
	gcc ${CFLAGS} -c DBGpthread.c

finiteTailQueue.o:	${FINITEQUEUE_SOURCE_DIR}finiteTailQueue.c
	gcc -c ${CFLAGS} -I${FINITEQUEUE_SOURCE_DIR} ${FINITEQUEUE_SOURCE_DIR}finiteTailQueue.c 


.PHONY:	clean

clean:
	-rm -f NProdMConsKbuffer.o finiteTailQueue.o DBGpthread.o NProdMConsKbuffer.exe


