/*
Name: IFJ PROJEKT 2022
Authors: xdolez0c, xvecer30, xnespo10, xtomko06
Description: --
*/

#include "error.h"
// #include "exp_parser.h"
#include "parser.h"
#include "scanner.h"

int main() {
    // token_t l = {0};
    // while (l.token_type != LEOF) {
    //     l = get_lex_value();
    //     print_lex(l);
    // }

    syntax_analyse();

    return 0;
}
