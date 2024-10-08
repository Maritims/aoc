#include <stdbool.h>
#include <stdlib.h>
#include "generics/types.h"

#define JSON_QUOTE '"'

typedef enum JsonTokenType {
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
} JsonTokenType;

typedef struct {
    JsonTokenType type;
    generic_value_t *value;
} JsonToken;

/**
 * Get the value between two quotes and advance the pointer past the ending quote. Allocates a new string on the heap containing a copy of the quoted string.
 * @param str Array of strings.
 * @return A copy of the quoted string is returned upon success, otherwise NULL is returned. It's the responsibility of the caller to free the memory allocated for the new string.
 */
char *json_lex_string(char **str);

/**
 * Get a number by advancing the pointer past any minus sign and digit until a non-digit character is encountered.
 * @param str Array of strings.
 * @return An integer pointer pointing to the number represented by the discovered digits upon success, otherwise NULL. It's the responsibility of the caller to free the memory allocated for the new integer.
 */
int *json_lex_number(char **str);

/**
 * Get the boolean value represented by the current string. Advances the pointer past the boolean value.
 * @param str Array of strings.
 * @return A boolean pointer pointing to the number represented by the current string upon success, otherwise NULL. It's the responsibility of the caller to free the memory allocated for the new boolan.
 */
bool *json_lex_bool(char **str);

/**
 * Check whether the current string represents a null value. Advances the pointer psat the string representation.
 * @param str Array of strings.
 * @return True or false depending on whether the current string is "null".
 */
bool json_lex_null(char **str);

JsonToken *json_lex(char *str, size_t *out_length);

generic_value_t *json_parse(JsonToken **tokens, bool is_root);

generic_value_t *json_parse_array(JsonToken **tokens);

generic_value_t *json_parse_object(JsonToken **tokens);

void json_parse_string(generic_value_t **result, char *str);
