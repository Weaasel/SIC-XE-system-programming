#include "opcode.h"

//declare 20-sized hash table
struct hash_node* hash_head[20] = {NULL};

//insert new hash node to matched hash index
void add_hash_node(struct hash_node* new_) {
	int idx = (new_->name[0] - 'A') % 20; //given hash index depends on 1st Character of name
	//add new node to tail
	if(hash_head[idx] == NULL) hash_head[idx] = new_;
	else {
		struct hash_node* tail = hash_head[idx];
		while(tail->next != NULL) {
			tail = tail->next;
		}
		tail->next = new_;
	}
	return;
}

//initialize opcode table with given text file "opcode.txt"
void init_optable() {
	int i;
	for(i = 0; i < 20; i++) hash_head[i] = NULL;
	char tmp[10];
	FILE* fp;
	fp = fopen("./opcode.txt", "r");
	if(fp == NULL) {
		printf("Error : opcode.txt does'nt exist.\n");
		return;
	}
	//read opcode, mnemonic, format of each opcode node
	while(fscanf(fp, "%s", tmp) != EOF) {
		struct hash_node* new_ = (struct hash_node*)malloc(sizeof(hash_node));
		new_->next = NULL;
		strcpy(new_->code, tmp);
		fscanf(fp, "%s", tmp);
		strcpy(new_->name, tmp);
		fscanf(fp, "%s", tmp);
		strcpy(new_->data, tmp);
		add_hash_node(new_);
	}
	fclose(fp);
	return;
}

//output matching opcode with input opcode name
int opcode(char* str) {
	if(str == NULL){
		undefined_argument();
		return ERROR;
	}
	//get hash index
	int idx = (str[0] - 'A') % 20;
	struct hash_node* tmp = hash_head[idx];
	//find corresponding node while iterating
	while(tmp != NULL) {
		if(!strcmp(tmp->name, str)) {
			printf("opcode is %s\n", tmp->code);
			return SUCCESS;
		}
		tmp = tmp->next;
	}
	undefined_argument();
	return ERROR;
}

//print all opcode data in hash table separated with index
void opcodelist(){
	int i;
	for(i = 0; i < 20; i++) {
		printf("%3d : ", i);
		struct hash_node* tmp = hash_head[i];
		while(tmp != NULL) {
			if(tmp->next != NULL) printf("[%s, %s] -> ", tmp->name, tmp->code);
			else printf("[%s, %s]", tmp->name, tmp->code);
			tmp = tmp->next;
		}
		printf("\n");
	}
	return;
}

//before exit program, free all allocated memories used in opcode table
void clear_optable() {
	int i;
	for(i = 0; i < 20; i++) {
		struct hash_node* tmp;
		while(hash_head[i] != NULL) {
			tmp = hash_head[i];
			hash_head[i] = hash_head[i]->next;
			free(tmp);
		}
	}
	return;
}
