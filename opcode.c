#include "opcode.h"

struct hash_node* hash_head[20] = {NULL};

void add_hash_node(struct hash_node* new_) {
	int idx = (new_->name[0] - 'A') % 20;
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

void init_optable() {
	int i;
	for(i = 0; i < 20; i++) hash_head[i] = NULL;
	char tmp[10];
	struct File* fp;
	fp = fopen("./opcode.txt", "r");
	if(fp == NULL) {
		printf("Error : opcode.txt does'nt exist.\n");
		return;
	}
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


int opcode(char* str) {
	if(str == NULL){
		undefined_argument();
		return ERROR;
	}
	int idx = (str[0] - 'A') % 20;
	struct hash_node* tmp = hash_head[idx];
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
