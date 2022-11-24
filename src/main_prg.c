#include "error.h"
// #include "exp_parser.h"
// #include "parser.h"
#include "lexeme.h"

int main() {
    lexeme l = {0};
    while (l.lex != LEOF) {
        l = get_lex_value();
        print_lex(l);
    }

    // if (syntax_analyse()) {
    //     printf("parser good");
    // } else {
    //     printf("parser not good");
    // }
    return 0;
}
