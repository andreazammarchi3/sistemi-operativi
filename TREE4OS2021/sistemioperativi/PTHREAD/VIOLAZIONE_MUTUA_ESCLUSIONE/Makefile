# Makefile
# -D_REENTRANT per funzioni thread safe
# -D_BSD_SOURCE per random
# -D_POSIX_C_SOURCE=199309L per nanosleep

CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_BSD_SOURCE -D_POSIX_C_SOURCE=199309L 
LFLAGS=-lpthread

all: verifica_assegnamenti_contemporanei_long_double.exe


verifica_assegnamenti_contemporanei_long_double.exe: verifica_assegnamenti_contemporanei_long_double.o
	gcc ${CFLAGS} -o verifica_assegnamenti_contemporanei_long_double.exe verifica_assegnamenti_contemporanei_long_double.o ${LFLAGS}

verifica_assegnamenti_contemporanei_long_double.o: verifica_assegnamenti_contemporanei_long_double.c
	gcc -c ${CFLAGS} verifica_assegnamenti_contemporanei_long_double.c

.PHONY:	clean

clean:
	rm -f verifica_assegnamenti_contemporanei_long_double.exe verifica_assegnamenti_contemporanei_long_double.o 
