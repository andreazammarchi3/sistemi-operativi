CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L

LIBRARIES=-lpthread

all: 	circolare1a1.exe SBAGLIATO_0_circolare1a1.exe SBAGLIATO_1_circolare1a1.exe SBAGLIATO_2_circolare1a1.exe

circolare1a1.exe: circolare1a1.o DBGpthread.o
	gcc ${CFLAGS} -o circolare1a1.exe circolare1a1.o DBGpthread.o ${LIBRARIES}

circolare1a1.o: circolare1a1.c DBGpthread.h printerror.h
	gcc ${CFLAGS} -c circolare1a1.c 

DBGpthread.o: DBGpthread.c printerror.h
	gcc ${CFLAGS} -c DBGpthread.c 

SBAGLIATO_0_circolare1a1.exe: SBAGLIATO_0_circolare1a1.o DBGpthread.o
	gcc ${CFLAGS} -o SBAGLIATO_0_circolare1a1.exe SBAGLIATO_0_circolare1a1.o DBGpthread.o ${LIBRARIES}

SBAGLIATO_0_circolare1a1.o: SBAGLIATO_0_circolare1a1.c DBGpthread.h printerror.h
	gcc ${CFLAGS} -c SBAGLIATO_0_circolare1a1.c 

SBAGLIATO_1_circolare1a1.exe: SBAGLIATO_1_circolare1a1.o DBGpthread.o
	gcc ${CFLAGS} -o SBAGLIATO_1_circolare1a1.exe SBAGLIATO_1_circolare1a1.o DBGpthread.o ${LIBRARIES}

SBAGLIATO_1_circolare1a1.o: SBAGLIATO_1_circolare1a1.c DBGpthread.h printerror.h
	gcc ${CFLAGS} -c SBAGLIATO_1_circolare1a1.c 

SBAGLIATO_2_circolare1a1.exe: SBAGLIATO_2_circolare1a1.o DBGpthread.o
	gcc ${CFLAGS} -o SBAGLIATO_2_circolare1a1.exe SBAGLIATO_2_circolare1a1.o DBGpthread.o ${LIBRARIES}

SBAGLIATO_2_circolare1a1.o: SBAGLIATO_2_circolare1a1.c DBGpthread.h printerror.h
	gcc ${CFLAGS} -c SBAGLIATO_2_circolare1a1.c 

.PHONY:	clean

clean:
	-rm -f circolare1a1.o DBGpthread.o circolare1a1.exe
	-rm -f SBAGLIATO_0_circolare1a1.o DBGpthread.o SBAGLIATO_0_circolare1a1.exe
	-rm -f SBAGLIATO_1_circolare1a1.o DBGpthread.o SBAGLIATO_1_circolare1a1.exe
	-rm -f SBAGLIATO_2_circolare1a1.o DBGpthread.o SBAGLIATO_2_circolare1a1.exe


