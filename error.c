#include "error.h"

void undefined_command() {
	printf("Error : Undefined Command!\n");
	return;
}

void undefined_argument() {
	printf("Error : Undefined argument!\n");
	return;
}

void out_of_range() {
	printf("Error : Value Out of Range!\n");
	return;
}
