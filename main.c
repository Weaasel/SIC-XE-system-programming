#include "main.h"

bool run(char* arg);

int main() {
	char arg[ARG_LEN];	

	while(1) {
		printf("sicsim> ");
		fgets(&arg, ARG_LEN, stdin);
		if(!run(arg)) break;
		printf("\n");
	}

	return 0;
}



bool run(char* arg) {
	return true;
}
