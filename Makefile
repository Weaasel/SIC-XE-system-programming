sicsim.out : main.o shell.o memory.o opcode.o error.o
	gcc -W -o sicsim.out main.o shell.o memory.o opcode.o error.o
main.o : main.c main.h
	gcc -W -c main.c
shell.o : shell.c shell.h
	gcc -W -c shell.c
memory.o : memory.c memory.h
	gcc -W -c memory.c
opcode.o : opcode.c opcode.h
	gcc -W -c opcode.c
error.o : error.c error.h
	gcc -W -c error.c
clean :
	rm sicsim.out main.o shell.o memory.o opcode.o error.o
