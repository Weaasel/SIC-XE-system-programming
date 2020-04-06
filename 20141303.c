#include "20141303.h"

int main() {
	//initialize optable before start
	init_optable();
	//initialize memory to zero before start
	reset();
	while(1) {
		memset(arg, 0, ARG_LEN);
		printf("sicsim> ");
		fgets(arg, ARG_LEN, stdin);
		arg[(int)strlen(arg) - 1] = 0; //erase '\n'
		if(!run(arg)) break;
		printf("\n");
	}
	//clear history node and opcode table before exit
	clear_history();
	clear_optable();
	return 0;
}
