
#include "lexeme.h"

int main() {
    lexeme l = {0};
    while (l.lex != LEOF) {
        l = get_lex_value();
        print_lex(l);
    }
    return 0;
}
