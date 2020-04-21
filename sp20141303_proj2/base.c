#include "base.h"

//convert each character with corresponding hexadecimal value
int char_to_hex(char c) {
	if('0' <= c && c <= '9') return c - '0';
	if('A' <= c && c <= 'F') return c - 'A' + 10;
	if('a' <= c && c <= 'f') return c - 'a' + 10;
	return IMPOSSIBLE;
}

//convert string with corresponding hexadecimal value
int str_to_hex(char* str) {
	//EMPTY means there is no parameter
	if(str==NULL) return EMPTY;

	int res = 0, tmp;
	int i;
	for(i = 0; i < (int)strlen(str); i++) {
		res *= 16;
		tmp = char_to_hex(str[i]);
		//IMPOSSIVLE means incorrect format hexadecimal like YJ
		if(tmp == IMPOSSIBLE) return IMPOSSIBLE;
		res += tmp;
	}
	return res;
}

//convert each character with corresponding decimal value
int char_to_int(char c) {
	if('0' <= c && c <= '9') return c - '0';
	return IMPOSSIBLE;
}

//convert string with corresponding decimal value
int str_to_int(char* str) {
	//EMPTY means there is no parameter
	if(str==NULL) return EMPTY;

	int res = 0, tmp;
	int i;
	for(i = 0; i < (int)strlen(str); i++) {
		res *= 10;
		tmp = char_to_int(str[i]);
		//IMPOSSIVLE means incorrect format decimal like AA
		if(tmp == IMPOSSIBLE) return IMPOSSIBLE;
		res += tmp;
	}
	return res;
}
