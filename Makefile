sicsim.out : main.o shell.o memory.o opcode.o error.o
	gcc -Wall -o sicsim.out main.o shell.o memory.o opcode.o error.o
main.o : main.c main.h
	gcc -Wall -c main.c
shell.o : shell.c shell.h
	gcc -Wall -c shell.c
memory.o : memory.c memory.h
	gcc -Wall -c memory.c
opcode.o : opcode.c opcode.h
	gcc -Wall -c opcode.c
error.o : error.c error.h
	gcc -Wall -c error.c
clean :
	rm sicsim.out main.o shell.o memory.o opcode.o error.o
