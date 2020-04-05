#include "error.h"

//Error occurred by invalid commands
void undefined_command() {
	printf("Error : Undefined Command!\n");
	return;
}

//Error occurred by Absence or invalid arguments. ex) 0xHZ
void undefined_argument() {
	printf("Error : Undefined argument!\n");
	return;
}

//Error occurred by out of range on memory address or value
void out_of_range() {
	printf("Error : Value Out of Range!\n");
	return;
}
