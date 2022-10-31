#include <stdbool.h>
#include "lexeme.h"

typedef struct {
    lexeme token;
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
