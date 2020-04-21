#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include "assembler.h"
#define ARG_LEN 101
#define COMMAND_NUM 20

//declare commands list to classify commands easier so that struct run() function clearly
typedef enum {
	h_,
	help_,
	d_,
	dir_,
	q_,
	quit_,
	hi_,
	history_,
	du_,
	dump_,
	e_,
	edit_,
	f_,
	fill_,
	reset_,
	opcode_,
	opcodelist_,
	assemble_,
	type_,
	symbol_
} command_list;

//node structure and head pointer for history functions
typedef struct history_node {
	char name[101];
	struct history_node* next;
}history_node;

//shell functions
void help();
void dir();
void quit();
void add_history(char* arg);
void remove_history_tail();
void clear_history();
void history();
int make_command(char* str);
int type(char* filename);
bool run(char* arg);
