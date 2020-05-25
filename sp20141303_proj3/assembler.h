#include "memory.h"
#include "opcode.h"

//node structure for symbol table
typedef struct symbol_node {
	char name[20];	//symbol name
	char idx[5];
	int LOC;	//symbol location
	struct symbol_node* next;	//pointer for struct linked list
}symbol_node;

//node structure for each assembly code line
typedef struct line_node {
	int idx;	//line number.
	int LOC;	//Location counter
	int PC;		//Program counter
	int format;	//1, 2, 3, 4 format
	int object_len;	//object_code length
	char raw[100];	//raw string of assembly code
	bool is_symbol;	//flag if node has a symbol
	bool is_start;	//flag if node is start node
	bool is_end;	//flag if node is end node
	bool is_base;	//flag if node is base node
	bool is_const;	//flag if node is "BYTE" or "WORD"
	bool is_var;	//flag if node is "RESW" or "RESB"
	bool is_opcode;	//flag if node has an instruction
	char param1[10];	//parameter1
	char param2[10];	//parameter2
	char param3[10];	//parameter3
	char param4[10];	//parameter4
	char object_code[8];	//object code. maximum length = 8
	struct line_node* next;		//pointer pointing next node for struct linked list.
}line_node;

//assembler functions. Will explain at assembler.c
bool in_range(int disp);
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
void make_lst(char* filename);
void make_obj(char* filename);
int assemble(char* filename);
