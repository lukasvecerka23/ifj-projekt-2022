/**
 * Project - IFJ Projekt 2022
 *
 * @author Lukas Vecerka xvecer30
 * @author Jachym Dolezal xdolez0c
 * @author Andrej Nespor xnespo10
 * @author Matej Tomko xtomko06
 *
 * @brief Header file for scanner.h
 */

#ifndef IFJ_SCANNER_H
#define IFJ_SCANNER_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @enum states of our Finite-state machine.
 */
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

/**
 * @enum names of generated tokens
 */
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

/**
 * @struct data for token
 */
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

/**
 * @struct data for scanner
 */
typedef struct {
    unsigned long long line_num;
    int err_flag;
    int free_token;
    char edge;
    char* token;
    int tokenmem;
    int usedmem;
} scanner_t;

/**
 * Our Implementation of FSM. Decides the next state based on the current state
 * and the current character
 * @param curr_state current state
 * @param edge current character
 * @return new current state
 */
States FSM(States curr_state, char edge);

/**
 * Create token based on passed string and gives it token type based on final
 * state
 * @param final state which decides type of parsed token
 * @param token string with current token data
 * @return token struct with all necessary token data
 */
token_t create_lex(States final, char* token);

/**
 * Called to recieve new token
 * @return pointer to a new token
 */
token_t* get_lex_value();

/**
 * Determines if parsed identifier is predefined keyword or function id
 * @param keywd pointer to the string
 * @return token struct with all necessary token data
 */
token_t isKeyword(char* keywd);

/**
 * Loads characters based on states of FSM, in case of final state returns
 * token, otherwise exits program.
 * @param scan struct with scanner data
 * @return token struct with all necessary token data
 */
token_t get_token_data(scanner_t* scan);

/**
 * parse the input string, converts symbols with backslash based on given rules
 * @param str pointer to input string to be parsed
 * @return pointer to a new modified string
 */
char* escape_sequence_parser(char* str);

/**
 * converts double float number from string
 * @param token pointer to a str representing token with float type
 * @return float number as double
 */
double return_float(char* token);

/**
 * converts integer number from string
 * @param token pointer to a str representing token with int type
 * @return int number
 */
int return_digit(char* token);

/**
 * frees token
 * @param token to be freed
 */
void token_free(char* token);

/**
 * todo delete
 * @param token
 */
void print_lex(token_t* token);

#endif

/*END OF FILE*/
