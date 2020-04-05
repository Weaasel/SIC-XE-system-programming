#include "main.h"

char arg[ARG_LEN];	

int main() {

	while(1) {
		printf("sicsim> ");
		fgets(&arg, ARG_LEN, stdin);
		if(!run(arg)) break;
		printf("\n");
	}

	return 0;
}
