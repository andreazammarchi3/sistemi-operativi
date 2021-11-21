#!/bin/bash

echo "ho passato $# argomenti alla shell" 
echo "l'identificatore del processo corrente e' $$" 
echo "il nome del processo in esecuzione e' $0" 
echo "gli argomenti passati a riga di comando sono $*" 

NUM=0
for name in  $* ;  do 
	echo "argomento ${name}" ; 
	if [ ${name} != "picchio" ] 
	then
		# controllo se il file di nome ${name} esiste gia' ed e' modificabile
		if [ -w ${name} ] 
		then
			# se il file esiste lo elimino
			rm ${name}
		else
			# se invece il file non eeiste allora lo creo 
			touch ${name}
			echo ciclismo > ${name}	
			echo "creato il file ${name}"
			((NUM=${NUM}+1))
		fi
	fi
done
echo "creati $NUM file"

# come leggere input da tastiera

echo -e "scrivi una frase \c"
read param
echo "la frase scritta e': $param"

# eseguiamo un programma e vediamo il suo risultato

./primo.exe

echo "il processo chiamato ha restituito come valore di uscita $? "
echo "il processo chiamato ha restituito come valore di uscita $? "

# domanda: perché la seconda volta echo dà un output diverso?



# eseguiamo ancora il programma e vediamo il suo output

OUT=`./primo.exe` 
echo "l'output del processo e' ${OUT}"

exit 9
