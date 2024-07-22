#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "json4c.h"
#include "test4c.h"

#define DEBUG 1

/*
 * lex_string - Gets the copy of the value between two quotes. Advances the pointer past the ending quote.
 * @str: An array of strings.
 * @return: If successful, a copy of the quoted string is returned or NULL if not. The caller must remember to free the memory used by the returned pointer.
 */
char* lex_string(char** str) {
    if (**str != JSON_QUOTE) {

        return NULL;
    }

    (*str)++; // Move past the initial quote.

    char *start = *str;
    char* end_quote = strchr(start, JSON_QUOTE);
    if (end_quote == NULL) {
        return NULL; // No ending quote found.
    }

    size_t length = end_quote - start;
    char* output = calloc(length + 1, sizeof(char));
    if(output == NULL) {
        debug_print("Failed to allocate memory for output.\n", "");
        return NULL;
    }

    strncpy(output, start, length);
    output[length] = '\0';

    *str += length + 1; // Move past the ending quote.

    return output;
}

/*
 * lex_number - Advances the pointer past any minus sign and digit until a non-digit character is encountered and returns number comprised by the digits.
 * @str: An array of strings.
 * @return: If successful, a number or NULL if not. The caller must remember to free the memory used by the returned pointer.
 */
int* lex_number(char** str) {
    char *start = *str;
    char *end;

    // Check for negative sign.
    if(**str == '-') {
        (*str)++;
    }

    // Ensure there is at least one digit.
    // There aren't any digits with an ASCII code lower than the ASCII code of the character '0'.
    // There aren't any digits with an ASCII code higher than the ASCII code of the character '9'.
    if(**str < '0' || **str > '9') {
        *str = start;
        return NULL;
    }

    // Move past the digits.
    while(**str >= '0' && **str <= '9') {
        (*str)++;
    }

    // Null terminate the number string temporarily for strtol.
    char temp = **str;
    **str = '\0';

    // Convert string to long.
    long number = strtol(start, &end, 10);

    // Restore the character after the number.
    **str = temp;

    // Check for conversion errors.
    if(end != *str || errno == ERANGE || number < INT_MIN || number > INT_MAX) {
        *str = start; // Reset pointer.
        return NULL;
    }

    int* output = malloc(sizeof(int));
    if(output == NULL) {
        debug_print("Failed to allocate memory for number.\n", "");
        return NULL;
    }

    *output = (int)number;
    return output;
}

/*
 * lex_bool - Gets the boolean value represtend by the current string. Advances the pointer past the boolean value.
 * @str: An array of strings.
 * @return: If successful, true or false or NULL if not. The caller must remember to free the memory used by the returned pointer.
 */
bool* lex_bool(char** str) {
    char *s = *str;

    if(strncmp(s, "true", 4) == 0) {
        bool *output = malloc(sizeof(bool));
        if(output == NULL) {
            debug_print("Failed to allocate memory for boolean.\n", "");
            return NULL;
        }
        *output = true;
        *str += 4;
        return output;
    }

    if(strncmp(s, "false", 5) == 0) {
        bool *output = malloc(sizeof(bool));
        if(output == NULL) {
            debug_print("Failed to allocate memory for boolean.\n", "");
            return NULL;
        }
        *output = false;
        (*str) += 5;
        return output;
    }

    return NULL;
}

/*
 * lex_null: Checks if the current string represents a null value. Advances the pointer past the string representation.
 * @str: An array of strings.
 * @return: True or false depending on whether the current string is "null".
 */
bool lex_null(char** str) {
    char *s = *str;

    // Check for NULL and string length to prevent a potential out of bounds error from strncmp.
    if(s == NULL || strlen(s) < 4) {
        return false;
    }

    if(strncmp(s, "null", 4) == 0) {
        (*str) += 4;
        return true;
    }

    return false;
}

JsonToken* lex(char* str, size_t *out_length) {
    char* buffer = strdup(str);
    if(buffer == NULL) {
        debug_print("Failed to duplicate string into buffer.\n", "");
        return NULL;
    }

    size_t capacity = 1024;
    JsonToken* json_tokens = calloc(capacity, sizeof(JsonToken));
    if(json_tokens == NULL) {
        debug_print("Failed to allocate memory for json tokens.\n", "");
        free(buffer);
        return NULL;
    }

    size_t i = 0;
    char *ptr = buffer;

    while (*ptr != '\0') {
        // Skip whitespace.
        while(isspace((unsigned char)*ptr)) {
            ptr++;
        }

        if(*ptr == '\0') {
            break;
        }

        if(i >= capacity) {
            capacity *= 2;
            JsonToken *new_tokens = realloc(json_tokens, capacity * sizeof(JsonToken));
            if(new_tokens == NULL) {
                debug_print("Failed to allocate additional memory for JSON tokens.\n", "");
                free(buffer);
                free(json_tokens);
                return NULL;
            }
            json_tokens = new_tokens;
        }

        char *json_string = lex_string(&ptr);
        if (json_string != NULL) {
            JsonToken token = { .type = JSON_TOKEN_TYPE_STRING, .value = generic_create_string(json_string) };
            json_tokens[i++] = token;
            free(json_string);
            continue;
        }

        int *json_number = lex_number(&ptr);
        if (json_number != NULL) {
            JsonToken token = { .type = JSON_TOKEN_TYPE_NUMBER, .value = generic_create_int(*json_number) };
            json_tokens[i++] = token;
            free(json_number);
            continue;
        }

        bool* json_bool = lex_bool(&ptr);
        if(json_bool != NULL) {
            JsonToken token = { .type = JSON_TOKEN_TYPE_BOOL, .value = generic_create_bool(*json_bool) };
            json_tokens[i++] = token;
            free(json_bool);
            continue;
        }

        if(lex_null(&ptr)) {
            JsonToken token = { .type = JSON_TOKEN_TYPE_NULL, .value = NULL };
            json_tokens[i++] = token;
            continue;
        }

        JsonToken token = { 0 };
        switch (*ptr) {
            case ',':
                token = (JsonToken){ .type = JSON_TOKEN_TYPE_COMMA, .value = generic_create_string(",") };
                json_tokens[i++] = token;
                break;
            case ':':
                token = (JsonToken){ .type = JSON_TOKEN_TYPE_COLON, .value = generic_create_string(":") };
                json_tokens[i++] = token;
                break;
            case '[':
                token = (JsonToken){ .type = JSON_TOKEN_TYPE_LEFT_BRACKET, .value = generic_create_string("[") };
                json_tokens[i++] = token;
                break;
            case ']':
                token = (JsonToken){ .type = JSON_TOKEN_TYPE_RIGHT_BRACKET, .value = generic_create_string("]") };
                json_tokens[i++] = token;
                break;
            case '{':
                token = (JsonToken){ .type = JSON_TOKEN_TYPE_LEFT_BRACE, .value = generic_create_string("{") };
                json_tokens[i++] = token;
                break;
            case '}':
                token = (JsonToken){ .type = JSON_TOKEN_TYPE_RIGHT_BRACE, .value = generic_create_string("}") };
                json_tokens[i++] = token;
                break;
            default:
                debug_print("Unexpected character encountered: %c\n", *ptr);
                free(buffer);
                free(json_tokens);
                return NULL;
        }

        ptr++;
    }

    free(buffer);
    *out_length = i;
    return json_tokens;
}

char *json_token_to_string(JsonToken* token) {

    switch(token->type) {
    }

    return NULL;
}

GenericValue* parse(JsonToken** tokens, bool is_root) {
    if (is_root && (tokens[0]->type != JSON_TOKEN_TYPE_LEFT_BRACE && tokens[0]->type != JSON_TOKEN_TYPE_LEFT_BRACKET)) {
        debug_print("Expected array or object, but the first token was neither a left bracket nor a left brace.\n", "");
        return NULL;
    }

    GenericValue* gv = malloc(sizeof(GenericValue));
    if(gv == NULL) {
        debug_print("Failed to allocate memory for generic value.\n", "");
        return NULL;
    }

    switch(tokens[0]->type) {
        case JSON_TOKEN_TYPE_LEFT_BRACKET:
            gv->type = TYPE_ARRAY;
            gv->data.array_value = parse_array(tokens);
        break;
        case JSON_TOKEN_TYPE_LEFT_BRACE:
            gv->type = TYPE_OBJECT;
            gv->data.object_value = parse_object(tokens);
        break;
        default:
            free(gv);
            gv = tokens[0]->value;
        break;
    }

    (*tokens)++;

    return gv;
}

GenericArray* parse_array(JsonToken** tokens) {
    if(tokens[0]->type != JSON_TOKEN_TYPE_LEFT_BRACKET) {
        printf("The first token should be a left bracket when dealing with an array.\n");
        return NULL;
    }
    (*tokens)++; // Move past the left bracket.

    GenericArray* json_array = generic_create_array(1);

    while(true) {
        if(tokens[0]->type == JSON_TOKEN_TYPE_RIGHT_BRACKET) {
            return json_array;
        }

        GenericValue* json = parse(tokens, false);
        generic_add_to_array(json_array, json);

        if (tokens[0]->type == JSON_TOKEN_TYPE_RIGHT_BRACKET) {
            return json_array;
        }

        if (tokens[0]->type != JSON_TOKEN_TYPE_COMMA) {
            char *token_string = generic_value_to_string(tokens[0]->value);
            debug_print("Expected comma after object in array, but the current token is %s\n", token_string);
            free(token_string);
            return NULL;
        }

        //debug_print("Moving past the comma.\n", "");
        (*tokens)++;
    }

    debug_print("Expected end of array, but the current token type is %d.\n", tokens[0]->type);
    return NULL;
}

GenericObject* parse_object(JsonToken** tokens) {
    if(tokens[0]->type != JSON_TOKEN_TYPE_LEFT_BRACE) {
        printf("The first token should be a left brace when dealing with an object.\n");
        return false;
    }

    (*tokens)++; // Move past the left brace.
    
    GenericObject *json_object = generic_create_object(1);

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

GenericValue* json_parse_string(char* str) {
    size_t number_of_tokens = 0;
    JsonToken* tokens = lex(str, &number_of_tokens);
    GenericValue* gv = parse(&tokens, true);
    return gv;
}
