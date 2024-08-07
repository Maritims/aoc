#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json/token.h"

const char *json_token_type_to_string(json_token_type_t type) {
    switch(type) {
        case JSON_TOKEN_TYPE_COLON:
            return "COLON";
        case JSON_TOKEN_TYPE_COMMA:
            return "COMMA";
        case JSON_TOKEN_TYPE_LEFT_BRACE:
            return "LEFT BRACE";
        case JSON_TOKEN_TYPE_LEFT_BRACKET:
            return "LEFT BRACKET";
        case JSON_TOKEN_TYPE_NUMBER:
            return "NUMBER";
        case JSON_TOKEN_TYPE_RIGHT_BRACE:
            return "RIGHT BRACE";
        case JSON_TOKEN_TYPE_RIGHT_BRACKET:
            return "RIGHT BRACKET";
        case JSON_TOKEN_TYPE_STRING:
            return "STRING";
        case JSON_TOKEN_TYPE_BOOL:
            return "BOOL";
        case JSON_TOKEN_TYPE_NULL:
            return "NULL";
        default:
            return "UKNOWN TYPE";
    }
}

static json_token_t *json_token_create_empty(json_token_type_t type) {
    json_token_t *token = malloc(sizeof(json_token_t));
    if(token == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to allocate memory for JSON token\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    token->type     = type;
    return token;
}

json_token_t *json_token_create(json_token_type_t type, const char *str) {
    json_token_t *token = json_token_create_empty(type);
    if(token == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to create token\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    strcpy(token->value.string_value, str);
    token->value.string_value[strlen(str)] = '\0';
    return token;
}

json_token_t *json_token_create_bool(bool value) {
    json_token_t *token = json_token_create_empty(JSON_TOKEN_TYPE_BOOL);
    if(token == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to create token\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    token->value.boolean_value = value;
    return token;
}

json_token_t *json_token_create_int(int value) {
    json_token_t *token = json_token_create_empty(JSON_TOKEN_TYPE_NUMBER);
    if(token == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to create token\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    token->value.int_value = value;
    return token;
}

json_token_t *json_token_create_string(char *value) {
    json_token_t *token = json_token_create_empty(JSON_TOKEN_TYPE_STRING);
    if(token == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to create token\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    strcpy(token->value.string_value, value);
    token->value.string_value[strlen(value)] = '\0';

    return token;
}

json_token_t *json_token_create_null() {
    json_token_t *token = json_token_create_empty(JSON_TOKEN_TYPE_NULL);
    if(token == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to create token\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    return token;
}

json_token_t **json_token_array_create(size_t capacity) {
    json_token_t **tokens = calloc(capacity, sizeof(json_token_t*));

    if(tokens == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to allocate memory for JSON tokens\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    for(size_t i = 0; i < capacity; i++) {
        tokens[i] = malloc(sizeof(json_token_t));
        if(tokens[i] == NULL) {
            fprintf(stderr, "%s:%s:%d: failed to allocate memory for JSON token at index %zu\n", __FILE__, __func__, __LINE__, i);
            for(size_t j = 0; j < i; j++) {
                free(tokens[j]);
            }
            free(tokens);
            return NULL;
        }
    }

    return tokens;
}

json_token_type_t json_token_get_type(json_token_t *token) {
    if(token == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter token was null\n", __FILE__, __func__, __LINE__);
        return JSON_TOKEN_TYPE_UNDEFINED;
    }

    return token->type;
}

bool json_token_get_bool(json_token_t *token) {
    if(token == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter token was null\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    if(token->type != JSON_TOKEN_TYPE_BOOL) {
        fprintf(stderr, "%s:%s:%d: attempted to retrieve boolean value from a token with type %s\n", __FILE__, __func__, __LINE__, json_token_type_to_string(token->type));
        return false;
    }

    return token->value.boolean_value;
}

int json_token_get_int(json_token_t *token) {
    if(token == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter token was null\n", __FILE__, __func__, __LINE__);
        return INT_MIN;
    }

    if(token->type != JSON_TOKEN_TYPE_NUMBER) {
        fprintf(stderr, "%s:%s:%d: attempted to retrieve int value from a token with type %s\n", __FILE__, __func__, __LINE__, json_token_type_to_string(token->type));
        return INT_MIN;
    }

    return token->value.int_value;
}

char *json_token_get_string(json_token_t *token) {
    if(token == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter token was null\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    switch(token->type) {
        case JSON_TOKEN_TYPE_STRING:
        case JSON_TOKEN_TYPE_COLON:
        case JSON_TOKEN_TYPE_COMMA:
        case JSON_TOKEN_TYPE_LEFT_BRACE:
        case JSON_TOKEN_TYPE_RIGHT_BRACE:
        case JSON_TOKEN_TYPE_LEFT_BRACKET:
        case JSON_TOKEN_TYPE_RIGHT_BRACKET:
            return token->value.string_value;
        default:
            fprintf(stderr, "%s:%s:%d: attempted to retrieve string value from token with type %s\n", __FILE__, __func__, __LINE__, json_token_type_to_string(token->type));
            return NULL;
    }
}
