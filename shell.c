#include "shell.h"

struct history_node* history_root = NULL;

const char* commands[COMMAND_NUM] = {
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
}

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

void quit() {
}

void add_history(struct history_node* root, char* arg) {
}
void remove_tail_history(struct history_node* root) {
}
void clear_history(struct history_node* root) {
}

void history() {

	printf("history\n");	
}

//have to edit
int str_to_hex(char* str) {
	if(str==NULL) return IMPOSSIBLE;
	int res = 0;
	int i;
	for(i=0;i<(int)strlen(str);i++) {
		res *= 16;
		if(str[i]>=65) res += str[i] - 'A' + 10;
		else res += str[i] - '0';
	}
	return res;
}

int make_command(char* str) {
	int i;
	for(i=0;i<COMMAND_NUM;i++) {
		if(!strcmp(str, commands[i])) return i;
	}
	return IMPOSSIBLE;
}

bool run(char* arg) {
	char *command, *param1, *param2, *param3;
	char arg_cpy[ARG_LEN];
	int p1, p2, p3;
	strcpy(arg_cpy, arg);
	add_history(history_root, arg_cpy);

	command = strtok(arg, " ");
	param1 = strtok(NULL, " ");
	param2 = strtok(NULL, " ");
	param3 = strtok(NULL, " ");
	p1 = str_to_hex(param1);
	p2 = str_to_hex(param2);
	p3 = str_to_hex(param3);

	int com = make_command(command);
	printf("%s\n", com);
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
			dump(p1, p2);
			return true;
		case e_:
		case edit_:
			edit(p1, p2);
			return true;
		case f_:
		case fill_:
			fill(p1, p2, p3);
			return true;
		case reset_:
			reset();
			return true;
		case opcode_:
			opcode(param1);
			return true;
		case opcodelist_:
			opcodelist();
			return true;
		default:
			remove_tail_history(history_root);
			undefined_command();
			return true;
	}
	//printf("%d\n%d\n%d\n%d\n", str_to_hex(command), str_to_hex(param1), str_to_hex(param2), str_to_hex(param3));
	//printf("%s\n%s\n%s\n%s\n", command, param1, param2, param3);
	return true;
}
