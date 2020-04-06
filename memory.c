#include "memory.h"

//declare 1MB memory and last address for dump function
unsigned char mem[MAX_ADDR + 1] = {0};
int last_addr = -1;

//return if value is in range 0x0 ~ 0xFF
bool is_valid_val(int val) {
	return (val >= 0 && val <= MAX_VAL);
}

//return if address is in range 0x0 ~ 0xFFFFF
bool is_valid_addr(int addr) {
	return (addr >= 0 && addr <= MAX_ADDR);
}

//dump memory from start to end, if not valid address range, return error
int dump(int start, int end){
	//classify three formats
	//dump
	if(start == EMPTY && end == EMPTY) {
		start = (last_addr + 1) % (MAX_ADDR + 1);
		end = start + 159;
	}

	//dump start
	else if(end == EMPTY) {
		if(!is_valid_addr(start)) {
			out_of_range();
			return ERROR;
		}
		end = start + 159;
	}

	//dump start, end
	else {
		if(!is_valid_addr(start) || !is_valid_addr(end)) {
			out_of_range();
			return ERROR;
		}
		else if (start > end) {
			start_bigger_than_end();
			return ERROR;
		}
	}

	//start , end 정해졌고 여기서부터 원칙대로 출력. end = min(end, MAC_ADDR) 해주고.
	if(end > MAX_ADDR) end = MAX_ADDR;
	int r_start = start - start%16;
	int r_end = end - end%16;
	int i, j;
	for(i = r_start; i <= r_end; i += 16) {
		printf("%05X ", i);
		for(j = 0; j < 16; j++){
			if(i + j < start || i + j > end) printf("   ");
			else {
				printf("%02X ", mem[i + j]);
			}
		}
		printf("; ");
		for(j = 0; j < 16; j++) {
			if(i + j < start || i + j > end) printf(".");
			else {
				if(mem[i + j] >= 0x20 && mem[i+j] <= 0x7E) printf("%c", mem[i+j]);
				else printf(".");
			}
		}
		printf("\n");
	}
	last_addr = end;
	return SUCCESS;
}

//edit memory in idx to val, if not valid address or val, return error
int edit(int idx, int val) {
	if(idx == EMPTY || val == EMPTY) {
		undefined_argument();
		return ERROR;
	}
	else if(!is_valid_addr(idx) || !is_valid_val(val)) {
		out_of_range();
		return ERROR;
	}
	mem[idx] = val;
	return SUCCESS;
}

//fill memory from start to end with val, if not valid argument/address or val, return error 
int fill(int start, int end, int val) {
	if(start == EMPTY || end == EMPTY || val == EMPTY) {
		undefined_argument();
		return ERROR;
	}
	else if(!is_valid_addr(start) || !is_valid_addr(end) || !is_valid_val(val)) {
		out_of_range();
		return ERROR;
	}
	else if (start > end) {
		start_bigger_than_end();
		return ERROR;
	}
	int i;
	for(i = start; i <= end; i++) {
		mem[i] = val;
	}
	return SUCCESS;
}

//reset all memory to zero
void reset(){
	int i;
	for(i = 0; i <= MAX_ADDR; i++) mem[i] = 0;
	return;
}
