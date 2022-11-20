#include "code_gen.h"
#include <stdio.h>

/** TODO
 * Functions for generate functino calls
 * Checking params in function declaration
 * Generating builtin functions
 * Generating expression
*/
void generate_header(){
    printf(".IFcode22\n");
    printf("JUMP $$main\n");
    printf("LABEL $$main\n");
}

void generate_end(){
    printf("# end of main\n");
}

void generate_func_header(char* func_id){
    printf("LABEL $$%s\n", func_id);
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@retval1\n");
    printf("MOVE LF@retval1 nil@nil\n");
}

void generate_func_end(){
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void generate_func_param(char* param_name, unsigned long long param_number){
    printf("DEFVAR LF@%s\n", param_name);
    printf("MOVE LF@%s, LF@%llu\n", param_name, param_number);
}