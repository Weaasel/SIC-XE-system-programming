#include "assembler.h"

int progaddr(int addr);
int find_loc_by_name(char* name);
int find_loc_by_idx(char* idx);
void insert_loader_symbol(struct symbol_node* root, char* name, int loc);
void clear_loader_symbol();
int loader_pass1(char* fname1, char* fname2, char* fname3);
int loader_pass2(char* fname1, char* fname2, char* fname3);
int loader(char* fname1, char* fname2, char* fname3);
int bp();
int runn();
