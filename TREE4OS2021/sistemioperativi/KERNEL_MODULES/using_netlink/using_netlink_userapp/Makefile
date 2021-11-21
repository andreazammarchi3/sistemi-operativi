
#VIC: compilo come utente normale, non root, perche' lavoro in directory utente.


all:		using_netlinl_userapp.exe


using_netlinl_userapp.exe:	using_netlinl_userapp.c
		gcc -o using_netlinl_userapp.exe using_netlinl_userapp.c 

.PHONY:		clean


clean:		
		rm -f using_netlinl_userapp.exe using_netlinl_userapp.o

