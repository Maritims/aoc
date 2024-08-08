#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json/lexer.h"

const char *json_token_type_strings[] = {
    "UNDEFINED",
    "COLON",
    "COMMA",
    "LEFT_BRACE",
    "LEFT_BRACKET",
    "NUMBER",
    "RIGHT_BRACE",
    "RIGHT_BRACKET",
    "STRING",
    "BOOL",
    "NULL"
};

char *json_lex_string(char **str) {
    if (**str != JSON_QUOTE) {
        return NULL;
    }

    (*str)++; // Move past the initial quote.

    char *start     = *str;
    char *end_quote = strchr(start, JSON_QUOTE);
    if (end_quote == NULL) {
        return NULL; // No ending quote found.
    }

    size_t length   = end_quote - start;
    char* output    = calloc(length + 1, sizeof(char));
    if(output == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to allocate memory for output\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    strncpy(output, start, length);
    output[length] = '\0';

    *str += length + 1; // Move past the ending quote.

    return output;
}

bool json_lex_number(int *output, char **str) {
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
        return false;
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
        return false;
    }

    *output = (int)number;
    return true;
}

bool json_lex_bool(bool *output, char** str) {
    char *s = *str;

    if(strncmp(s, "true", 4) == 0) {
        *output = true;
        *str += 4;
        return true;
    }

    if(strncmp(s, "false", 5) == 0) {
        *output = false;
        (*str) += 5;
        return true;
    }

    return false;
}

bool json_lex_null(char** str) {
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

json_token_t *json_lex(char *str, size_t *out_length) {
    char* buffer = strdup(str);
    if(buffer == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to duplicate string\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    size_t capacity             = 1024;
    json_token_t *json_tokens   = calloc(capacity, sizeof(json_token_t));
    if(json_tokens == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to create JSON token array\n", __FILE__, __func__, __LINE__);
        free(buffer);
        return NULL;
    }

    size_t i    = 0;
    char *ptr   = buffer;

    while (*ptr != '\0') {
        // Skip whitespace.
        while(isspace((unsigned char)*ptr)) {
            ptr++;
        }

        if(*ptr == '\0') {
            break;
        }

        if(i >= capacity) {
            size_t new_capacity = capacity * 2;
            json_token_t *new_tokens = realloc(json_tokens, new_capacity * sizeof(json_token_t));
            if(new_tokens == NULL) {
                fprintf(stderr, "%s:%s:%d: failed to allocate additional memory for JSON tokens\n", __FILE__, __func__, __LINE__);
                free(json_tokens);
                free(buffer);
                return NULL;
            }

            json_tokens = new_tokens;
            capacity = new_capacity;
        }

        char *json_string = json_lex_string(&ptr);
        if (json_string != NULL) {
            size_t json_string_len = strlen(json_string);
            json_token_t token = {
                .type = JSON_TOKEN_TYPE_STRING,
            };
            snprintf(token.value.string_value, json_string_len + 1, "%s", json_string);
            json_tokens[i++] = token;
            free(json_string);
            continue;
        }

        int json_number;
        if(json_lex_number(&json_number, &ptr)) {
            json_tokens[i++] = (json_token_t){
                .type   = JSON_TOKEN_TYPE_NUMBER,
                .value  = {
                    .int_value = json_number
                }
            };
            continue;
        }

        bool json_bool;
        if(json_lex_bool(&json_bool, &ptr)) {
            json_tokens[i++] = (json_token_t){
                .type   = JSON_TOKEN_TYPE_BOOL,
                .value  = {
                    .boolean_value = json_bool
                }
            };
            continue;
        }

        if(json_lex_null(&ptr)) {
            json_tokens[i++] = (json_token_t){
                .type = JSON_TOKEN_TYPE_NULL
            };
            continue;
        }

        json_token_type_t token_type;
        switch (*ptr) {
            case ',':
                token_type = JSON_TOKEN_TYPE_COMMA;
                break;
            case ':':
                token_type = JSON_TOKEN_TYPE_COLON;
                break;
            case '[':
                token_type = JSON_TOKEN_TYPE_LEFT_BRACKET;
                break;
            case ']':
                token_type = JSON_TOKEN_TYPE_RIGHT_BRACKET;
                break;
            case '{':
                token_type = JSON_TOKEN_TYPE_LEFT_BRACE;
                break;
            case '}':
                token_type = JSON_TOKEN_TYPE_RIGHT_BRACE;
                break;
            default:
                fprintf(stderr, "%s:%s:%d: unexpected character encountered: %c\n", __FILE__, __func__, __LINE__, *ptr);
                free(buffer);
                free(json_tokens);
                return NULL;
        }

        json_token_t token = {
            .type = token_type,
        };
        token.value.string_value[0] = *ptr;
        token.value.string_value[1] = '\0';
        json_tokens[i++] = token;
        ptr++;
    }

    free(buffer);
    *out_length = i;
    return json_tokens;
}
