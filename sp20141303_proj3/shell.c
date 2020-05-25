#include "shell.h"

//constant string array for valid shell commands to match in run()
const char* valid_commands[COMMAND_NUM] = {
	"h",
	"help",
	"d",
	"dir",
	"q",
	"quit",
	"hi",
	"history",
	"du",
	"dump",
	"e",
	"edit",
	"f",
	"fill",
	"reset",
	"opcode",
	"opcodelist",
	"assemble",
	"type",
	"symbol",
	"progaddr",
	"loader",
	"bp",
	"run"
};

//head node pointer for history list
struct history_node* history_head = NULL;

//print all valid commands with their correct format
void help() {
	printf("h[elp]\n");
	printf("d[ir]\n");
	printf("q[uit]\n");
	printf("hi[story]\n");
	printf("du[mp] [start, end]\n");
	printf("e[dit] address, value\n");
	printf("f[ill] start, end, value\n");
	printf("reset\n");
	printf("opcode mnemonic\n");
	printf("opcodelist\n");
	printf("assemble filename\n");
	printf("type filename\n");
	printf("symbol\n");
	printf("progaddr address\n");
	printf("loader filename\n");
	printf("bp address\n");
	printf("run\n");
	return;
}

//print list of directory / files on current directory
void dir() {
	struct dirent **namelist;
	int count, idx, len;
	struct stat buf;
	
	//scan all files in directory
	if((count = scandir("./", &namelist, NULL, alphasort)) != -1) {
		for(idx = 0; idx < count; idx++) {
			char fname[101];
			strcpy(fname, namelist[idx]->d_name);
			len = strlen(fname);
			lstat(namelist[idx]->d_name, &buf);
			
			//exe file(a.out*)
			if(fname[len-4] == '.' && fname[len-3] == 'o' && fname[len-2] == 'u' && fname[len-1] == 't') {
				printf("%s*\n", namelist[idx]->d_name);
			}
			//directory(a/)
			else if(S_ISDIR(buf.st_mode)) {
				printf("%s/\n", namelist[idx]->d_name);
			}
			//regular files
			else if(S_ISREG(buf.st_mode)) {
				printf("%s\n", namelist[idx]->d_name);
			}
		}
		for(idx = 0; idx < count; idx++) {
			free(namelist[idx]);
		}
		free(namelist);
	}
	return;
}

//nothing to do in here to quit program
void quit() {
	return;
}

//add command history to history head
void add_history(char* arg) {
	struct history_node* new_ = (struct history_node*)malloc(sizeof(history_node));
	new_->next = NULL;
	strcpy(new_->name, arg);
	if(history_head==NULL) history_head = new_;
	else {
		struct history_node* tail = history_head;
		while(tail->next != NULL) {
			tail = tail->next;
		}
		tail->next = new_;
	}
	return;
}

//remove last history node. This is for exceptions(invalid commands)
void remove_history_tail() {
	if(history_head==NULL) return;
	if(history_head->next == NULL) {
		free(history_head);
		history_head = NULL;
		return;
	}
	struct history_node* tmp;
	tmp = history_head;
	while((tmp->next)->next != NULL) {
		tmp = tmp->next;
	}
	free(tmp->next);
	tmp->next = NULL;
	return;
}

//clear all allocated history memories before exit program
void clear_history() {
	struct history_node* tmp;
	while(history_head != NULL) {
		tmp = history_head;
		history_head = history_head->next;
		free(tmp);
	}
	return;
}

//print valid commands history with indices
void history() {
	int idx = 1;
	struct history_node* tmp = history_head;
	while(tmp) {
		printf("%3d ", idx);
		printf("%s\n", tmp->name);
		idx++;
		tmp = tmp->next;
	}
	return;
}

//math string wuth corresponding command number. This is for switch statement in run()
int make_command(char* str) {
	int i;
	for(i=0;i<COMMAND_NUM;i++) {
		if(!strcmp(str, valid_commands[i])) return i;
	}
	return IMPOSSIBLE;
}

//print out file. If there is no file with given name, print error.
int type(char* filename) {
	FILE* fp;
	fp = fopen(filename, "r");
	if(fp == NULL) {
		invalid_filename();
		return ERROR;
	}
	char c;
	while(fscanf(fp, "%c", &c) != EOF) printf("%c", c);
	fclose(fp);
	return SUCCESS;
}

//Tokenize whole argument with command and parameters, then execute corresponding job.
bool run(char* arg) {
	char *command, *param1, *param2, *param3;
	char arg_cpy[ARG_LEN];
	int p1, p2, p3, err, len1 = 0, len2 = 0;
	strcpy(arg_cpy, arg);
	add_history(arg_cpy);

	//split argument to command, param1~3, param1~3 can be NULL
	command = strtok(arg, " ");
	param1 = strtok(NULL, " ");
	param2 = strtok(NULL, " ");
	param3 = strtok(NULL, " ");
	//for make str_to_hexa easier, erase ','
	if(param1 != NULL) len1 = strlen(param1);
	if(param2 != NULL) len2 = strlen(param2);
	if(len1>0 && param1[len1-1]==',') param1[len1-1] = 0;
	if(len2>0 && param2[len2-1]==',') param2[len2-1] = 0;

	//p1~3 == integer version of param1~3
	p1 = str_to_hex(param1);
	p2 = str_to_hex(param2);
	p3 = str_to_hex(param3);
	int com = make_command(command);
	//in switch statement, if some cases return ERROR, we will remove history of its command
	switch(com) {
		case h_:
		case help_:
			help();
			return true;
		case d_:
		case dir_:
			dir();
			return true;
		case q_:
		case quit_:
			quit();
			return false;
		case hi_:
		case history_:
			history();
			return true;
		case du_:
		case dump_:
			err = dump(p1, p2);
			if(err == ERROR) remove_history_tail();
			return true;
		case e_:
		case edit_:
			err = edit(p1, p2);
			if(err == ERROR) remove_history_tail();
			return true;
		case f_:
		case fill_:
			err = fill(p1, p2, p3);
			if(err == ERROR) remove_history_tail();
			return true;
		case reset_:
			reset();
			return true;
		case opcode_:
			err = opcode(param1);
			if(err == ERROR) remove_history_tail();
			return true;
		case opcodelist_:
			opcodelist();
			return true;
		case assemble_:
			err = assemble(param1);
			if(err == ERROR) remove_history_tail();
			return true;
		case type_:
			err = type(param1);
			if(err == ERROR) remove_history_tail();
			return true;
		case symbol_:
			err = symbol();
			if(err == ERROR) remove_history_tail();
			return true;
		case progaddr_:
			err = progaddr(p1);
			if(err == ERROR) remove_history_tail();
			return true;
		case loader_:
			err = loader(param1, param2, param3);
			if(err == ERROR) remove_history_tail();
			return true;
		case bp_:
			err = bp();
			if(err == ERROR) remove_history_tail();
			return true;
		case run_:
			err = runn();
			if(err == ERROR) remove_history_tail();
			return true;
		default:
			remove_history_tail();
			undefined_command();
			return true;
	}
	return true;
}
