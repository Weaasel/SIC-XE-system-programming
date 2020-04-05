#include "memory.h"
#include "opcode.h"

typedef enum {false, true} bool;
typedef struct history_node {
	char name[101];
	struct history_node* link;
}history_node;

bool run(char* arg);
void help();
void dir();
void quit();
void history();

