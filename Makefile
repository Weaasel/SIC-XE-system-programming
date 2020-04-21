20141303.out : 20141303.o shell.o assembler.o memory.o opcode.o base.o error.o
	gcc -Wall -o 20141303.out 20141303.o shell.o assembler.o memory.o opcode.o base.o error.o
20141303.o : 20141303.c 20141303.h
	gcc -Wall -c 20141303.c
shell.o : shell.c shell.h
	gcc -Wall -c shell.c
assembler.o : assembler.c assembler.h
	gcc -Wall -c assembler.c
memory.o : memory.c memory.h
	gcc -Wall -c memory.c
opcode.o : opcode.c opcode.h
	gcc -Wall -c opcode.c
base.o : base.c base.h
	gcc -Wall -c base.c
error.o : error.c error.h
	gcc -Wall -c error.c
clean :
	rm 20141303.out 20141303.o shell.o assembler.o memory.o opcode.o base.o error.o
