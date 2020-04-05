#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "memory.h"
#include "opcode.h"
#include "error.h"
#define IMPOSSIBLE 2000000000
#define ARG_LEN 101
#define COMMAND_NUM 17

typedef enum {false, true} bool;
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
	opcodelist_
} command_list;

extern const char* commands[COMMAND_NUM];

typedef struct history_node {
	char name[101];
	struct history_node* next;
}history_node;

void help();
void dir();
void quit();
void add_history(struct history_node* root, char* arg);
void remove_tail_history(struct history_node* root);
void clear_history(struct history_node* root);
void history();
int str_to_hex(char* str);
int make_command(char* str);
bool run(char* arg);
