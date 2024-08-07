#ifndef JSON_TOKEN_H
#define JSON_TOKEN_H

#include <stdbool.h>

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

typedef union json_token_value_t json_token_value_t; 

typedef struct json_token_t json_token_t;

/**
 * Create a JSON token with a specific type and string value.
 * @param type Token type.
 * @param value Token value.
 * @return A new JSON token. It's the responsibility of the caller to free the allocated memoryl.
 */
json_token_t *json_token_create(json_token_type_t type, const char *value);

/**
 * Create a JSON token with a boolean value.
 * @param value True or false.
 * @return A new JSON token. It's the responsibility of the caller to free the allocated memory.
 */
json_token_t *json_token_create_bool(bool value);

/**
 * Create a JSON token with an int value.
 * @param value Any int.
 * @return A new JSON token. It's the responsibility of the caller to free the allocated memory.
 */
json_token_t *json_token_create_int(int value);

/**
 * Create a JSON token with a string value.
 * @param value Any string. Can be NULL.
 * @return A new JSON token. It's the responsibility of the caller to free the allocated memory.
 */
json_token_t *json_token_create_string(char *value);

/**
 * Create JSON token with a null value.
 * @return A new JSON token. It's the responsibility of the caller to free the allocated memory.
 */
json_token_t *json_token_create_null();

/**
 * Free the memory used by the token.
 * @param Token.
 */
void json_token_destroy(json_token_t *token);
/**
 * Get a human friendly representation of a JSON token type.
 * @param result String to store the representation in.
 * @param type JSON token type to be understood.
 * @return A string repsentation of the JSON token type.
 */
const char *json_token_type_to_string(json_token_type_t type);


json_token_type_t   json_token_get_type(json_token_t *token);
bool                json_token_get_bool(json_token_t *token);
int                 json_token_get_int(json_token_t *token);
char*               json_token_get_string(json_token_t *token);

#endif
