#include <stdio.h>
#include "generics/array.h"
#include "generics/object.h"
#include "generics/value.h"
#include "json/lexer.h"
#include "json/parser.h"
#include "json/token.h"

static generic_value_t *get_generic_value_from_json_token(json_token_t *token) {
    if(token == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter token was null\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    generic_value_t *v              = NULL;
    json_token_type_t token_type    = json_token_get_type(token);

    switch(token_type) {
        case JSON_TOKEN_TYPE_BOOL:
            v = generic_value_create_bool(json_token_get_bool(token));
        break;
        case JSON_TOKEN_TYPE_NUMBER:
            v = generic_value_create_int(json_token_get_int(token));
        break;
        case JSON_TOKEN_TYPE_NULL:
            v = NULL;
        break;
        default:
            v = generic_value_create_string(json_token_get_string(token));
        break;
    }

    return v;
}

generic_value_t *json_parse(json_token_t ***tokens, bool is_root) {
    json_token_type_t token_type = json_token_get_type(*tokens[0]);

    if (is_root && (token_type != JSON_TOKEN_TYPE_LEFT_BRACE && token_type != JSON_TOKEN_TYPE_LEFT_BRACKET)) {
        fprintf(stderr, "%s:%s:%d: expected array or object, but the first token was neither a left bracket nor a left brace (token_type = %d/%s)\n", __FILE__, __func__, __LINE__, token_type, json_token_type_to_string(token_type));
        return NULL;
    }

    generic_value_t *v = generic_value_create();
    if(v == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to create generic value\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    switch(token_type) {
        case JSON_TOKEN_TYPE_LEFT_BRACKET:
            generic_value_set_type(v, GENERIC_VALUE_TYPE_ARRAY);
            generic_value_set_array(v, json_parse_array(tokens));
        break;
        case JSON_TOKEN_TYPE_LEFT_BRACE:
            generic_value_set_type(v, GENERIC_VALUE_TYPE_OBJECT);
            generic_value_set_object(v, json_parse_object(tokens));
        break;
        default:
            generic_value_destroy(v);
            v = get_generic_value_from_json_token(*tokens[0]);
        break;
    }

    (*tokens)++;

    return v;
}

generic_array_t *json_parse_array(json_token_t ***tokens) {
    if(json_token_get_type(*tokens[0]) != JSON_TOKEN_TYPE_LEFT_BRACKET) {
        fprintf(stderr, "%s:%s:%d: the first token should be a left bracket when dealing with an array.\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    (*tokens)++; // Move past the left bracket.

    generic_array_t *json_array = generic_array_create(1);

    while(true) {
        if(json_token_get_type(*tokens[0]) == JSON_TOKEN_TYPE_RIGHT_BRACKET) {
            return json_array;
        }

        generic_value_t *json = json_parse(tokens, false);
        generic_array_add(json_array, json);

        if (json_token_get_type(*tokens[0]) == JSON_TOKEN_TYPE_RIGHT_BRACKET) {
            return json_array;
        }

        if (json_token_get_type(*tokens[0]) != JSON_TOKEN_TYPE_COMMA) {
            generic_value_t *token_value    = get_generic_value_from_json_token(*tokens[0]);
            char *token_string              = generic_value_to_string(token_value);

            fprintf(stderr, "%s:%s:%d: expected comma after object in array, but the current token is %s\n", __FILE__, __func__, __LINE__, token_string);
            free(token_string);
            
            return NULL;
        }

        (*tokens)++; // Move past the comma.
    }

    fprintf(stderr, "%s:%s:%d: expected end of array, but the current token type is %d\n", __FILE__, __func__, __LINE__, json_token_get_type(*tokens[0]));
    generic_array_destroy(generic_value_create_array(json_array));

    return NULL;
}

generic_object_t *json_parse_object(json_token_t ***tokens) {
    if(json_token_get_type(*tokens[0]) != JSON_TOKEN_TYPE_LEFT_BRACE) {
        fprintf(stderr, "%s:%s:%d: the first token should be a left brace when dealing with an object\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    (*tokens)++; // Move past the left brace.

    generic_object_t *json_object   = generic_object_create(1);

    while(true) {
        if(json_token_get_type(*tokens[0]) == JSON_TOKEN_TYPE_RIGHT_BRACE) { // Are we done?
            return json_object;
        }

        json_token_type_t json_key_type = json_token_get_type(*tokens[0]);
        generic_value_t *json_key_value = get_generic_value_from_json_token(*tokens[0]);
        char *json_key_string_value     = generic_value_get_string(json_key_value);
        if(json_key_type != JSON_TOKEN_TYPE_STRING) {
            printf("Expected string key, but got %d (value: %s)\n", json_key_type, json_key_string_value);
            return NULL;
        }

        if(json_key_string_value == NULL) {
            printf("The string key is NULL.\n");
            return NULL;
        }

        (*tokens)++; // Move past the key.
        generic_value_t *token_value    = get_generic_value_from_json_token(*tokens[0]);
        char *token_string_value        = generic_value_get_string(token_value);

        if(json_token_get_type(*tokens[0]) != JSON_TOKEN_TYPE_COLON) { // Is there a colon?
            fprintf(stderr, "%s:%s:%d: expected colon, but got %d (value: %s)\n", __FILE__, __func__, __LINE__, json_token_get_type(*tokens[0]), token_string_value);
            return NULL;
        }

        (*tokens)++; // Move past the colon.

        // We should be at the value now. Parse it.
        generic_value_t *json_value = json_parse(tokens, false);
        generic_object_add(json_object, json_key_string_value, json_value);

        // Are we done?
        if (json_token_get_type(*tokens[0]) == JSON_TOKEN_TYPE_RIGHT_BRACE) {
            return json_object;
        }

        if (json_token_get_type(*tokens[0]) != JSON_TOKEN_TYPE_COMMA) {
            fprintf(stderr, "%s:%s:%d: expected comma after pair in object, but got %d\n", __FILE__, __func__, __LINE__, json_token_get_type(*tokens[0]));
            generic_object_destroy(generic_value_create_object(json_object));
            return NULL;
        }

        (*tokens)++; // Move past the comma.
    }

    fprintf(stderr, "%s:%s:%d: expected end of object\n", __FILE__, __func__, __LINE__);
    return NULL;
}

void json_parse_string(generic_value_t **result, char *str) {
    size_t tokens_length    = 0;
    json_token_t **tokens   = json_lex(str, &tokens_length);
    *result = json_parse(&tokens, true);
}
