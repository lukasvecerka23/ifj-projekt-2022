#include "scanner.h"
#include <string.h>
#include "error.h"

/*
just for testing
*/
int err_flag = 0;

void print_lex(token_t token) {
    switch (token.token_type) {
        case L_LPAR:
            printf("( ( )\n");
            return;
        case L_RPAR:
            printf("( ) )\n");
            return;
        case L_COMMA:
            printf("( , )\n");
            return;
        case L_DOT:
            printf("( . )\n");
            return;
        case L_LCURL:
            printf("( { )\n");
            return;
        case L_RCURL:
            printf("( } )\n");
            return;
        case L_SEMICOL:
            printf("( ; )\n");
            return;
        case L_COLON:
            printf("( : )\n");
            return;
        case LEOF:
            printf("( EOF )\n");
            return;
        case L_PLUS:
            printf("( + )");
            return;
        case L_MUL:
            printf("( * )\n");
            return;
        case L_SLASH:
            printf("( / )\n");
            return;
        case L_EXP:
            printf("( exp, %f)\n", token.float_val);
            // printf("( exp, %s)\n", lex.string);
            return;
        case L_NUMBER:
            printf("(integer, %lld)\n", token.val);
            return;
        case L_VARID:
            printf("(varid, %s)\n", token.string);
            return;
        case L_VARPREF:
            printf("( ? )\n");
            return;
        case L_ID:
            printf("(identifier, %s)\n", token.string);
            return;
        case L_STRING:
            printf("(string, %s)\n", token.string);
            return;
        case L_DASH:
            printf("( - )\n");
            return;
        case L_ASSIGN:
            printf("( = )\n");
            return;
        case L_EQ:
            printf("( === )\n");
            return;
        case L_NEQ:
            printf("( !== )\n");
            return;
        case L_LESS:
            printf("( < )\n");
            return;
        case L_GREATER:
            printf("( > )\n");
            return;
        case L_LESSEQ:
            printf("( <= )\n");
            return;
        case L_GREATEREQ:
            printf("( >= )\n");
            return;
        case L_FLOAT:
            printf("(float, %f)\n", token.float_val);
            return;
        case K_ELSE:
            printf("( else )\n");
            return;
        case K_FUNCTION:
            printf("( function )\n");
            return;
        case K_IF:
            printf("( if )\n");
            return;
        case K_INT:
            printf("( int )");
            return;
        case K_NULL:
            printf("( null )\n");
            return;
        case K_RETURN:
            printf("( return )\n");
            return;
        case K_STRING:
            printf("( string )\n");
            return;
        case K_VOID:
            printf("( void )\n");
            return;
        case K_WHILE:
            printf("( while )\n");
            return;
        case K_FLOAT:
            printf("( float )\n");
            return;
        case L_FUNCID:
            printf("( funcid, %s )\n", token.string);
            return;
        case L_PHPEND:
            printf("( php end )\n");
            return;
        case L_PHPSTART:
            printf("( php start )\n");
            return;
        case K_STRICTTYPES:
            printf("( strict_types )\n");
            return;
        case K_DECLARE:
            printf("( declare )\n");
            return;
        default:
            warning_msg("did not match any token \n");
            return;
    }
    warning_msg(
        "token should have been printed (didnt you forget to add it print "
        "func)");
    return;
}

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

token_t isKeyword(char* keywd) {
    if (strcmp(keywd, "else") == 0)
        return (token_t){.token_type = K_ELSE};
    if (strcmp(keywd, "float") == 0)
        return (token_t){.token_type = K_FLOAT};
    if (strcmp(keywd, "if") == 0)
        return (token_t){.token_type = K_IF};
    if (strcmp(keywd, "int") == 0)
        return (token_t){.token_type = K_INT};
    if (strcmp(keywd, "null") == 0)
        return (token_t){.token_type = K_NULL};
    if (strcmp(keywd, "return") == 0)
        return (token_t){.token_type = K_RETURN};
    if (strcmp(keywd, "void") == 0)
        return (token_t){.token_type = K_VOID};
    if (strcmp(keywd, "while") == 0)
        return (token_t){.token_type = K_WHILE};
    if (strcmp(keywd, "string") == 0)
        return (token_t){.token_type = K_STRING};
    if (strcmp(keywd, "function") == 0)
        return (token_t){.token_type = K_FUNCTION};
    if (strcmp(keywd, "declare") == 0)
        return (token_t){.token_type = K_DECLARE};
    if (strcmp(keywd, "strict_types") == 0)
        return (token_t){.token_type = K_STRICTTYPES};
    return (token_t){.token_type = L_FUNCID, .string = keywd};
}

States FSM(States curr_state, char edge) {
    switch (curr_state) {
        case TOKEN_END:
            fprintf(stderr, "token should have been created.");
            break;
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
            }
            if (edge == '=') {
                return LESSEQ;
            }
            return TOKEN_END;
        // case LESS:
        //     return TOKEN_END;
        // case LESSEQ:
        //     return TOKEN_END;
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
        case ID1:
            if (isalpha(edge) || edge == '_')
                return ID1;
            return TOKEN_END;
        // case ID2:
        //     if (isalnum(edge) || edge == '_')
        //         return ID2;
        //     return TOKEN_END;
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
token_t create_lex(States final, char* token) {
    switch (final) {
        case PHPEND2:
            return (token_t){.token_type = L_PHPEND};
        case LPAR:
            return (token_t){.token_type = L_LPAR};
        case PHPSTART5:
            return (token_t){.token_type = L_PHPSTART};
        case RPAR:
            return (token_t){.token_type = L_RPAR};
        case COMMA:
            return (token_t){.token_type = L_COMMA};
        case DOT:
            return (token_t){.token_type = L_DOT};
        case LCURL:
            return (token_t){.token_type = L_LCURL};
        case RCURL:
            return (token_t){.token_type = L_RCURL};
        case SEMICOLON:
            return (token_t){.token_type = L_SEMICOL};
        case COLON:
            return (token_t){.token_type = L_COLON};
        case MUL:
            return (token_t){.token_type = L_MUL};
        case SLASH:
            return (token_t){.token_type = L_SLASH};
        case PLUS:
            return (token_t){.token_type = L_PLUS};
        case DASH:
            return (token_t){.token_type = L_DASH};
        case STRING_LIT_END:
            return (token_t){.token_type = L_STRING,
                             .string = escape_sequence_parser(token)};
        case EXP_2:
            return (token_t){.token_type = L_EXP,
                             .float_val = return_float(token)};
        case ID1:
            // call function for decision between funcid, keyword or type id
            return isKeyword(token);
        case VARID:
            return (token_t){.token_type = L_VARID, .string = token};
        case NUMBER:
            return (token_t){.token_type = L_NUMBER,
                             .val = return_digit(token)};
        case EQ1:
            return (token_t){.token_type = L_ASSIGN};
        case EQ3:
            return (token_t){.token_type = L_EQ};
        case NEQ3:
            return (token_t){.token_type = L_NEQ};
        case LESSEQ:
            return (token_t){.token_type = L_LESSEQ};
        case GREATEREQ:
            return (token_t){.token_type = L_GREATEREQ};
        case PHPSTART:
            return (token_t){.token_type = L_LESS};
        case GREATER:
            return (token_t){.token_type = L_GREATER};
        case FLOAT2:
            return (token_t){.token_type = L_FLOAT,
                             .float_val = return_float(token)};
        case PHPEND:
            return (token_t){.token_type = L_VARPREF};
        case TOKEN_END:
            exit_program(1, "wrong token");
            break;
            // error_exit("reached end of token");
        default:
            exit_program(1, "undefined lexeme");
            break;
    }
    return (token_t){0};
}

token_t get_token_data(scanner_t scan) {
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
                return (token_t){.token_type = LEOF};
            }
            // return create_lex(now, scan.token);
        }
        States next = FSM(now, edge);
        if (next == TOKEN_END) {
            ungetc(edge, stdin);
            scan.token[idx++] = '\0';
            if (!err_flag) {
                return create_lex(now, scan.token);

            } else {
                exit_program(1, "wrong token");
                err_flag = 0;
                next = START;
            }
        }
        scan.token[idx] = edge;
        idx++;
        scan.usedmem++;
        if (scan.usedmem >= (scan.tokenmem * 0.9)) {
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

void token_free(token_t* token) {
    free(token);
}

token_t get_lex_value(scanner_t scan) {
    token_t* token = malloc(sizeof(token_t));

    *token = get_token_data(scan);

    return *token;
}
