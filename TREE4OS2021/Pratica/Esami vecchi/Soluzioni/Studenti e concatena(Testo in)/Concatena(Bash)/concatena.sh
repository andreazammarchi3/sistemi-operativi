#!/bin/bash

echo "" > output.txt
for name in `find /usr/include/ -maxdepth 1 -name "std*" ` ; do
	NUM=`head -n 4 ${name} | wc -l`
	# tengo solo il numero, escludo il nome del file
	NUM=${NUM%% *} 
	if (( ${NUM} > 1 )) ; then
		((NUM=${NUM}-1))
		head -n 4 ${name} | tail -n ${NUM} >> output.txt
	fi
done

