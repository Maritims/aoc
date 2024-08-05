#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generics/value.h"
#include "json/lexer.h"
#include "json/token.h"


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

int *json_lex_number(char** str) {
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
        fprintf(stderr, "%s:%s:%d: failed to allocate memory for output\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    *output = (int)number;
    return output;
}

bool *json_lex_bool(char** str) {
    char *s = *str;

    if(strncmp(s, "true", 4) == 0) {
        bool *output = malloc(sizeof(bool));
        if(output == NULL) {
            fprintf(stderr, "%s:%s:%d: failed to allocate memory for output\n", __FILE__, __func__, __LINE__);
            return NULL;
        }
        *output = true;
        *str += 4;
        return output;
    }

    if(strncmp(s, "false", 5) == 0) {
        bool *output = malloc(sizeof(bool));
        if(output == NULL) {
            fprintf(stderr, "%s:%s:%d: failed to allocate memory for output\n", __FILE__, __func__, __LINE__);
            return NULL;
        }
        *output = false;
        (*str) += 5;
        return output;
    }

    return NULL;
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
    json_token_t *json_tokens   = json_token_array_create(capacity);
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

        char *json_string = json_lex_string(&ptr);
        if (json_string != NULL) {
            generic_value_t *string_value   = generic_value_create_string(json_string);
            json_token_t *token             = json_token_create(JSON_TOKEN_TYPE_STRING, string_value);
            json_token_array_add(&json_tokens, &capacity, token, i);
            free(json_string);
            continue;
        }

        int *json_number = json_lex_number(&ptr);
        if (json_number != NULL) {
            generic_value_t *int_value      = generic_value_create_int(*json_number);
            json_token_t *token             = json_token_create(JSON_TOKEN_TYPE_NUMBER, int_value);
            json_token_array_add(&json_tokens, &capacity, token, i);
            free(json_number);
            continue;
        }

        bool* json_bool = json_lex_bool(&ptr);
        if(json_bool != NULL) {
            generic_value_t *bool_value     = generic_value_create_bool(*json_bool);
            json_token_t *token             = json_token_create(JSON_TOKEN_TYPE_BOOL, bool_value);
            json_token_array_add(&json_tokens, &capacity, token, i);
            free(json_bool);
            continue;
        }

        if(json_lex_null(&ptr)) {
            json_token_t *token = json_token_create(JSON_TOKEN_TYPE_NULL, NULL);
            json_token_array_add(&json_tokens, &capacity, token, i);
            continue;
        }

        json_token_t *token = NULL;
        switch (*ptr) {
            case ',':
                token = json_token_create(JSON_TOKEN_TYPE_COMMA, generic_value_create_string(","));
                break;
            case ':':
                token = json_token_create(JSON_TOKEN_TYPE_COLON, generic_value_create_string(":"));
                break;
            case '[':
                token = json_token_create(JSON_TOKEN_TYPE_LEFT_BRACKET, generic_value_create_string("["));
                break;
            case ']':
                token = json_token_create(JSON_TOKEN_TYPE_RIGHT_BRACKET, generic_value_create_string("]"));
                break;
            case '{':
                token = json_token_create(JSON_TOKEN_TYPE_LEFT_BRACE, generic_value_create_string("{"));
                break;
            case '}':
                token = json_token_create(JSON_TOKEN_TYPE_RIGHT_BRACE, generic_value_create_string("}"));
                break;
            default:
                fprintf(stderr, "%s:%s:%d: unexpected character encountered: %c\n", __FILE__, __func__, __LINE__, *ptr);
                free(buffer);
                free(json_tokens);
                return NULL;
        }

        json_token_array_add(&json_tokens, &capacity, token, i);

        ptr++;
    }

    free(buffer);
    *out_length = i;
    return json_tokens;
}
