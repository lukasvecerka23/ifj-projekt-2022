/**
 * Project - IFJ Projekt 2022
 *
 * @author Lukas Vecerka xvecer30
 * @author Jachym Dolezal xdolez0c
 * @author Andrej Nespor xnespo10
 * @author Matej Tomko xtomko06
 *
 * @brief Scanner which utilizes implementation of FSM to parse and return
 * tokens
 */

#include "scanner.h"
#include <string.h>
#include "error.h"

unsigned long char_cnt = 0;  // counter
int err_flag = 0;            // global variable for error_flag

void token_free(char* token) {
    if (token != NULL) {
        free(token);
    }
}

int return_digit(char* token) {
    int i = atoi(token);
    return i;
}

double return_float(char* token) {
    double i = atof(token);
    return i;
}

char* escape_sequence_parser(char* str) {
    unsigned int i = 0;
    unsigned int j = 0;
    char* tmp = malloc(sizeof(char) * (strlen(str) * 2));
    for (i, j = 0; str[i] != '\0'; i++, j++) {
        if (str[i] == '$') {
            exit_program(1,
                         "Error when parsing string, $ must be written with \\ "
                         "try -> \\$");
        }
        if (str[i] == '\\') {
            i++;  // ignores '\'
            if (str[i] == '$') {
                tmp[j] = '$';
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
                for (k = 0; k < 2; k++) {  // stores digits of hex number
                    hex_num[k] = str[(i + k + 1)];
                }
                char* err_str;
                int num = (int)strtol(hex_num, &err_str, 16);  // hex -> dec
                hex_num[k++] = '\0';
                /* checks if err_str contains any characters, in that case
                original number was not correct hex number */
                if (strlen(err_str) >= 1) {
                    tmp[j] = str[i];
                    continue;
                } else {
                    char buffer[3];
                    sprintf(buffer, "%d", num);  // convert dec num to str form.
                    tmp[j++] = '\\';
                    tmp[j++] = '0';
                    // stores the result in decadic form in the tmp string
                    for (int l = 0; buffer[l] != '\0'; l++, j++) {
                        tmp[j] = buffer[l];
                    }
                }
                j--;        // for index correction
                i = i + 2;  // skip original chars
                continue;
            }

            if (isdigit(str[i])) {
                char dec_num[4];
                int k;
                for (k = 0; k <= 2; k++) {  // stores digits of octal number
                    dec_num[k] = str[i + k];
                }
                dec_num[3] = '\0';
                char* err_str_dec;
                int num = (int)strtol(dec_num, &err_str_dec,
                                      8);  // oct -> dec conversion
                /* if err_str_dec is not empty (excluding \0 char) it means the
                 * original data was not a correct octal number*/
                if (strlen(err_str_dec) >= 1) {
                    tmp[j] = str[i];
                    continue;
                } else {
                    char buffer_dec[4];
                    // convert dec num to str form.
                    sprintf(buffer_dec, "%d", num);
                    tmp[j++] = '\\';
                    tmp[j++] = '0';
                    // stores the result in decadic form in the tmp string
                    for (int l = 0; buffer_dec[l] != '\0'; l++) {
                        tmp[j++] = buffer_dec[l];
                    }
                }
                j--;
                i = i + 2;
                continue;
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
            if (isspace(edge) || edge == '/') {
                if (char_cnt == 6)
                    return TOKEN_END;
                else {
                    err_flag = 1;
                    return TOKEN_END;
                }
            }
            err_flag = 1;
            return TOKEN_END;
        case PHPEND:
            if (edge == '>')
                return PHPEND2;
            else
                return TOKEN_END;
        case PHPEND2:
            if (edge == EOF) {
                return TOKEN_END;
            }
            err_flag = 1;
            return TOKEN_END;
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
            if (isdigit(edge))
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
            if (isalnum(edge) || edge == '_')
                return ID1;
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

token_t create_lex(States final, char* token) {
    token_t tmp_token = {0};
    switch (final) {
        case PHPEND2:
            tmp_token = (token_t){.token_type = L_PHPEND};
            break;
        case LPAR:
            tmp_token = (token_t){.token_type = L_LPAR};
            break;
        case PHPSTART5:
            tmp_token = (token_t){.token_type = L_PHPSTART};
            break;
        case RPAR:
            tmp_token = (token_t){.token_type = L_RPAR};
            break;
        case COMMA:
            tmp_token = (token_t){.token_type = L_COMMA};
            break;
        case DOT:
            tmp_token = (token_t){.token_type = L_DOT};
            break;
        case LCURL:
            tmp_token = (token_t){.token_type = L_LCURL};
            break;
        case RCURL:
            tmp_token = (token_t){.token_type = L_RCURL};
            break;
        case SEMICOLON:
            tmp_token = (token_t){.token_type = L_SEMICOL};
            break;
        case COLON:
            tmp_token = (token_t){.token_type = L_COLON};
            break;
        case MUL:
            tmp_token = (token_t){.token_type = L_MUL};
            break;
        case SLASH:
            tmp_token = (token_t){.token_type = L_SLASH};
            break;
        case PLUS:
            tmp_token = (token_t){.token_type = L_PLUS};
            break;
        case DASH:
            tmp_token = (token_t){.token_type = L_DASH};
            break;
        case STRING_LIT_END:
            tmp_token = (token_t){.token_type = L_STRING,
                                  .string = escape_sequence_parser(token)};
            break;
        case EXP_2:
            tmp_token = (token_t){.token_type = L_EXP,
                                  .float_val = return_float(token)};
            break;
        case ID1:
            // call function for decision between funcid, keyword or type id
            tmp_token = isKeyword(token);
            break;
        case VARID:
            tmp_token = (token_t){.token_type = L_VARID, .string = token};
            break;
        case NUMBER:
            tmp_token =
                (token_t){.token_type = L_NUMBER, .val = return_digit(token)};
            break;
        case EQ1:
            tmp_token = (token_t){.token_type = L_ASSIGN};
            break;
        case EQ3:
            tmp_token = (token_t){.token_type = L_EQ};
            break;
        case NEQ3:
            tmp_token = (token_t){.token_type = L_NEQ};
            break;
        case LESSEQ:
            tmp_token = (token_t){.token_type = L_LESSEQ};
            break;
        case GREATEREQ:
            tmp_token = (token_t){.token_type = L_GREATEREQ};
            break;
        case PHPSTART:
            tmp_token = (token_t){.token_type = L_LESS};
            break;
        case GREATER:
            tmp_token = (token_t){.token_type = L_GREATER};
            break;
        case FLOAT2:
            tmp_token = (token_t){.token_type = L_FLOAT,
                                  .float_val = return_float(token)};
            break;
        case PHPEND:
            tmp_token = (token_t){.token_type = L_VARPREF};
            break;
        default:
            free(token);
            exit_program(1, "undefined lexeme");
            break;
    }
    return tmp_token;
}

token_t get_token_data(scanner_t* scan) {
    States now = START;
    scan->tokenmem = 5;
    scan->usedmem = 0;
    scan->token = (char*)calloc(scan->tokenmem, sizeof(char*));
    char edge = ' ';
    int idx = 0;
    while (true) {
        if (edge == '\n')
            scan->line_num++;
        edge = getchar();
        char_cnt++;
        if (edge == EOF) {
            if (now == START) {
                return (token_t){.token_type = LEOF};
            }
        }
        States next = FSM(now, edge);
        if (next == TOKEN_END) {
            ungetc(edge, stdin);
            scan->token[idx++] = '\0';
            if (!err_flag) {
                return create_lex(now, scan->token);

            } else {
                token_free(scan->token);
                exit_program(1, "wrong token");
                err_flag = 0;
                next = START;
            }
        }
        scan->token[idx] = edge;
        idx++;
        scan->usedmem++;
        if (scan->usedmem >= (scan->tokenmem * 0.9)) {
            scan->tokenmem = scan->tokenmem * 2;
            scan->token = (char*)realloc(scan->token, scan->tokenmem);
            if (scan->token == NULL) {
                fprintf(stderr,
                        "realloc memory allocation fail (I am in scanner if "
                        "you want to delete me\n");
            }
        }

        if (next == START) {
            idx = 0;
        }
        now = next;
    }
}

token_t* get_lex_value() {
    scanner_t* scan = (scanner_t*)malloc(sizeof(scanner_t));
    token_t* token = malloc(sizeof(token_t));

    *token = get_token_data(scan);

    if (scan != NULL) {
        free(scan);
    }

    return token;
}

/*END OF FILE*/
