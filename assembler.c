#include "assembler.h"

//declare symbol table, line list roots
struct symbol_node* sym_root = NULL;
struct line_node* line_root = NULL;

//declare symbol table, line list temporary roots
struct symbol_node* sym_tmp = NULL;
struct line_node* line_tmp = NULL;

int start_addr = 0;
int base_LOC = 0;
int LOCCTR = 0;
int line = 5;
int prog_len = 0;

bool in_range(int disp) {
	return disp >= -2048 && disp <= 2047;
}

int register_num(char* c) {
	if(c == NULL) return ERROR;
	if(!strcmp(c, "A")) return 0;
	if(!strcmp(c, "X")) return 1;
	if(!strcmp(c, "L")) return 2;
	if(!strcmp(c, "PC")) return 8;
	if(!strcmp(c, "SW")) return 9;
	if(!strcmp(c, "B")) return 3;
	if(!strcmp(c, "S")) return 4;
	if(!strcmp(c, "T")) return 5;
	if(!strcmp(c, "F")) return 6;
	return ERROR;
}

//clear allocated memories for symbol table
void clear_symbol(struct symbol_node* root) {
	struct symbol_node* tmp;
	while(root != NULL) {
		tmp = root;
		root = root->next;
		free(tmp);
	}
}

//clear allocated memories for line_data
void clear_line(struct line_node* root) {
	struct line_node* tmp;
	while(root != NULL) {
		tmp = root;
		root = root->next;
		free(tmp);
	}
}

//clear all allocated memories for assembler
void clear_assembler() {
	clear_symbol(sym_tmp);
	clear_line(line_tmp);
	clear_symbol(sym_root);
	clear_line(line_root);
	return;
}

//insert symbol data by increasing order
void insert_symbol(char* name, int LOC) {
	struct symbol_node* new_ = (struct symbol_node*)malloc(sizeof(symbol_node));
	new_->next = NULL;
	strcpy(new_->name, name);
	new_->LOC = LOC;
	
	struct symbol_node* pos = sym_tmp;
	int cnt = 0;
	while(pos != NULL && strcmp(pos->name, name) < 0) {
		pos = pos->next;
		cnt++;
	}
	//if new_ be root
	if(cnt == 0) {
		new_->next = sym_tmp;
		sym_tmp = new_;
		return;
	}
	//else
	cnt--;
	pos = sym_tmp;
	while(cnt--) pos = pos->next;
	new_->next = pos->next;
	pos->next = new_;
	return;
}

//find LOC of symbol
int find_symbol(char* name) {
	if(name == NULL) return ERROR;
	struct symbol_node* tmp = sym_tmp;
	while(tmp != NULL) {
		if(!strcmp(tmp->name, name)) {
			return tmp->LOC;
		}
		tmp = tmp->next;
	}
	return ERROR;
}

//symbol function : print symbol name and LOC by order
int symbol() {
	if(sym_root == NULL) {
		printf("Error : no symbol!\n");
		return ERROR;
	}
	struct symbol_node* tmp = sym_root;
	while(tmp != NULL) {
		printf("\t%s\t%04X\n", tmp->name, tmp->LOC);
		tmp = tmp->next;
	}

	return SUCCESS;
}

//insert new line node
void insert_line(struct line_node* new_) {
	if(line_tmp == NULL) {
		line_tmp = new_;
		return;
	}
	struct line_node* tmp = line_tmp;
	while(tmp->next != NULL) tmp = tmp->next;
	tmp->next = new_;
	return;
}

//pass1 : parse each line of asm file and save symbol table / line data
int pass1(char* filename) {
	int len = strlen(filename);
	FILE* fp = NULL;
	fp = fopen(filename, "r");
	//if not in directory or not asm file, return ERROR
	if(fp == NULL || len <= 3 || !(filename[len-4] == '.' && filename[len-3] == 'a' && filename[len-2] == 's' && filename[len-1] == 'm')) {
		printf("Error : Invalid filename\n");
		return ERROR;
	}
	//input current line
	char cur_line[100];
	char raw_line[100];
	while(1) {
		fgets(cur_line, 100, fp);
		cur_line[(int)strlen(cur_line)-1] = 0;
		char *param1, *param2, *param3, *param4;
		strcpy(raw_line, cur_line);
		//parse data
		param1 = strtok(cur_line, " ");
		param2 = strtok(NULL, " ");
		param3 = strtok(NULL, " ");
		param4 = strtok(NULL, " ");
		
		//initialize new line node
		struct line_node* new_ = (struct line_node*)malloc(sizeof(line_node));
	
		int j;
		new_->next = NULL;
		new_->idx = line;
		new_->LOC = 0;
		new_->PC = 0;
		new_->format = 0;
		new_->object_len = 0;
		for(j=0;j<100;j++) new_->raw[j] = 0;
		for(j=0;j<10;j++) new_->param1[j] = 0;
		for(j=0;j<10;j++) new_->param2[j] = 0;
		for(j=0;j<10;j++) new_->param3[j] = 0;
		for(j=0;j<10;j++) new_->param4[j] = 0;
		for(j=0;j<8;j++) new_->object_code[j] = 0;
		//may have to debug
		
		if(raw_line != NULL) strcpy(new_->raw, raw_line);
		if(param1 != NULL) strcpy(new_->param1, param1);
		if(param2 != NULL) strcpy(new_->param2, param2);
		if(param3 != NULL) strcpy(new_->param3, param3);
		if(param4 != NULL) strcpy(new_->param4, param4);
		new_->is_symbol = false;
		new_->is_start = false;
		new_->is_end = false;
		new_->is_base = false;
		new_->is_opcode = false;
		new_->is_const = false;
		new_->is_var = false;
	
		if(!strcmp(param1, "END")) {
			new_->is_end = true;
			prog_len = LOCCTR - start_addr;
			insert_line(new_);
			break;
		}
		//comment line
		else if(cur_line[0] == '.') {
			continue;
		}
		else if(param2 != NULL && !strcmp(param2, "START")) {
			new_->is_start = true;
			start_addr = str_to_hex(param3);
			line = 5;
			base_LOC = 0;
			new_->idx = line;
			LOCCTR = start_addr;
			insert_line(new_);
			line += 5;
			continue;
		}
		else if(!strcmp(param1, "BASE")) {
			new_->is_base = true;
			new_->idx = line;
			insert_line(new_);
			line += 5;
			continue;
		}
		else {
			//have symbol
			if(raw_line[0] != ' '){
				new_->is_symbol = true;
				int loc = find_symbol(param1);
				//if there exists same symbol
				if(loc != ERROR) {
					printf("line %d Error : Duplicate Symbol!\n", line);
					return ERROR;
				}
				//new symbol
				else {
					insert_symbol(param1, LOCCTR);
				}
				//opcode
				if(get_format(param2) != ERROR ) {
					new_->is_opcode = true;
					new_->LOC = LOCCTR;
					LOCCTR += get_format(param2);
					new_->PC = LOCCTR;
					new_->format = get_format(param2);
					insert_line(new_);
				}
				else if(!strcmp(param2, "WORD")) {
					new_->is_const = true;
					new_->LOC = LOCCTR;
					LOCCTR += 3;
					new_->PC = LOCCTR;
					insert_line(new_);
				}
				else if(!strcmp(param2, "RESW")) {
					new_->is_var = true;
					new_->LOC = LOCCTR;
					LOCCTR += 3 * str_to_int(param3);
					new_->PC = LOCCTR;
					insert_line(new_);
				}
				else if(!strcmp(param2, "RESB")) {
					new_->is_var = true;
					new_->LOC = LOCCTR;
					LOCCTR += str_to_int(param3);
					new_->PC = LOCCTR;
					insert_line(new_);
				}
				else if(!strcmp(param2, "BYTE")) {
					new_->is_const = true;
					new_->LOC = LOCCTR;
					if(param3[0] == 'C') LOCCTR += (int)strlen(param3) - 3;
					else LOCCTR += ((int)strlen(param3) - 3) / 2;
					new_->PC = LOCCTR;
					insert_line(new_);
				}
				else {
					printf("line %d Error : invalid operation code!\n", line);
					return ERROR;
				}
			}
			//no symbol
			else {
				if(get_format(param1) != ERROR) {
					new_->is_opcode = true;
					new_->LOC = LOCCTR;
					LOCCTR += get_format(param1);
					new_->PC = LOCCTR;
					new_->format = get_format(param1);
					insert_line(new_);
				}
				else if(!strcmp(param1, "WORD")) {
					new_->is_const = true;
					new_->LOC = LOCCTR;
					LOCCTR += 3;
					new_->PC = LOCCTR;
					insert_line(new_);
				}
				else if(!strcmp(param1, "RESW")) {
					new_->is_var = true;
					new_->LOC = LOCCTR;
					LOCCTR += 3 * str_to_int(param2);
					new_->PC = LOCCTR;
					insert_line(new_);
				}
				else if(!strcmp(param1, "RESB")) {
					new_->is_var = true;
					new_->LOC = LOCCTR;
					LOCCTR += str_to_int(param2);
					new_->PC = LOCCTR;
					insert_line(new_);
				}
				else if(!strcmp(param1, "BYTE")) {
					new_->is_const = true;
					new_->LOC = LOCCTR;
					if(param2[0] == 'C') LOCCTR += (int)strlen(param2) - 3;
					else LOCCTR += ((int)strlen(param2) - 3) / 2;
					new_->PC = LOCCTR;
					insert_line(new_);
				}
				else {
					printf("line %d Error : invalid operation code!\n", line);
					return ERROR;
				}
			}
		}
		line += 5;
	}
	fclose(fp);
	return SUCCESS;
}

//pass2 : make opcode for each line with corresponding line data and symbol table
int pass2(char* filename) {
	struct line_node* tmp = line_tmp;
	while(tmp != NULL) {
		//have symbol
		if(tmp->is_symbol) {
			//no need to make object code
			if(tmp->is_start || tmp->is_base || tmp->is_end || tmp->is_var) {
				if(tmp->is_base) {
					base_LOC = find_symbol(tmp->param3);	
				}
				tmp->object_len = 0;
				tmp = tmp->next;
				continue;
			}
			if(tmp->is_opcode) {
				tmp->object_len = (tmp->format) * 2;
				if(tmp->format == 1) {
					int opval = opname_to_hex(tmp->param2);
					tmp->object_code[0] = opval / 16;
					tmp->object_code[1] = opval % 16;
				}
				else if(tmp->format == 2) {
					int opval = opname_to_hex(tmp->param2);
					int r1 = register_num(tmp->param3);
					int r2 = register_num(tmp->param4);
					tmp->object_code[0] = opval / 16;
					tmp->object_code[1] = opval % 16;
					tmp->object_code[2] = r1 == ERROR ? 0 : r1;
					tmp->object_code[3] = r2 == ERROR ? 0 : r2;
				}
				else if(tmp->format == 3) {
					int len = strlen(tmp->param2);
					if(tmp->param2[len-1] == ',') tmp->param2[len-1] = 0;
					len = strlen(tmp->param3);
					if(tmp->param3[len-1] == ',') tmp->param3[len-1] = 0;
					int opval = opname_to_hex(tmp->param2);
					int disp;
					tmp->object_code[0] = opval / 16;
					tmp->object_code[1] = opval % 16;
					
					int n = 0, i = 0, x = 0, b = 0, p = 0, e = 0;
					bool flag = false;
					//check addressing mode

					//immediate addressing
					if(tmp->param3[0] == '#') {
						n = 0, i = 1;
						disp = find_symbol(tmp->param3+1);
						if(disp == ERROR) {
							disp = str_to_int(tmp->param3+1);
							flag = true;
						}
					}
					//indirect addressing
					else if(tmp->param3[0] == '@') {
						n = 1, i = 0;
						disp = find_symbol(tmp->param3+1);
						if(disp == ERROR) {
							disp = str_to_int(tmp->param3+1);
							flag = true;
						}
					}
					//simple addressing
					else {
						n = 1, i = 1;
						disp = find_symbol(tmp->param3);
						if(disp == ERROR){
							disp = str_to_int(tmp->param3);
							flag = true;
						}
					}
					
					if(!flag) {
						disp -= tmp->PC;
						if(in_range(disp)) {
							p = 1;
						}
						else {
							disp += tmp->PC;
							disp -= base_LOC;
							b = 1;
						}
					}
					if(tmp->param4[0] == 'X') {
						x = 1;
					}
				
					if(disp < 0 ) disp += 4096;

					tmp->object_code[1] += n * 2;
					tmp->object_code[1] += i;

					tmp->object_code[2] = 8*x + 4*b + 2*p + e;
					tmp->object_code[3] = disp / 256;
					disp -= tmp->object_code[3] * 256;
					tmp->object_code[4] = disp / 16;
					disp -= tmp->object_code[4] * 16;
					tmp->object_code[5] = disp;
				}
				else if(tmp->format == 4) {
					int len = strlen(tmp->param2);
					if(tmp->param2[len-1] == ',') tmp->param2[len-1] = 0;
					len = strlen(tmp->param3);
					if(tmp->param3[len-1] == ',') tmp->param3[len-1] = 0;
					int opval = opname_to_hex(tmp->param2+1);
					int disp;
					tmp->object_code[0] = opval / 16;
					tmp->object_code[1] = opval % 16;
					
					int n = 0, i = 0, x = 0, b = 0, p = 0, e = 0;
					//check addressing mode
					//immediate addressing
					if(tmp->param3[0] == '#') {
						n = 0, i = 1;
						disp = find_symbol(tmp->param3+1);
						if(disp == ERROR) disp = str_to_int(tmp->param3+1);
					}
					//indirect addressing
					else if(tmp->param3[0] == '@') {
						n = 1, i = 0;
						disp = find_symbol(tmp->param3+1);
						if(disp == ERROR) disp = str_to_int(tmp->param3+1);
					}
					//simple addressing
					else {
						n = 1, i = 1;
						disp = find_symbol(tmp->param3);
						if(disp == ERROR) disp = str_to_int(tmp->param3);
					}
					if(tmp->param4[0] == 'X') {
						x = 1;
					}
					
					e = 1;
					tmp->object_code[1] += n * 2;
					tmp->object_code[1] += i;
					tmp->object_code[2] = 8*x + 4*b + 2*p + e;
					
					tmp->object_code[3] = disp / 65536;
					disp -= tmp->object_code[3] * 65536;
					tmp->object_code[4] = disp / 4096;
					disp -= tmp->object_code[4] * 4096;
					tmp->object_code[5] = disp / 256;
					disp -= tmp->object_code[5] * 256;
					tmp->object_code[6] = disp / 16;
					disp -= tmp->object_code[6] * 16;
					tmp->object_code[7] = disp;
				}
			}
			//constants
			else {
				int disp;
				//BYTE
				if(!strcmp(tmp->param2, "BYTE")) {
					if(tmp->param3[0] == 'C') {
						tmp->object_len = 6;
						int c1 = tmp->param3[2];
						int c2 = tmp->param3[3];
						int c3 = tmp->param3[4];

						tmp->object_code[0] = c1/16;
						tmp->object_code[1] = c1%16;
						tmp->object_code[2] = c2/16;
						tmp->object_code[3] = c2%16;
						tmp->object_code[4] = c3/16;
						tmp->object_code[5] = c3%16;
						
					}
					else if(tmp->param3[0] == 'X') {
						tmp->object_len = 2;
						tmp->object_code[0] = char_to_hex(tmp->param3[2]);
						tmp->object_code[1] = char_to_hex(tmp->param3[3]);
					}
				}
				//WORD
				else {
					tmp->object_len = 6;
					disp = str_to_hex(tmp->param3);
					tmp->object_code[0] = disp / 1048576;
					disp -= tmp->object_code[0] * 1048576;
					tmp->object_code[1] = disp / 65536;
					disp -= tmp->object_code[1] * 65536;
					tmp->object_code[2] = disp / 4096;
					disp -= tmp->object_code[2] * 4096;
					tmp->object_code[3] = disp / 256;
					disp -= tmp->object_code[3] * 256;
					tmp->object_code[4] = disp / 16;
					disp -= tmp->object_code[4] * 16;
					tmp->object_code[5] = disp;
				}
			}
		}
		//no symbol
		else {
			//no need to make object code
			if(tmp->is_start || tmp->is_base || tmp->is_end || tmp->is_var) {
				if(tmp->is_base) {
					base_LOC = find_symbol(tmp->param2);	
				}
				tmp->object_len = 0;
				tmp = tmp->next;
				continue;
			}
			if(tmp->is_opcode) {
				tmp->object_len = (tmp->format) * 2;
				if(tmp->format == 1) {
					int opval = opname_to_hex(tmp->param1);
					tmp->object_code[0] = opval / 16;
					tmp->object_code[1] = opval % 16;
				}
				else if(tmp->format == 2) {
					int opval = opname_to_hex(tmp->param1);
					int r1 = register_num(tmp->param2);
					int r2 = register_num(tmp->param3);
					tmp->object_code[0] = opval / 16;
					tmp->object_code[1] = opval % 16;
					tmp->object_code[2] = r1 == ERROR ? 0 : r1;
					tmp->object_code[3] = r2 == ERROR ? 0 : r2;
				}
				else if(tmp->format == 3) {
					int len = strlen(tmp->param1);
					if(tmp->param1[len-1] == ',') tmp->param1[len-1] = 0;
					len = strlen(tmp->param2);
					if(tmp->param2[len-1] == ',') tmp->param2[len-1] = 0;
					
					int opval = opname_to_hex(tmp->param1);
					int disp;
					tmp->object_code[0] = opval / 16;
					tmp->object_code[1] = opval % 16;
					
					int n = 0, i = 0, x = 0, b = 0, p = 0, e = 0;
					bool flag = false;
					//check addressing mode

					//immediate addressing
					if(tmp->param2[0] == '#') {
						n = 0, i = 1;
						disp = find_symbol(tmp->param2+1);
						if(disp == ERROR) {
							disp = str_to_int(tmp->param2+1);
							flag = true;
						}
					}
					//indirect addressing
					else if(tmp->param2[0] == '@') {
						n = 1, i = 0;
						disp = find_symbol(tmp->param2+1);
						if(disp == ERROR) {
							disp = str_to_int(tmp->param2+1);
							flag = true;
						}
					}
					//simple addressing
					else {
						n = 1, i = 1;
						disp = find_symbol(tmp->param2);
						if(disp == ERROR) {
							disp = str_to_int(tmp->param2);
							flag = true;
						}
					}
					
					if(!flag) {
						disp -= tmp->PC;
						if(in_range(disp)) {
							p = 1;
						}
						else {
							disp += tmp->PC;
							disp -= base_LOC;
							b = 1;
						}
					}
					if(tmp->param3[0] == 'X') {
						x = 1;
					}
				
					if(disp < 0 ) disp += 4096;
					tmp->object_code[1] += n * 2;
					tmp->object_code[1] += i;

					tmp->object_code[2] = 8*x + 4*b + 2*p + e;
					tmp->object_code[3] = disp / 256;
					disp -= tmp->object_code[3] * 256;
					tmp->object_code[4] = disp / 16;
					disp -= tmp->object_code[4] * 16;
					tmp->object_code[5] = disp;
				}
				else if(tmp->format == 4) {
					int len = strlen(tmp->param1);
					if(tmp->param1[len-1] == ',') tmp->param1[len-1] = 0;
					len = strlen(tmp->param2);
					if(tmp->param2[len-1] == ',') tmp->param2[len-1] = 0;
				
					int opval = opname_to_hex(tmp->param1+1);
					int disp;
					tmp->object_code[0] = opval / 16;
					tmp->object_code[1] = opval % 16;
					
					int n = 0, i = 0, x = 0, b = 0, p = 0, e = 0;
					//check addressing mode
					//immediate addressing
					if(tmp->param2[0] == '#') {
						n = 0, i = 1;
						disp = find_symbol(tmp->param2+1);
						if(disp == ERROR) disp = str_to_int(tmp->param2+1);
					}
					//indirect addressing
					else if(tmp->param2[0] == '@') {
						n = 1, i = 0;
						disp = find_symbol(tmp->param2+1);
						if(disp == ERROR) disp = str_to_int(tmp->param2+1);
					}
					//simple addressing
					else {
						n = 1, i = 1;
						disp = find_symbol(tmp->param2);
						if(disp == ERROR) disp = str_to_int(tmp->param2);
					}
					if(tmp->param3[0] == 'X') {
						x = 1;
					}
					
					e = 1;
					tmp->object_code[1] += n * 2;
					tmp->object_code[1] += i;
					tmp->object_code[2] = 8*x + 4*b + 2*p + e;
					
					tmp->object_code[3] = disp / 65536;
					disp -= tmp->object_code[3] * 65536;
					tmp->object_code[4] = disp / 4096;
					disp -= tmp->object_code[4] * 4096;
					tmp->object_code[5] = disp / 256;
					disp -= tmp->object_code[5] * 256;
					tmp->object_code[6] = disp / 16;
					disp -= tmp->object_code[6] * 16;
					tmp->object_code[7] = disp;
				}
			}
			//constants
			else {
				int disp;
				//BYTE
				if(!strcmp(tmp->param1, "BYTE")) {
					if(tmp->param2[0] == 'C') {
						tmp->object_len = 6;
						int c1 = tmp->param2[2];
						int c2 = tmp->param2[3];
						int c3 = tmp->param2[4];

						tmp->object_code[0] = c1/16;
						tmp->object_code[1] = c1%16;
						tmp->object_code[2] = c2/16;
						tmp->object_code[3] = c2%16;
						tmp->object_code[4] = c3/16;
						tmp->object_code[5] = c3%16;
						
					}
					else if(tmp->param2[0] == 'X') {
						tmp->object_len = 2;
						tmp->object_code[0] = char_to_hex(tmp->param2[2]);
						tmp->object_code[1] = char_to_hex(tmp->param2[3]);
					}
				}
				//WORD
				else {
					tmp->object_len = 6;
					disp = str_to_hex(tmp->param2);
					tmp->object_code[0] = disp / 1048576;
					disp -= tmp->object_code[0] * 1048576;
					tmp->object_code[1] = disp / 65536;
					disp -= tmp->object_code[1] * 65536;
					tmp->object_code[2] = disp / 4096;
					disp -= tmp->object_code[2] * 4096;
					tmp->object_code[3] = disp / 256;
					disp -= tmp->object_code[3] * 256;
					tmp->object_code[4] = disp / 16;
					disp -= tmp->object_code[4] * 16;
					tmp->object_code[5] = disp;
				}
			}
		}
		tmp = tmp->next;
	}
	return SUCCESS;
}

void make_lst(char* filename) {
	FILE* fp;
	int len = strlen(filename);
	filename[len-3] = 'l';
	filename[len-2] = 's';
	filename[len-1] = 't';
	fp = fopen(filename, "w");

	struct line_node* tmp1 = line_root;
	while(tmp1 != NULL) {
		if(tmp1->is_base || tmp1->is_end) fprintf(fp, "%d\t\t%s\n", tmp1->idx, tmp1->raw);
		else if(tmp1->is_start || tmp1->is_var) fprintf(fp, "%d\t%04X\t%s\n", tmp1->idx, tmp1->LOC, tmp1->raw);
		else {
			fprintf(fp, "%d\t%04X\t%-25s\t", tmp1->idx, tmp1->LOC, tmp1->raw);
			int i;
			for(i=0;i<tmp1->object_len;i++) fprintf(fp, "%X", tmp1->object_code[i]);
			fprintf(fp, "\n");
		}
		tmp1 = tmp1->next;
	}
	fclose(fp);
}

void make_obj(char* filename){ 
	FILE* fp;
	int len = strlen(filename);
	filename[len-3] = 'o';
	filename[len-2] = 'b';
	filename[len-1] = 'j';
	fp = fopen(filename, "w");
	
	//H header
	struct line_node* tmp = line_root;
	fprintf(fp, "H%-6s%06X%06X\n", tmp->param1, start_addr, prog_len);
	
	//T header
	tmp = line_root;

	int arr[500], i;
	
	while(tmp != NULL) {
		while(tmp != NULL && tmp->object_len == 0) tmp = tmp->next;
		if(tmp == NULL) break;
		int sLOC = tmp->LOC;
		fprintf(fp, "T%06X", sLOC);


		int cnt = 0;
		while(tmp != NULL && tmp->object_len > 0) {
			for(i = 0; i< tmp->object_len; i++) {
				arr[cnt++] = tmp->object_code[i];
			}
			tmp = tmp->next;
		}
		fprintf(fp, "%02X", cnt/2);
		for(i = 0; i < cnt; i++) fprintf(fp, "%X", arr[i]);
		fprintf(fp, "\n");
	}


	//M header
	tmp = line_root;
	while(tmp!=NULL) {
		if(tmp->format == 4 && !(tmp->is_symbol && tmp->param3[0] == '#') && !(!tmp->is_symbol && tmp->param2[0] == '#')) {
			fprintf(fp, "M%06X05\n", tmp->LOC + 1);
		}
		tmp = tmp->next;
	}

	//E header
	tmp = line_root;
	int exec_LOC;

	while(tmp->object_len == 0) tmp = tmp->next;
	exec_LOC = tmp->LOC;
	fprintf(fp, "E%06X", exec_LOC);

	fclose(fp);
	filename[len-3] = 'a';
	filename[len-2] = 's';
	filename[len-1] = 'm';
}

//assemble .asm file
int assemble(char* filename) {
	int err = pass1(filename);
	if(err == ERROR) {
		clear_symbol(sym_tmp);
		clear_line(line_tmp);
		return ERROR;
	}
	err = pass2(filename);
	if(err == ERROR) {
		clear_symbol(sym_tmp);
		clear_line(line_tmp);
		return ERROR;
	}
	else {
		clear_symbol(sym_root);
		clear_line(line_root);
		sym_root = sym_tmp;
		line_root = line_tmp;
		sym_tmp = NULL;
		line_tmp = NULL;
		
		make_lst(filename);
		make_obj(filename);
		printf("Successfully assemble %s!\n", filename);
		return SUCCESS;
	}
	return ERROR;
}

