#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "generics/types.h"
#include "json/token.h"

struct json_token_t {
    json_token_type_t type;
    generic_value_t *value;
};

json_token_t *json_token_create(json_token_type_t type, generic_value_t *value) {
    if(value == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter value was null\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    json_token_t *token = malloc(sizeof(json_token_t));
    if(token == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to allocate memory for JSON token\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    token->type = type;
    token->value = value;

    return token;
}

json_token_t *json_token_array_create(size_t capacity) {
    json_token_t *tokens = calloc(capacity, sizeof(json_token_t));

    if(tokens == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to allocate memory for JSON tokens\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    return tokens;
}

void json_token_array_add(json_token_t **tokens, size_t *capacity, json_token_t *token, size_t i) {
    if(i >= *capacity) {
        size_t new_capacity = *capacity * 2;
        json_token_t *new_tokens = realloc(*tokens, new_capacity * sizeof(json_token_t));
        if(new_tokens == NULL) {
            fprintf(stderr, "%s:%s:%d: failed to allocate additional memory for JSON tokens\n", __FILE__, __func__, __LINE__);
            return;
        }

        *capacity = new_capacity;
        *tokens = new_tokens;
    }

    (*tokens)[i] = *token;
}

void json_token_array_advance(json_token_t **tokens) {
    if(tokens == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter tokens was null\n", __FILE__, __func__, __LINE__);
        return;
    }

    (*tokens)++;
}

json_token_type_t json_token_get_type(json_token_t *token) {
    return token->type;
}

generic_value_t *json_token_get_value(json_token_t *token) {
    return token->value;
}
