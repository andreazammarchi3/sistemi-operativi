#!/bin/bash

sysctl kernel.randomize_va_space

# oppure    
# cat /proc/sys/kernel/randomize_va_space


# visualizza una opzione di configurazione del kernel
# che stabilisce se modificare casualmente AL CARICAMENTO IN MEMORIA
# la collocazione delle sezioni di memoria di un file eseguibile


# L'opzione e' esposta mediante una astrazione di file
# collocato nel filesystem in memoria /proc
# in particolare nella directory /proc/sys

# Questa parte del filesystem proc 
# espone alcune settaggi del kernel
# e permette di modificare ALCUNE di queste configurazioni.

# Le modifiche possono essere effettuate
# mediante il comando  sysctl
# ad esempio
#    sudo sysctl -w kernel.randomize_va_space=2
#
# Una volta le modifiche potevano essere effettuate
# anche mediante degli accessi diretti al file 
# (es. ridirezionamenti:   echo 2 > /proc/sys/kernel/randomize_va_space
# Ora non e' piu' possibile.

# Vedere, come curiosita',
# quale e' il meccanismo attualmente usato dal protocollo TCP
# per il controllo della congestione, 
#     cat /proc/sys/net/ipv4/tcp_congestion_control
# e quali sono gli altri meccanismi disponibili
# che potrebbero sostituire quello correntemente usato.
#     cat /proc/sys/net/ipv4/tcp_available_congestion_control

