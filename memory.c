#include "memory.h"

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

	printf("dump\n");	
}

//edit memory in idx to val, if not valid address or val, return error
int edit(int idx, int val) {

	printf("edit\n");	
}

//fill memory from start to end with val, if not valid argument/address or val, return error 
int fill(int start, int end, int val) {

	printf("fill\n");	
}

//reset all memory to zero
void reset(){

	printf("reset\n");	
}
