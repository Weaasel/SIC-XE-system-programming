#include "main.h"

char arg[ARG_LEN];	

int main() {
	init_optable();
	while(1) {
		memset(arg, 0, ARG_LEN);
		printf("sicsim> ");
		fgets(arg, ARG_LEN, stdin);
		arg[(int)strlen(arg) - 1] = 0;
			
		if(!run(arg)) break;
		printf("\n");
	}
	clear_history();
	clear_optable();
	return 0;
}
