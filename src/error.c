/**
 * Project - IFJ Projekt 2022
 *
 * @author Lukas Vecerka xvecer30
 * @author Jachym Dolezal xdolez0c
 * @author Andrej Nespor xnespo10
 * @author Matej Tomko xtomko06
 *
 * @brief Function implementation for exiting program with specified code
 */

#include "error.h"

void exit_program(int err_code, char* msg) {
    switch (err_code) {
        case 1:
            // Error in lexical analysis (wrong structure of actual lexeme)
            fprintf(stderr, "lexical error: %s\n", msg);
            exit(1);
        case 2:
            // Error in syntactic analysis, wrong program syntax, etc.
            fprintf(stderr, "syntax error: %s\n", msg);
            exit(2);
        case 3:
            // Undefined function or function redefinition
            fprintf(stderr, "semantic error: %s\n", msg);
            exit(3);
        case 4:
            // Wrong parameter count in function call, wrong return value of
            // function
            fprintf(stderr, "semantic error: %s\n", msg);
            exit(4);
        case 5:
            // Using of undefined variable
            fprintf(stderr, "semantic error: %s\n", msg);
            exit(5);
        case 6:
            // Missing/Unexpected expression in function return
            fprintf(stderr, "semantic error: %s\n", msg);
            exit(6);
        case 7:
            // Wrong type compatibility in arithmetic, string or relation
            // expression
            fprintf(stderr, "semantic error: %s\n", msg);
            exit(7);
        case 8:
            // Others semantic errors
            fprintf(stderr, "semantic error: %s\n", msg);
            exit(8);
        case 99:
            // Internal error of compiler
            fprintf(stderr, "internal error: %s\n", msg);
            exit(99);
    }
}
/*END OF FILE*/
