#!/bin/bash

if [[ -e PARZIALE.txt ]] ; then
	rm -f PARZIALE.txt
fi

for nomefile in `ls /usr/include/*.h` ; do
	OUT=`cat ${nomefile} | grep ifdef | wc -l `
	LINES=${OUT%% *}
	if (( ${LINES} >= 10 )) ; then
		cat ${nomefile} | grep ifdef | head -n 5 >> PARZIALE.txt
	fi
done
sort PARZIALE.txt > FINALE.txt
exit 0

