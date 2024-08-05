#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "generics/object.h"
#include "json4c.h"
#include "test4c.h"


char *json_token_to_string(JsonToken* token) {

    switch(token->type) {
    }

    return NULL;
}

generic_value_t *json_parse_object(JsonToken** tokens) {
    if(tokens[0]->type != JSON_TOKEN_TYPE_LEFT_BRACE) {
        printf("The first token should be a left brace when dealing with an object.\n");
        return false;
    }

    (*tokens)++; // Move past the left brace.
    
    generic_value_t *json_object = generic_object_create(1);

    while(true) {
        if((*tokens)->type == JSON_TOKEN_TYPE_RIGHT_BRACE) { // Are we done?
            GenericObject* json_object = generic_create_object(1);
            return json_object;
        }

        JsonToken json_key = **tokens; // Grab the key for later.
        if(json_key.type != JSON_TOKEN_TYPE_STRING) {
            printf("Expected string key, but got %d (value: %s)\n", json_key.type, json_key.value->data.string_value);
            return NULL;
        }

        if(json_key.value->data.string_value == NULL) {
            printf("The string key is NULL.\n");
            return NULL;
        }

        (*tokens)++; // Move past the key.

        if((*tokens)->type != JSON_TOKEN_TYPE_COLON) { // Is there a colon?
            printf("Expected colon, but got %d (value: %s)\n", tokens[0]->type, tokens[0]->value->data.string_value);
            return NULL;
        }

        (*tokens)++; // Move past the colon.

        // We should be at the value now. Parse it.
        GenericValue* json_value = parse(tokens, false);
        generic_add_to_object(json_object, json_key.value->data.string_value, json_value);

        // Are we done?
        if ((*tokens)->type == JSON_TOKEN_TYPE_RIGHT_BRACE) {
            return json_object;
        }

        if ((*tokens)->type != JSON_TOKEN_TYPE_COMMA) {
            debug_print("Expected comma after pair in object, but got %d\n", (*tokens)->type);
            generic_destroy_object(json_object);
            return NULL;
        }

        //debug_print("Moving past the comma.\n", "");
        (*tokens)++;
    }

    debug_print("Expected end of object\n", "");
    return NULL;
}

void json_parse_string(GenericValue **result, char* str) {
    size_t number_of_tokens = 0;
    JsonToken* tokens = lex(str, &number_of_tokens);
    *result = parse(&tokens, true);
}
