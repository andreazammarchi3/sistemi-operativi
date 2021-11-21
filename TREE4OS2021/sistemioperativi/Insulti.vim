" Vim syntax file
" Language:	Insulti
" Maintainer:	Vittorio Ghini
" Last Change:	2016 Apr 12  

"Quit when a (custom) syntax file was already loaded
" if exists("b:current_syntax")
"   finish
" endif

let s:cpo_save = &cpo			" salva l'attuale impostazione di compatbilita' di vim
set cpo&vim				" resetta l'impostazione di vim portandola al default per vim

" A bunch of useful keywords
syn keyword	Insulti1 Cretino Fesso Idiota
syn keyword     Insulti2 Rincoglionito Coglione Testadicazzo 

syn match Insulti3 '[pP][oO][rR][cC][oO]'
syn match Insulti3 '[mM][eE][rR][dD][aA]'

"hi def link cStatement		Statement
hi def link Insulti1	Error	
hi def link Insulti2	Statement	
hi def link Insulti3	String

let b:current_syntax = "Insulti"

let &cpo = s:cpo_save
unlet s:cpo_save
" vim: ts=4
