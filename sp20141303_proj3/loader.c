#include "loader.h"

//progaddr
int prog_addr = 0;

//symbol list root for maximum 3 files
struct symbol_node* symbol_root[3] = {NULL, };

//set progaddr
int progaddr(int addr) {
	if(!is_valid_addr(addr)) return ERROR;
	prog_addr = addr;
	return SUCCESS;
}

//find loc value for symbol with symbol name
int find_loc_by_name(char* name){
	int i;
	for(i = 0; i < 3; i++) {
		struct symbol_node* r = symbol_root[i];
		while(r != NULL && strcmp(r->name, name) != 0) {
			r = r->next;
		}
		if(r!=NULL) return r->LOC;
	}
	return ERROR;
}

//find loc value for symbol with symbol idx
int find_loc_by_idx(char* idx){
	int i;
	for(i = 0; i < 3; i++) {
		struct symbol_node* r = symbol_root[i];
		while(r != NULL && strcmp(r->idx, idx) != 0) {
			r = r->next;
		}
		if(r!=NULL) return r->LOC;
	}
	return ERROR;
}

//insert symbol node to symbol list
void insert_loader_symbol(struct symbol_node* root, char* name,  int loc) {
	struct symbol_node* new_ = (struct symbol_node*)malloc(sizeof(symbol_node));
	new_->next = NULL;
	strcpy(new_->name, name);
	new_->LOC = loc;
	if(root==NULL) {
		root = new_;
	}
	else {
		new_->next = root->next;
		root = new_;
	}
	return;
}

//clear all symbol list
void clear_loader_symbol() {
	int i;
	for(i = 0; i < 3; i++) {
		struct symbol_node* r = symbol_root[i];
		struct symbol_node* tmp;
		while(r != NULL) {
			tmp = r;
			r = r->next;
			free(tmp);
		}
	}
	return;
}

//pass1
int loader_pass1(char* fname1, char* fname2, char* fname3) {
	int i, j, val, len, tot = 0;
	char line[50];
	printf("control symbol address length\nsection name\n");
	printf("-----------------------------------\n");
	int fi;
	for(fi = 0; fi < 3; fi++) {
		char* cur;
		if(fi==0) cur = fname1;
		else if(fi==1) cur = fname2;
		else cur = fname3;
		if(cur == NULL) continue;
		FILE* fp;
		struct symbol_node* r;
		fp = fopen(cur, "r");
		if(fp == NULL) continue;
		fscanf(fp, "%s", line);
		
		for(j=1; j<7; j++) printf("%c", line[j]);
		printf("\t\t");
		char valstr[7], valname[7];
		for(j=7;j<13;j++) valstr[j-7] = line[j];
		valstr[6] = 0;

		val = str_to_hex(valstr);
		val += prog_addr;
		len = str_to_hex(line + 13);
		printf("%04x\t%04x\n", val, len);
		tot += len;
		fscanf(fp, "%s", line);
		int walk = 1, mx = strlen(line);
		while(walk < mx) {
			for(i=walk; i< walk+6;i++) valname[i - walk] = line[i];
			for(i=walk + 6; i< walk+12;i++) valstr[i - walk - 6] = line[i];
			val = str_to_hex(valstr);
			val += prog_addr;

			if(find_loc_by_name(valname) != ERROR) {
				printf("Error! Duplicate Symbol\n");
				return ERROR;
			}
			else {
				r = symbol_root[fi];
				insert_loader_symbol(r, valname, val);
			}
			walk += 12;
		}
		r = symbol_root[fi];
		while(r != NULL) {
			printf("\t\t%s\t%04x\n", r->name, r->LOC);
			r = r->next;
		}
	}
	printf("-----------------------------------\n");
	printf("\t\t total length ");
	printf("%04x\n", tot);
	return SUCCESS;
}


int loader_pass2(char* fname1, char* fname2, char* fname3) {

	return SUCCESS;
}

int loader(char* fname1, char* fname2, char* fname3){
	int err = loader_pass1(fname1, fname2, fname3);
	if(err == ERROR) return ERROR;

	clear_loader_symbol();
	return SUCCESS;
}

int bp() {
	printf("not completed...\n");
	return SUCCESS;
}

int runn() {
	printf("not completed...\n");
	return SUCCESS;
}
