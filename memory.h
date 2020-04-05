#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "error.h"
#define MAX_VAL 255
#define MAX_ADDR 1048575

//memory functions
bool is_valid_val(int val);
bool is_valid_addr(int addr);
int dump(int start, int end);
int edit(int idx, int val);
int fill(int start, int end, int val);
int reset();

//declare 1MB memory and last address for dump function
char mem[MAX_ADDR + 1] = {0};
int last_addr = -1;
