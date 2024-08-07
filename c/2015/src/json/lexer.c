#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

json_token_t **json_lex(char *str, size_t *out_length) {
    char* buffer = strdup(str);
    if(buffer == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to duplicate string\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    size_t capacity             = 1024;
    json_token_t **json_tokens  = calloc(capacity, sizeof(json_token_t*));
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
            json_token_t **new_tokens = realloc(json_tokens, new_capacity = sizeof(json_token_t*));
            if(new_tokens == NULL) {
                fprintf(stderr, "%s:%s:%d: failed to allocate additional memory for JSON tokens\n", __FILE__, __func__, __LINE__);
                for(size_t j = 0; j < i; j++) {
                    free(json_tokens[j]);
                }
                free(json_tokens);
                free(buffer);
                return NULL;
            }

            json_tokens = new_tokens;
        }

        char *json_string = json_lex_string(&ptr);
        if (json_string != NULL) {
            json_token_t *token             = json_token_create_string(json_string);
            json_tokens[i++] = token;
            free(json_string);
            continue;
        }

        int *json_number = json_lex_number(&ptr);
        if (json_number != NULL) {
            json_token_t *token = json_token_create_int(*json_number);
            json_tokens[i++]    = token;
            free(json_number);
            continue;
        }

        bool* json_bool = json_lex_bool(&ptr);
        if(json_bool != NULL) {
            json_token_t *token = json_token_create_bool(*json_bool);
            json_tokens[i++]    = token;
            free(json_bool);
            continue;
        }

        if(json_lex_null(&ptr)) {
            json_token_t *token = json_token_create_null();
            json_tokens[i++]    = token;
            continue;
        }

        json_token_t *token = NULL;
        switch (*ptr) {
            case ',':
                token = json_token_create(JSON_TOKEN_TYPE_COMMA, ",");
                break;
            case ':':
                token = json_token_create(JSON_TOKEN_TYPE_COLON, ":");
                break;
            case '[':
                token = json_token_create(JSON_TOKEN_TYPE_LEFT_BRACKET, "[");
                break;
            case ']':
                token = json_token_create(JSON_TOKEN_TYPE_RIGHT_BRACKET, "]");
                break;
            case '{':
                token = json_token_create(JSON_TOKEN_TYPE_LEFT_BRACE, "{");
                break;
            case '}':
                token = json_token_create(JSON_TOKEN_TYPE_RIGHT_BRACE, "}");
                break;
            default:
                fprintf(stderr, "%s:%s:%d: unexpected character encountered: %c\n", __FILE__, __func__, __LINE__, *ptr);
                free(buffer);
                free(json_tokens);
                return NULL;
        }

        json_tokens[i++] = token;
        ptr++;
    }

    free(buffer);
    *out_length = i;
    return json_tokens;
}
