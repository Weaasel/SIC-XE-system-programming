#include "memory.h"
#include "opcode.h"

//node structure for symbol table
typedef struct symbol_node {
	char name[20];
	int LOC;
	struct symbol_node* next;
}symbol_node;

//node structure for each assembly code line
typedef struct line_node {
	int idx;
	int LOC;
	int PC;
	int format;
	char raw[100];
	bool is_symbol;
	bool is_start;
	bool is_end;
	bool is_base;
	bool is_const;
	bool is_var;
	bool is_opcode;
	char param1[10];
	char param2[10];
	char param3[10];
	char param4[10];
	char object_code[8];
	struct line_node* next;
}line_node;

int register_num(char*c );
void clear_symbol(struct symbol_node* root);
void clear_line(struct line_node* root);
void clear_assembler();
void insert_symbol(char* name, int LOC);
int find_symbol(char* name);
int symbol(void);
void insert_line(struct line_node* new_);
int pass1(char* filename);
int pass2(char* filename);

int assemble(char* filename);
void make_lst(char* filename);
void make_obj(char* filename);

