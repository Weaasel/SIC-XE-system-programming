#include <stdio.h>
#include <string.h>

typedef struct history_node {
	char name[101];
	history_node* link;
}history_node;

void help();
void dir();
void quit();
void history();

