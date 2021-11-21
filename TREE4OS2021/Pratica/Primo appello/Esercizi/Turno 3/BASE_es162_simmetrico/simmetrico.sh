#!/bin/bash

if (( $# == 0 )) ; then # nessun argomento, cioe' la stringa vuota e' simmetrica
	echo vero
	exit 0;
fi
STRINGA=$1
if (( ${#STRINGA} == 1 )) ; then # la stringa di un solo carattere e' simmetrica
	echo vero
	exit 0;
else # stringa con 2 o piu' caratteri
	CARATTEREINIZIALE=${STRINGA:0:1}
	LUNGHEZZASTRINGA=${#STRINGA}
	CARATTEREFINALE=${STRINGA:${LUNGHEZZASTRINGA}-1:1}
	if [[ ${CARATTEREINIZIALE} != ${CARATTEREFINALE} ]] ; then
		echo falso
		exit 1
	else
		# chiamata ricorsiva
		# COMPLETARE A PARTIRE DA QUI SOTTO
		

		# prendo la parte di mezzo della stringa
		# e richiamo lo script sulla parte  di mezzo


		# COMPLETARE FINO A QUI
	fi
fi


// stringaricor=${STRINGA:1:${LUNGHEZZASTRINGA}-2}
// ./simmetrico.sh $stringaricor