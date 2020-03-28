#include <main.h>

int main() {
	char arg[ARG_LEN];	

	while(1) {
		scanf("%s", &arg);
		if(!run(arg)) break;
	}

	return 0;
}
