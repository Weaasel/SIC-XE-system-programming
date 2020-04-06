#include <stdio.h>
#include <stdbool.h>
#define ERROR -1
#define EMPTY -1
#define SUCCESS 1
#define IMPOSSIBLE 2000000000

//declaration for boolean type
//extern typedef enum {false, true} bool;

//error functions
void undefined_command();
void undefined_argument();
void out_of_range();
void start_bigger_than_end();
