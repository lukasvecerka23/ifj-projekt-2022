#include <stdbool.h>
#include "lexeme.h"

typedef struct {
    lexeme token;
} Parser;

void get_next_token();
void get_token_check_token_type(lex token_type);
bool check_param_types();
bool check_param_type(lex token_type);

// funtions for rule checking
bool type();
bool program();
bool list_params();
// void prolog();