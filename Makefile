sicsim.out : main.o shell.o memory.o opcode.o
	gcc -W -o sicsim.out main.o shell.o memory.o opcode.o
main.o : main.c main.h
	gcc -W -c main.c
shell.o : shell.c shell.h
	gcc -W -c shell.c
memory.o : memory.c memory.h
	gcc -W -c memory.c
opcode.o : opcode.c opcode.h
	gcc -W -c opcode.c
clean :
	rm sicsim.out main.o shell.o memory.o opcode.o
