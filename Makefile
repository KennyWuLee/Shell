all: shell.o list.o alias.o list.h alias.h
	gcc -Wall -o shell shell.o list.o alias.o

shell.o: shell.c
	gcc -Wall -c shell.c

list.o: list.c
	gcc -Wall -c list.c

alias.o: alias.c
	gcc -Wall -c alias.c
