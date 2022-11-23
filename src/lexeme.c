#include "lexeme.h"
#include <string.h>
#include "error.h"

/*


*/

// TODO: dynamically allocated
char string[2000] = {0};
char* string_start = &string[0];
int err_flag = 0;
/*
// TODO:
- add all transitions
- implement prolog and function for escape sequences
*/

int return_digit(char* token) {
    return atoi(token);
}

double return_float(char* token) {
    return atof(token);
}

char* escape_sequence_parser(char* str) {
    unsigned long long i = 0;
    char* tmp = malloc(sizeof(char) * (strlen(str) + 1));
    unsigned long long j = 0;
    for (i, j = 0; str[i] != '\0'; i++, j++) {
        if (str[i] == '$') {  // error
            return NULL;
        }
        if (str[i] == '\\') {
            i++;
            if (str[i] == '$') {
                tmp[j] = '$';
                // printf("hit\n");
                continue;
            }
            if (str[i] == 'n') {
                tmp[j] = '\n';
                continue;
            }
            if (str[i] == 't') {
                tmp[j] = '\t';
                continue;
            }
            if (str[i] == 'r') {
                tmp[j] = '\r';
                continue;
            }
            if (str[i] == '\\') {
                tmp[j] = '\\';
                continue;
            }
            if (str[i] == '"') {
                tmp[j] = '\"';
                continue;
            }
            if (str[i] == 'x') {
                char hex_num[3];
                int k;
                for (k = 0; k < 2; k++) {
                    hex_num[k] =
                        str[(i + k + 1)];  // load of hex number to buffer
                }
                char* err_str;
                int num = (int)strtol(hex_num, &err_str, 16);  // hex -> dec
                hex_num[k++] = '\0';
                if (strlen(err_str) >=
                    1) {  // check if if valid hex else skip parsing hex.
                    tmp[j] = str[i];
                    // printf("err\n");
                    continue;
                } else {
                    char buffer[3];
                    sprintf(buffer, "%d", num);  // convert dec num to str form.
                    for (int l = 0; buffer[l] != '\0'; l++) {
                        tmp[j++] = buffer[l];  // load dec number to tmp
                    }
                }
                i = i + 3;  // skip original chars
            }

            if (isdigit(str[i])) {
                // printf("hit\n");
                char dec_num[4];
                int k;
                for (k = 0; k <= 2; k++) {
                    dec_num[k] = str[i + k];
                    // printf("loading...\n");
                }
                dec_num[3] = '\0';
                // printf("dec_num, %s :\n", dec_num);
                char* err_str_dec;
                int num = (int)strtol(dec_num, &err_str_dec, 8);
                // printf("num: %d\n", num);
                dec_num[k++] = '\0';
                if (strlen(err_str_dec) >= 1) {
                    tmp[j] = str[i];
                    continue;
                } else {
                    char buffer_dec[4];
                    sprintf(buffer_dec, "%d", num);
                    for (int l = 0; buffer_dec[l] != '\0'; l++) {
                        tmp[j++] = buffer_dec[l];
                        // printf("c: %c\n", tmp[j]);
                    }
                }
                i = i + 3;
                // printf("str: %c\n", str[i]);
                // tmp[j] = str[i];
                // continue;
            }
            tmp[j] = str[i];

        } else {
            tmp[j] = str[i];
        }
    }
    tmp[j++] = '\0';

    free(str);
    return tmp;
}

lexeme isKeyword(char* keywd) {
    if (strcmp(keywd, "else") == 0)
        return (lexeme){.lex = K_ELSE};
    if (strcmp(keywd, "float") == 0)
        return (lexeme){.lex = K_FLOAT};
    if (strcmp(keywd, "if") == 0)
        return (lexeme){.lex = K_IF};
    if (strcmp(keywd, "int") == 0)
        return (lexeme){.lex = K_INT};
    if (strcmp(keywd, "null") == 0)
        return (lexeme){.lex = K_NULL};
    if (strcmp(keywd, "return") == 0)
        return (lexeme){.lex = K_RETURN};
    if (strcmp(keywd, "void") == 0)
        return (lexeme){.lex = K_VOID};
    if (strcmp(keywd, "while") == 0)
        return (lexeme){.lex = K_WHILE};
    if (strcmp(keywd, "string") == 0)
        return (lexeme){.lex = K_STRING};
    if (strcmp(keywd, "function") == 0)
        return (lexeme){.lex = K_FUNCTION};
    if (strcmp(keywd, "declare") == 0)
        return (lexeme){.lex = K_DECLARE};
    if (strcmp(keywd, "strict_types") == 0)
        return (lexeme){.lex = K_STRICTTYPES};
    return (lexeme){.lex = L_FUNCID, .string = keywd};
}

States FSM(States curr_state, char edge) {
    switch (curr_state) {
        case TOKEN_END:
            fprintf(stderr, "token should have been created.");
        case START:
            if (edge == ')')
                return RPAR;
            if (edge == '(')
                return LPAR;
            if (edge == ',')
                return COMMA;
            if (edge == '{')
                return LCURL;
            if (edge == '}')
                return RCURL;
            if (edge == ':')
                return COLON;
            if (edge == ';')
                return SEMICOLON;
            if (edge == '.')
                return DOT;
            if (edge == '+')
                return PLUS;
            if (edge == '-')
                return DASH;
            if (edge == '/')
                return SLASH;
            if (edge == '$')
                return VARID;
            if (edge == '?')
                return PHPEND;
            if (edge == '*')
                return MUL;
            if (edge == '<')
                return PHPSTART;
            if (edge == '>')
                return GREATER;
            if (edge == '"')
                return STRING_LIT_E;
            if (isalpha(edge) || edge == '_')
                return ID1;
            if (isspace(edge))
                return START;
            if (isdigit(edge))
                return NUMBER;
            if (edge == '=')
                return EQ1;
            if (edge == '!')
                return NEQ1;
            return TOKEN_END;
        case STRING_LIT_END:
            return TOKEN_END;
        case PHPSTART:
            if (edge == '?') {
                return PHPSTART2;
            } else
                return LESS;
        case PHPSTART2:
            if (edge == 'p') {
                return PHPSTART3;
            } else {
                err_flag = 1;
                return TOKEN_END;
            }
        case PHPSTART3:
            if (edge == 'h') {
                return PHPSTART4;
            } else {
                err_flag = 1;
                return TOKEN_END;
            }
        case PHPSTART4:
            if (edge == 'p') {
                return PHPSTART5;
            } else {
                err_flag = 1;
                return TOKEN_END;
            }
        case PHPSTART5:
            if (isspace(edge)) {
                return TOKEN_END;
            }
            return err_flag = 1;
            return TOKEN_END;
        case PHPEND:
            if (edge == '>')
                return PHPEND2;
            else
                return TOKEN_END;
        case PHPEND2:
            return TOKEN_END;
        // case DECLARE:
        case STRING_LIT_E:
            if (edge == '"') {
                return STRING_LIT_END;
            }
            if (edge == '\\') {
                return STRING_SLASH;
            }
            if (edge == EOF) {
                err_flag = 1;
                return TOKEN_END;
            }
            return STRING_LIT_E;
        case STRING_SLASH:
            if (edge == '"')
                return STRING_LIT_E;
            return STRING_LIT_E;
        case SLASH:
            if (edge == '/')
                return ONE_L_COMMENT;
            if (edge == '*')
                return MULT_L_COMMENT;
            return TOKEN_END;
        case MULT_L_COMMENT:
            if (edge == EOF) {
                err_flag = 1;
                return TOKEN_END;
            }
            if (edge == '*')
                return STAR_END;
            else
                return MULT_L_COMMENT;
        case STAR_END:
            if (edge == '/')
                return START;
            else {
                return MULT_L_COMMENT;
            }
        case ONE_L_COMMENT:
            if (edge == '\n' || edge == EOF)
                return START;
            else
                return ONE_L_COMMENT;
        case NUMBER:
            if (isdigit(edge))
                return NUMBER;
            if (edge == '.')
                return FLOAT1;
            if (edge == 'e' || edge == 'E')
                return EXP_1;
            else
                return TOKEN_END;
        case FLOAT1:
            if (isdigit(edge))
                return FLOAT2;
            else {
                return err_flag = 1;
                return TOKEN_END;
            }
        case FLOAT2:
            if (isdigit(edge))
                return FLOAT2;
            if (edge == 'E' || edge == 'e')
                return EXP_1;
            else
                return TOKEN_END;
        case EXP_1:
            if (edge == '+' || edge == '-')
                return EXP_1_5;
            if (isdigit(edge))  // add one state to fix 10E+ is valid
                return EXP_2;
            else {
                err_flag = 1;
                return TOKEN_END;
            }
        case EXP_1_5:
            if (isdigit(edge))
                return EXP_2;
            else
                err_flag = 1;
            return TOKEN_END;
        case EXP_2:
            if (isdigit(edge))
                return EXP_2;
            else
                return TOKEN_END;
        case VARID:
            if (isalnum(edge) || edge == '_')
                return VARID;
            return TOKEN_END;
        case EQ1:
            if (edge == '=')
                return EQ2;
            return TOKEN_END;
        case EQ2:
            if (edge == '=')
                return EQ3;
            err_flag = 1;
            return TOKEN_END;
        case NEQ1:
            if (edge == '=')
                return NEQ2;
            err_flag = 1;
            return TOKEN_END;
        case NEQ2:
            if (edge == '=')
                return NEQ3;
            err_flag = 1;
            return TOKEN_END;
        case LESS:
            if (edge == '=')
                return LESSEQ;
        case ID1:
            if (isalpha(edge) || edge == '_')
                return ID2;
            return TOKEN_END;
        case ID2:
            if (isalnum(edge) || edge == '_')
                return ID2;
            return TOKEN_END;
        case GREATER:
            if (edge == '=')
                return GREATEREQ;
            return TOKEN_END;
        default:
            return TOKEN_END;
    }
    return TOKEN_END;
}
/*
TODO:
- store data to lexeme depends on its type
- uploading var id, func id to symtable
- sending tokens to syntax analyzer
*/
lexeme create_lex(States final, char* token) {
    switch (final) {
        case PHPEND2:
            return (lexeme){.lex = L_PHPEND};
        case LPAR:
            return (lexeme){.lex = L_LPAR};
        case PHPSTART5:
            return (lexeme){.lex = L_PHPSTART};
        case RPAR:
            return (lexeme){.lex = L_RPAR};
        case COMMA:
            return (lexeme){.lex = L_COMMA};
        case DOT:
            return (lexeme){.lex = L_DOT};
        case LCURL:
            return (lexeme){.lex = L_LCURL};
        case RCURL:
            return (lexeme){.lex = L_RCURL};
        case SEMICOLON:
            return (lexeme){.lex = L_SEMICOL};
        case COLON:
            return (lexeme){.lex = L_COLON};
        case MUL:
            return (lexeme){.lex = L_MUL};
        case SLASH:
            return (lexeme){.lex = L_SLASH};
        case PLUS:
            return (lexeme){.lex = L_PLUS};
        case DASH:
            return (lexeme){.lex = L_DASH};
        case STRING_LIT_END:
            return (lexeme){.lex = L_STRING,
                            .string = escape_sequence_parser(token)};
        case EXP_2:
            return (lexeme){.lex = L_EXP, .float_val = return_float(token)};
            // return (lexeme){.lex = L_EXP, .string = token};
        case ID2:
            // call function for decision between funcid, keyword or type id
            return isKeyword(token);
        case VARID:
            return (lexeme){.lex = L_VARID, .string = token};
        case NUMBER:
            return (lexeme){.lex = L_NUMBER, .val = return_digit(token)};
        case EQ1:
            return (lexeme){.lex = L_ASSIGN};
        case EQ3:
            return (lexeme){.lex = L_EQ};
        case NEQ3:
            return (lexeme){.lex = L_NEQ};
        case LESSEQ:
            return (lexeme){.lex = L_LESSEQ};
        case GREATEREQ:
            return (lexeme){.lex = L_GREATEREQ};
        case LESS:
            return (lexeme){.lex = L_LESS};
        case GREATER:
            return (lexeme){.lex = L_GREATER};
        case FLOAT2:
            return (lexeme){.lex = L_FLOAT, .float_val = return_float(token)};
        case PHPEND:
            return (lexeme){.lex = L_VARPREF};
        case TOKEN_END:
            error_exit("reached end of token");
        default:
            warning_msg("No state implemented for this input");
    }
    warning_msg("No state implemented for this input");
}
/*
TODO:
- string handling
- dynamical allocation
*/
// lexeme get_lex_value() {
//     States now = START;
//     char* lex_start = string_start;
//     char edge = ' ';
//     while (true) {
//         if (edge == '\n')
//             line_num++;
//         edge = getchar();
//         if (edge == EOF) {
//             if (now == START) {
//                 return (lexeme){.lex = LEOF};
//             }
//             printf("hit\n");
//             return create_lex(now, lex_start);
//         }
//         States next = FSM(now, edge);
//         if (next == TOKEN_END) {
//             ungetc(edge, stdin);
//             *(string_start++) = '\0';
//             if (!err_flag) {
//                 return create_lex(now, lex_start);
//             } else {
//                 warning_msg("line: %d token: %s", line_num, lex_start);
//                 err_flag = 0;
//                 next = START;
//             }
//         }
//         *(string_start++) = edge;
// if (next == START) {
//     string_start = lex_start;
// }
//         now = next;
//     }
// }

lexeme get_token_data(scanner_t scan) {
    // tmp, move to main and pass as argument
    States now = START;
    scan.tokenmem = 100;
    scan.usedmem = 0;
    scan.token =
        (char*)calloc(scan.tokenmem, sizeof(char*));  // tmp, create fnc
    char edge = ' ';
    int idx = 0;
    while (true) {
        if (edge == '\n')
            scan.line_num++;
        edge = getchar();
        if (edge == EOF) {
            if (now == START) {
                return (lexeme){.lex = LEOF};
            }
            return create_lex(now, scan.token);
        }
        States next = FSM(now, edge);
        if (next == TOKEN_END) {
            ungetc(edge, stdin);
            scan.token[idx++] = '\0';
            if (!err_flag) {
                // printf("token %s\n", token);
                // printf("createlex param: %s\n", scan.token);
                return create_lex(now, scan.token);

            } else {
                error_exit("line: %d token: %s", scan.line_num, scan.token);
                err_flag = 0;
                next = START;
            }
        }
        scan.token[idx] = edge;
        idx++;
        scan.usedmem++;
        if (scan.usedmem >= (scan.tokenmem * 0.9)) {
            printf("hit\n");
            scan.tokenmem = scan.tokenmem * 2;
            scan.token = (char*)realloc(scan.token, scan.tokenmem);
            scan.usedmem = 0;
        }

        if (next == START) {
            idx = 0;
        }
        now = next;
    }
}

void token_free(lexeme* token) {
    free(token);
}

lexeme get_lex_value(scanner_t scan) {
    lexeme* token = malloc(sizeof(lexeme));

    *token = get_token_data(scan);

    // if(token->lex != L_STRING || token->lex != L_VARID){
    //     free(scan.token);
    // }

    return *token;
}