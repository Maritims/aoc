#ifndef JSON_LEXER_H
#define JSON_LEXER_H

#include <stdbool.h>
#include <stdlib.h>

#define JSON_QUOTE '"'

typedef enum json_token_type_t {
    JSON_TOKEN_TYPE_UNDEFINED = 1,
    JSON_TOKEN_TYPE_COLON,
    JSON_TOKEN_TYPE_COMMA,
    JSON_TOKEN_TYPE_LEFT_BRACE,
    JSON_TOKEN_TYPE_LEFT_BRACKET,
    JSON_TOKEN_TYPE_NUMBER,
    JSON_TOKEN_TYPE_RIGHT_BRACE,
    JSON_TOKEN_TYPE_RIGHT_BRACKET,
    JSON_TOKEN_TYPE_STRING,
    JSON_TOKEN_TYPE_BOOL,
    JSON_TOKEN_TYPE_NULL
} json_token_type_t;

extern const char *json_token_type_strings[];

typedef union json_token_value_t {
    bool    boolean_value;
    int     int_value;
    char    string_value[1024];
} json_token_value_t; 

typedef struct json_token_t {
    json_token_type_t   type;
    json_token_value_t  value;
} json_token_t;

/**
 * Get the value between two quotes and advance the pointer past the ending quote. Allocates a new string on the heap containing a copy of the quoted string.
 * @param str Array of strings.
 * @return A copy of the quoted string is returned upon success, otherwise NULL is returned. It's the responsibility of the caller to free the memory allocated for the new string.
 */
char *json_lex_string(char **str);

/**
 * Get a number by advancing the pointer past any minus sign and digit until a non-digit character is encountered.
 * @param output Pointer to output.
 * @param str Array of strings.
 * @return True if successful, false if not.
 */
bool json_lex_number(int *output, char **str);

/**
 * Get the boolean value represented by the current string. Advances the pointer past the boolean value.
 * @param output Pointer to output.
 * @param str Array of strings.
 * @return True if successful, false if not. Must not be confused with the actual boolean value represented by the current string.
 */
bool json_lex_bool(bool *output, char **str);

/**
 * Check whether the current string represents a null value. Advances the pointer psat the string representation.
 * @param str Array of strings.
 * @return True or false depending on whether the current string is "null".
 */
bool json_lex_null(char **str);

json_token_t *json_lex(char *str, size_t *out_length);

#endif
