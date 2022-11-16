#include "lexeme.c"
#include "lexeme.h"

typedef struct {
    lexeme token;
    // htab_t* global_symtable;
    // htab_t* local_symtable;
    // htab_item_data_t* global_symtable_data;
    // htab_item_data_t* local_symtable_data;
    bool in_function;
} Exp_parser;