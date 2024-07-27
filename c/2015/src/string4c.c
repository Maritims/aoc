#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "string4c.h"

#define DEBUG 1

bool string_is_numeric(char *str)
{
    if (str == NULL || *str == '\0')
    {
        return false;
    }

    for (size_t i = 0; i < strlen(str); i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            return false;
        }
    }
    return true;
}

void string_split(char ***result, size_t *result_size, char *str, char *delimiter) {
    char *save_ptr;
    char *token;
    size_t capacity = 10;
    size_t size = 0;
    char *str_copy = strdup(str);

    if (str_copy == NULL)
    {
        fprintf(stderr, "Unable to create copy for tokenization\n");
        return;
    }

    char **lines = calloc(capacity, sizeof(char *));
    if (lines == NULL) {
        fprintf(stderr, "%s:%d: Failed allocating memory for lines: %s\n", __func__, __LINE__, strerror(errno));
        free(str_copy);
        return;
    }

    token = strtok_r(str_copy, delimiter, &save_ptr);

    while (token != NULL) {
        if (size >= capacity) {
            capacity += 10;
            
            char **temp = realloc(lines, capacity * sizeof(char *));
            if (temp == NULL)
            {
                fprintf(stderr, "%s:%d: Failed allocating additional memory for lines: %s\n", __func__, __LINE__, strerror(errno));
                free(str_copy);
                for (size_t i = 0; i < size; ++i)
                {
                    free(lines[i]);
                }
                free(lines);
                return;
            }

            lines = temp;
        }

        if (strcmp(token, delimiter) == 0) {
            token = strtok_r(NULL, delimiter, &save_ptr);
            continue;
        }

        lines[size] = strdup(token);
        if (lines[size] == NULL) {
            fprintf(stderr, "Unable to allocate memory for token\n");
            free(str_copy);
            for (size_t i = 0; i < size; ++i)
            {
                free(lines[i]);
            }
            free(lines);
            return;
        }

        token = strtok_r(NULL, delimiter, &save_ptr);
        size++;
    }

    *result = lines;
    *result_size = size;

    free(str_copy);
}

// Duplicates the given string and trims leading and trailing whitespace. Does not modify the source string.
char *string_trim(char *str)
{
    if (str == NULL)
    {
        fprintf(stderr, "Parameter \"str\" cannot be NULL\n");
        return NULL;
    }

    if (strlen(str) == 0)
    {
        fprintf(stderr, "Parameter \"str\" cannot be empty\n");
        return NULL;
    }

    char *str_copy = strdup(str);
    if (str_copy == NULL)
    {
        fprintf(stderr, "Unable to duplicate string\n");
        return NULL;
    }

    size_t size = strlen(str_copy);
    // Point to the last character of the string. Subtract one because we don't want to look at the null terminator.
    char *end = str_copy + size - 1;

    // Move the end pointer backwards until we encounter a non-whitespace character.
    while (end >= str_copy && isspace(*end))
    {
        end--;
    }

    // The end pointer now points to the last non-whitespace character in the string.
    // Add a null terminator after this character to terminate the string and drop all the whitespaces we just moved over backwards.
    *(end + 1) = '\0';

    // Move the str pointer forward until we encounter a non-whitespace character.
    while (*str_copy && isspace(*str_copy))
    {
        str_copy++;
    }

    // The str pointer now points to the first non-whitespace character in the string.
    return str_copy;
}

int convert_hex_char_to_int(char c)
{
    int result;
    if ('0' <= c && c <= '9')
    {
        result = c - '0';
    }
    else if ('a' <= c && c <= 'f')
    {
        result = c - 'a' + 10;
    }
    else if ('A' <= c && c <= 'F')
    {
        result = c - 'A' + 10;
    }
    else
    {
        result = -1;
    }
    return result;
}

char *string_unescape(const char *str)
{
    if (str == NULL)
    {
        fprintf(stderr, "The parameter \"str\" cannot be NULL\n");
        return NULL;
    }

    if (strlen(str) == 0)
    {
        fprintf(stderr, "The parameter \"str\" cannot be empty\n");
        return NULL;
    }

    char *str_copy = strdup(str);
    if (str_copy == NULL)
    {
        fprintf(stderr, "Unable to duplicate string\n");
        return NULL;
    }

    char *result = calloc(strlen(str), sizeof(char));

    while (*str_copy != '\0')
    {
        if (*str_copy == '\\')
        {
            str_copy++;
            switch (*str_copy)
            {
            case '\\':
                sprintf(result + strlen(result), "\\");
                break;
            case '"':
                sprintf(result + strlen(result), "\"");
                break;
            case 'x':
                int high = convert_hex_char_to_int(*(str_copy + 1));
                int low = convert_hex_char_to_int(*(str_copy + 2));
                if (high != -1 && low != -1)
                {
                    // Move the high character to the left and then append the low character to the right.
                    sprintf(result + strlen(result), "%c", (high << 4) | low);
                    // Move past the two character we've just handled.
                    str_copy += 2;
                }
                else
                {
                    // We encountered something that's not a valid hexadecimal string, so we just add it to the result so that it's preserved.
                    sprintf(result + strlen(result), "\\x");
                }
                break;
            }
        }
        else
        {
            sprintf(result + strlen(result), "%c", *str_copy);
        }
        str_copy++;
    }
    sprintf(result + strlen(result), "\0");

    return result;
}

char *string_escape(const char *str)
{
    if (str == NULL)
    {
        fprintf(stderr, "The parameter \"str\" cannot be NULL\n");
        return NULL;
    }

    if (strlen(str) == 0)
    {
        fprintf(stderr, "The parameter \"str\" cannot be empty\n");
        return NULL;
    }

    char *str_copy = strdup(str);
    if (str_copy == NULL)
    {
        fprintf(stderr, "Unable to duplicate string\n");
        return NULL;
    }

    char *result = calloc(strlen(str) * 2, sizeof(char));
    sprintf(result, "\"");

    while (*str_copy != '\0')
    {
        switch (*str_copy)
        {
        case '\\':
            sprintf(result + strlen(result), "\\\\");
            break;
        case '"':
            sprintf(result + strlen(result), "\\\"");
            break;
        case 'x':
            int high = convert_hex_char_to_int(*(str_copy + 1));
            int low = convert_hex_char_to_int(*(str_copy + 2));
            if(high != -1 && low != -1)
            {
                sprintf(result + strlen(result), "%c%c%c", *str_copy, *(str_copy + 1), *(str_copy + 2));
                str_copy += 2;
            }
            else
            {
                // We encountered something that's not a valid hexadecimal string, so we just add it to the result so that it's preserved.
                sprintf(result + strlen(result), "%c", *str_copy);
            }
            break;
        default:
            sprintf(result + strlen(result), "%c", *str_copy);
            break;
        }
        str_copy++;
    }
    sprintf(result + strlen(result), "\"\0");

    return result;
}

StringBuffer* string_buffer_create(size_t total_size) {
    StringBuffer* buffer = malloc(sizeof(StringBuffer));
    if(buffer == NULL) {
        debug_print("Failed to allocate memory for buffer.\n", "");
        return NULL;
    }
    
    buffer->total_size = total_size;
    buffer->actual_length = 0;
    buffer->content = calloc(buffer->total_size, sizeof(char));
    if(buffer->content == NULL) {
        debug_print("Failed to allocate memory for buffer content.\n", "");
        free(buffer);
        return NULL;
    }

    return buffer;;
}

bool string_buffer_realloc(StringBuffer* buffer, size_t additional_length) {
    if(buffer->actual_length + additional_length >= buffer->total_size) {
        size_t new_size = buffer->total_size * 2;
        while(new_size <= buffer->actual_length + additional_length) {
            new_size *= 2;
        }

        char *new_content = realloc(buffer->content, new_size);
        if(new_content == NULL) {
            debug_print("Failed to allocate additional memory for buffer content.\n", "");
            return false;
        }

        buffer->total_size = new_size;
        buffer->content = new_content;
    }

    return true;
}

bool string_buffer_append(StringBuffer* buffer, const char* str) {
    size_t length = strlen(str);
    if(!string_buffer_realloc(buffer, length)) {
        debug_print("Failed to reallocate additional memory for string buffer.\n", "");
        return false;
    }

    strcpy(buffer->content + buffer->actual_length, str);
    buffer->actual_length += length;
    
    return true;
}

size_t string_index_of(char *haystack, char needle) {
    char *first_needle = strchr(haystack, needle);
    size_t index = (size_t)(first_needle - haystack);
    return index;
}
char *string_substring(char *str, size_t inclusive_start, size_t exclusive_end)
{
    if (inclusive_start > strlen(str))
    {
        fprintf(stderr, "The starting position %zu exceeds the length %zu of the input string \"%s\"\n", inclusive_start, exclusive_end, str);
        return NULL;
    }

    size_t length = exclusive_end - inclusive_start;

    char *output = malloc(length + 1);
    if (output == NULL)
    {
        fprintf(stderr, "Unable to allocate memory for output\n");
        return NULL;
    }

    strncpy(output, str + inclusive_start, exclusive_end);
    output[length] = '\0';

    return output;
}

bool string_contains_non_overlapping_pair(const char *str) {
    size_t length = strlen(str);
    for(size_t i = 0; i < length; i++) {
        char pair[3] = { str[i], str[i + 1], '\0' };

        // Search the remainder of the string. The remainder starts at i + 2 because we've already stashed line[i] and line [i+1] in the "pair" variable.
        for(size_t j = i + 2; j < length; j++) {
            char next_pair[3] = { str[j], str[j + 1], '\0' };
            if(strcmp(pair, next_pair) == 0) {
                if(str[j - 1] == str[j] && str[j] == str[j + 1]) {
                    return false;
                }
                return true; 
            }
        }
    }
    return false;
}

bool string_has_straight_of_n(const char *str, int n) {
    size_t length = strlen(str);

    for(size_t i = 0; i < length - 2; i++) {
        if(str[i] == str[i + 1] - 1 && str[i] == str[i + 2] - 2) {
            return true;
        }
    }

    return false;
}

bool string_has_any_needle(const char *str, char *needles, size_t number_of_needles) {
    for(size_t i = 0; i < number_of_needles; i++) {
        char needle = needles[i];
        if(strchr(str, needle)) {
            return true;
        }
    }
    return false;
}
