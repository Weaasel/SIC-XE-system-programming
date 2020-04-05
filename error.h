#include <stdio.h>
#define ERROR -1
#define SUCCESS 1

//declaration for boolean type
typedef enum {false, true} bool;

//error functions
void undefined_command();
void undefined_argument();
void out_of_range();
