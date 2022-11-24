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
