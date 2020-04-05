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
	"opcodelist"
};

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
	return;
}

//print list of directory / files on current directory
void dir() {
	struct dirent **namelist;
	int count, idx, len;
	struct stat buf;

	if((count = scandir("./", &namelist, NULL, alphasort)) != -1) {
		for(idx = 0; idx < count; idx++) {
			char fname[101];
			strcpy(fname, namelist[idx]->d_name);
			len = strlen(fname);
			lstat(namelist[idx]->d_name, &buf);
			
			if(fname[len-4] == '.' && fname[len-3] == 'o' && fname[len-2] == 'u' && fname[len-1] == 't') {
				printf("%s*\n", namelist[idx]->d_name);
			}
			else if(S_ISDIR(buf.st_mode)) {
				printf("%s/\n", namelist[idx]->d_name);
			}
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

//convert each character with corresponding hexadecimal value
int char_to_hex(char c) {
	if('0' <= c && c <= '9') return c - '0';
	if('A' <= c && c <= 'F') return c - 'A' + 10;
	if('a' <= c && c <= 'f') return c - 'a' + 10;
	return IMPOSSIBLE;
}

//convert string with corresponding hexadecimal value
int str_to_hex(char* str) {
	if(str==NULL) return EMPTY;

	int res = 0, tmp;
	int i;
	for(i = 0; i < (int)strlen(str); i++) {
		res *= 16;
		tmp = char_to_hex(str[i]);
		if(tmp == IMPOSSIBLE) return IMPOSSIBLE;
		res += tmp;
	}
	return res;
}

//math string wuth corresponding command number. This is for switch statement in run()
int make_command(char* str) {
	int i;
	for(i=0;i<COMMAND_NUM;i++) {
		if(!strcmp(str, valid_commands[i])) return i;
	}
	return IMPOSSIBLE;
}

//Tokenize whole argument with command and parameters, then execute corresponding job.
bool run(char* arg) {
	char *command, *param1, *param2, *param3;
	char arg_cpy[ARG_LEN];
	int p1, p2, p3, err;
	strcpy(arg_cpy, arg);
	add_history(arg_cpy);

	command = strtok(arg, " ");
	param1 = strtok(NULL, " ");
	param2 = strtok(NULL, " ");
	param3 = strtok(NULL, " ");
	p1 = str_to_hex(param1);
	p2 = str_to_hex(param2);
	p3 = str_to_hex(param3);
	
	int com = make_command(command);
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
			err = reset();
			if(err == ERROR) remove_history_tail();
			return true;
		case opcode_:
			err = opcode(param1);
			if(err == ERROR) remove_history_tail();
			return true;
		case opcodelist_:
			opcodelist();
			return true;
		default:
			remove_history_tail();
			undefined_command();
			return true;
	}
	return true;
}
