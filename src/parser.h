#include <stdbool.h>
// #include "lexeme.h"
#include "symtable.h"

typedef struct {
    lexeme token;
    htab_t* global_symtable;
    htab_t* local_symtable;
    htab_item_data_t* global_symtable_data;
    htab_item_data_t* local_symtable_data;
    bool in_function;
    bool func_check;
    int scope;
    int param_counter;
} Parser;

void get_next_token();
void get_token_consume_token(lex token_type, char* err_msg);
void consume_token(lex token_type, char* err_msg);

bool check_param_types();
bool check_return_type();
bool check_data_type(lex token_type);

// funtions for rule checking
bool type();
bool program();
bool list_params();
bool syntax_analyse();
bool next_parameter();
bool list_input_params();
bool next_input_parameter();
bool term();
bool return_type();
bool statement();
// void prolog();
