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
            fprintf(stderr, "lexical error: %s\n", msg);
            exit(1);
        case 2:
            fprintf(stderr, "syntax error: %s\n", msg);
            exit(2);
        case 3:
            fprintf(stderr, "semantic error: %s\n", msg);
            exit(3);
        case 4:
            fprintf(stderr, "semantic error: %s\n", msg);
            exit(4);
        case 5:
            fprintf(stderr, "semantic error: %s\n", msg);
            exit(5);
        case 6:
            fprintf(stderr, "semantic error: %s\n", msg);
            exit(6);
        case 7:
            fprintf(stderr, "semantic error: %s\n", msg);
            exit(7);
        case 8:
            fprintf(stderr, "semantic error: %s\n", msg);
            exit(8);
        case 99:
            fprintf(stderr, "internal error: %s\n", msg);
            exit(99);
    }
}
/*END OF FILE*/
