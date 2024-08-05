#include <stdio.h>
#include "generics/array.h"
#include "generics/object.h"
#include "generics/value.h"
#include "json/lexer.h"
#include "json/parser.h"
#include "json/token.h"

generic_value_t *json_parse(json_token_t **tokens, bool is_root) {
    json_token_type_t token_type = json_token_get_type(tokens[0]);

    if (is_root && (token_type != JSON_TOKEN_TYPE_LEFT_BRACE && token_type != JSON_TOKEN_TYPE_LEFT_BRACKET)) {
        fprintf(stderr, "%s:%s:%d: expected array or object, but the first token was neither a left bracket nor a left brace.\n", __FILE__, __func__, __LINE__);
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
            v = json_token_get_value(tokens[0]);
        break;
    }

    json_token_array_advance(tokens);

    return v;
}

generic_array_t *json_parse_array(json_token_t **tokens) {
    json_token_type_t token_type = json_token_get_type(tokens[0]);

    if(token_type != JSON_TOKEN_TYPE_LEFT_BRACKET) {
        fprintf(stderr, "%s:%s:%d: the first token should be a left bracket when dealing with an array.\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    json_token_array_advance(tokens); // Move past the left bracket.
    token_type                  = json_token_get_type(tokens[0]);
    generic_array_t *json_array = generic_array_create(1);

    while(true) {
        if(token_type == JSON_TOKEN_TYPE_RIGHT_BRACKET) {
            return json_array;
        }

        generic_value_t *json = json_parse(tokens, false);
        generic_array_add(json_array, json);

        if (token_type == JSON_TOKEN_TYPE_RIGHT_BRACKET) {
            return json_array;
        }

        if (token_type != JSON_TOKEN_TYPE_COMMA) {
            generic_value_t *token_value    = json_token_get_value(tokens[0]);
            char *token_string              = generic_value_to_string(token_value);

            fprintf(stderr, "%s:%s:%d: expected comma after object in array, but the current token is %s\n", __FILE__, __func__, __LINE__, token_string);
            free(token_string);
            
            return NULL;
        }

        //debug_print("Moving past the comma.\n", "");
        json_token_array_advance(tokens);
        token_type = json_token_get_type(tokens[0]);
    }

    fprintf(stderr, "%s:%s:%d: expected end of array, but the current token type is %d\n", __FILE__, __func__, __LINE__, token_type);
    generic_array_destroy(generic_value_create_array(json_array));

    return NULL;
}

generic_object_t *json_parse_object(json_token_t **tokens) {
    json_token_type_t token_type = json_token_get_type(tokens[0]);
    if(token_type != JSON_TOKEN_TYPE_LEFT_BRACE) {
        fprintf(stderr, "%s:%s:%d: the first token should be a left brace when dealing with an object\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    json_token_array_advance(tokens); // Move past the left brace.
    token_type                      = json_token_get_type(tokens[0]);
    generic_object_t *json_object   = generic_object_create(1);

    while(true) {
        if(token_type == JSON_TOKEN_TYPE_RIGHT_BRACE) { // Are we done?
            generic_object_t *json_object = generic_object_create(1);
            return json_object;
        }

        json_token_type_t json_key_type = json_token_get_type(tokens[0]);
        generic_value_t *json_key_value = json_token_get_value(tokens[0]);
        char *json_key_string_value     = generic_value_get_string(json_key_value);
        if(json_key_type != JSON_TOKEN_TYPE_STRING) {
            printf("Expected string key, but got %d (value: %s)\n", json_key_type, json_key_string_value);
            return NULL;
        }

        if(json_key_string_value == NULL) {
            printf("The string key is NULL.\n");
            return NULL;
        }

        json_token_array_advance(tokens); // Move past the key.
        token_type                      = json_token_get_type(tokens[0]);
        generic_value_t *token_value    = json_token_get_value(tokens[0]);
        char *token_string_value        = generic_value_get_string(token_value);

        if(token_type != JSON_TOKEN_TYPE_COLON) { // Is there a colon?
            fprintf(stderr, "%s:%s:%d: expected colon, but got %d (value: %s)\n", __FILE__, __func__, __LINE__, token_type, token_string_value);
            return NULL;
        }

        json_token_array_advance(tokens); // Move past the colon.

        // We should be at the value now. Parse it.
        generic_value_t *json_value = json_parse(tokens, false);
        generic_object_add(json_object, json_key_string_value, json_value);
        token_type = json_token_get_type(tokens[0]);

        // Are we done?
        if (token_type == JSON_TOKEN_TYPE_RIGHT_BRACE) {
            return json_object;
        }

        if (token_type != JSON_TOKEN_TYPE_COMMA) {
            fprintf(stderr, "%s:%s:%d: expected comma after pair in object, but got %d\n", __FILE__, __func__, __LINE__, token_type);
            generic_object_destroy(generic_value_create_object(json_object));
            return NULL;
        }

        //debug_print("Moving past the comma.\n", "");
        json_token_array_advance(tokens);
    }

    fprintf(stderr, "%s:%s:%d: expected end of object\n", __FILE__, __func__, __LINE__);
    return NULL;
}

void json_parse_string(generic_value_t **result, char *str) {
    size_t tokens_length = 0;
    json_token_t *tokens = json_lex(str, &tokens_length);
    *result = json_parse(&tokens, true);
}
