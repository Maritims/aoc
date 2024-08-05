#ifndef JSON_TOKEN_H
#define JSON_TOKEN_H

#include <stdlib.h>
#include "generics/types.h"

typedef enum json_token_type_t {
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

typedef struct json_token_t json_token_t;

/**
 * Create a new JSON token.
 * @param type Token type.
 * @param value Token value.
 * @return The new token allocated on the heap. It's the responsibility of the caller to free the memory.
 */
json_token_t *json_token_create(json_token_type_t type, generic_value_t *value);

/**
 * Create a new array of JSON tokens.
 * @param capacity Array capacity.
 * @return The new array allocated on the heap. It's the responsibility of the caller to free the memory.
 */
json_token_t *json_token_array_create(size_t capacity);

/**
 * Add token to JSON token array.
 * @param tokens Token array.
 * @param capacity Token array capacity.
 * @param token New token.
 * @param i New token position in array.
 */
void json_token_array_add(json_token_t **tokens, size_t *capacity, json_token_t *token, size_t i);

/**
 * Advance the token array pointer by one.
 * @param Token array.
 */
void json_token_array_advance(json_token_t **tokens);

/**
 * Get the type of a JSON token.
 * @param JSON token.
 * @return Type of JSON token.
 */
json_token_type_t json_token_get_type(json_token_t *token);

/*
 * Get the value of a JSON token.
 * @param JSON token.
 * @return Value of JSON token.
 */
generic_value_t *json_token_get_value(json_token_t *token);

#endif
