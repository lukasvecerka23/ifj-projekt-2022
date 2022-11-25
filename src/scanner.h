/*
Name: IFJ PROJEKT 2022
Authors: xdolez0c, xvecer30, xnespo10, xtomko06
Description: --
*/

#ifndef IFJ_SCANNER_H
#define IFJ_SCANNER_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum States {
    START,
    LPAR,
    RPAR,
    COMMA,
    DOT,
    LCURL,
    RCURL,
    COLON,
    SEMICOLON,
    TOKEN_END,
    STRING_LIT_S,
    STRING_LIT_E,
    STRING_LIT_END,
    STRING_SLASH,
    PHPSTART,
    PHPSTART2,
    PHPSTART3,
    PHPSTART4,
    PHPEND,
    PHPEND2,
    DECLARE,
    PLUS,
    DASH,
    SLASH,
    MUL,
    EXP_1_5,
    ID1,
    ID2,
    VARID,
    NUMBER,
    EQ1,
    EQ2,
    EQ3,
    LESS,
    GREATER,
    PHPSTART5,
    LESSEQ,
    GREATEREQ,
    NEQ1,
    NEQ2,
    EXP_1,
    EXP_2,
    NEQ3,
    FLOAT1,
    FLOAT2,
    VARPREF,
    ONE_L_COMMENT,
    MULT_L_COMMENT,
    STAR_END
} States;

typedef enum lex {
    L_PHPEND,
    L_LPAR,
    L_RPAR,
    L_COMMA,
    L_SEMICOL,
    L_COLON,
    L_RCURL,
    L_LCURL,
    L_DOT,
    LEOF,
    L_MUL,
    L_SLASH,
    L_PLUS,
    L_DASH,
    L_ID,
    L_PHPSTART,
    L_VARID,
    L_FUNCID,
    L_NUMBER,
    L_ASSIGN,
    L_STRING,
    L_EQ,
    L_EXP,
    L_NEQ,
    L_LESS,
    L_GREATER,
    L_LESSEQ,
    L_GREATEREQ,
    L_FLOAT,
    L_VARPREF,
    K_ELSE,
    K_FUNCTION,
    K_IF,
    K_INT,
    K_NULL,
    K_RETURN,
    K_STRING,
    K_VOID,
    K_WHILE,
    K_DECLARE,
    K_STRICTTYPES,
    K_FLOAT
} token_type;

typedef struct {
    token_type token_type;
    unsigned long long line_index;
    union {
        char* string;
        unsigned long long val;
        double float_val;
        int index;
    };

} token_t;

typedef struct {
    unsigned long long line_num;
    int err_flag;
    int free_token;
    char edge;
    char* token;
    int tokenmem;
    int usedmem;
} scanner_t;

States FSM(States curr_state, char edge);
token_t create_lex(States final, char* token);
token_t get_lex_value();
token_t isKeyword(char* keywd);

void print_lex(token_t token);
#endif
