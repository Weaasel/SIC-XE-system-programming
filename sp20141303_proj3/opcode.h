#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "base.h"

//node structure for opcode hash table
typedef struct hash_node {
	char code[10];
	char name[10];
	char data[10];
	struct hash_node* next;
}hash_node;

//opcode functions
void add_hash_node(struct hash_node* new_);
void init_optable();
int opname_to_hex(char* str);
int get_format(char* str);
int opcode(char* str);
void opcodelist();
void clear_optable();
