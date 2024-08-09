#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "math4c.h"
#include "string4c.h"

int string_compare_asc(const void *str1, const void *str2) {
    return strcmp(*(const char**)str1, *(const char**)str2);
}

char *string_from_number(int n) {
    int length = math_number_places(n);
    char *result = malloc(length + 1);
    sprintf(result, "%d", n);
    result[length] = '\0';
    return result;
}

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

char **string_split(size_t *out_result_length, char *input, char *delimiter) {
    if(strlen(delimiter) == 0) {
        fprintf(stderr, "%s:%s:%d: delimiter was empty\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    size_t input_length = strlen(input);
    char *buffer        = malloc(input_length + 1);
    strcpy(buffer, input);
    buffer[input_length] = '\0';

    size_t result_capacity  = 10;
    size_t result_length    = 0;
    char **result           = calloc(result_capacity, sizeof(char *));

    if (result == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to allocate memory for result: %s\n", __FILE__, __func__, __LINE__, strerror(errno));
        free(buffer);
        return NULL;
    }

    char *save_ptr;
    char *token = strtok_r(buffer, delimiter, &save_ptr);

    while (token != NULL) {
        if (result_length >= result_capacity) {
            size_t new_result_capacity = result_capacity * 2;
            char **new_result = realloc(result, new_result_capacity * sizeof(char *));

            if (new_result == NULL)
            {
                fprintf(stderr, "%s:%d: failed to allocate additional memory for result: %s\n", __func__, __LINE__, strerror(errno));
                free(buffer);
                for (size_t i = 0; i < result_length; ++i)
                {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }

            result_capacity = new_result_capacity;
            result = new_result;
        }

        if (strcmp(token, delimiter) == 0) {
            token = strtok_r(NULL, delimiter, &save_ptr);
            continue;
        }

        size_t token_length = strlen(token);
        result[result_length] = malloc(token_length + 1);
        strcpy(result[result_length], token);
        result[result_length][token_length] = '\0';

        token = strtok_r(NULL, delimiter, &save_ptr);
        result_length++;
    }

    *out_result_length = result_length;
    free(buffer);
    return result;
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
    char *original_str_copy = str_copy;

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
    size_t result_length = strlen(str_copy);
    char *result = malloc(result_length + 1);
    strcpy(result, str_copy);
    result[result_length] = '\0';
    free(original_str_copy);
    return result;
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
    size_t len          = strlen(str);
    string_buffer_t *sb = string_buffer_create(len);
    int high            = 0;
    int low             = 0;

    for(size_t i = 0; i < len; i++) {
        if(str[i] == '\\') {
            i++;
            switch(str[i]) {
                case '\\':
                    string_buffer_append(sb, "\\");
                break;
                case '"':
                    string_buffer_append(sb, "\"");
                break;
                case 'x':
                    high = convert_hex_char_to_int(str[i + 1]);
                    low = convert_hex_char_to_int(str[i + 2]);
                    if (high != -1 && low != -1) {
                        // Move the high character to the left and then append the low character to the right.
                        char c[2];
                        sprintf(c, "%c", (high << 4) | low);
                        c[1] = '\0';
                        string_buffer_append(sb, c);
                        // Move past the two character we've just handled.
                        i += 2;
                    }
                    else {
                        // We encountered something that's not a valid hexadecimal string, so we just add it to the result so that it's preserved.
                        string_buffer_append(sb, "\\x");
                    }
                break;
            }
        }
        else {
            char c[2];
            sprintf(c, "%c", str[i]);
            c[1] = '\0';
            string_buffer_append(sb, c);
        }
    }

    char *result = malloc(sb->length + 1);
    strcpy(result, sb->content);
    result[sb->length] = '\0';
    string_buffer_destroy(sb);
    return result;
}

char *string_escape(const char *str) {
    size_t len          = strlen(str);
    string_buffer_t *sb = string_buffer_create(len);
    int high            = 0;
    int low             = 0;

    string_buffer_append(sb, "\"");

    for(size_t i = 0; i < len; i++) {
        switch(str[i]) {
            case '\\':
                string_buffer_append(sb, "\\\\");
            break;
            case '"':
                string_buffer_append(sb, "\\\"");
            break;
            case 'x':
                high = convert_hex_char_to_int(str[i + 1]);
                low = convert_hex_char_to_int(str[i + 2]);
                if(high != -1 && low != -1) {
                    char c[4];
                    sprintf(c, "%c%c%c", str[i], str[i + 1], str[i + 2]);
                    c[3] = '\0';
                    string_buffer_append(sb, c);
                    i += 2;
                }
                else {
                    // We encountered something that's not a valid hexadecimal string, so we just add it to the result so that it's preserved.
                    char c[2];
                    sprintf(c, "%c", str[i]);
                    c[1] = '\0';
                    string_buffer_append(sb, c);
                }
            break;
            default:
                string_buffer_append(sb, (char[2]){ str[i], '\0' });
            break;
        }
    }

    string_buffer_append(sb, "\"");

    char *result = malloc(sb->length + 1);
    strcpy(result, sb->content);
    result[sb->length] = '\0';
    string_buffer_destroy(sb);
    return result;
}

string_buffer_t *string_buffer_create(size_t capacity) {
    string_buffer_t *buffer = malloc(sizeof(string_buffer_t));
    if(buffer == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to allocate memory for buffer\n", __FILE__, __func__, __LINE__);
        return NULL;
    }
    
    buffer->capacity    = capacity;
    buffer->length      = 0;
    buffer->content     = malloc(buffer->capacity);
    if(buffer->content == NULL) {
        fprintf(stderr, "%s:%d: failed to allocate memory for buffer content.\n", __func__, __LINE__);
        free(buffer);
        return NULL;
    }

    return buffer;
}

void string_buffer_destroy(string_buffer_t *buffer) {
    free(buffer->content);
    free(buffer);
}

bool string_buffer_realloc(string_buffer_t *buffer, size_t additional_length) {
    size_t new_length = buffer->length + additional_length + 1;

    if(new_length > buffer->capacity) {
        size_t new_capacity = buffer->capacity * 2;
        while(new_capacity <= buffer->capacity + new_length) {
            new_capacity *= 2;
        }

        char *new_content   = realloc(buffer->content, new_capacity);
        if(new_content == NULL) {
            fprintf(stderr, "%s:%d: failed to allocate additional memory for buffer content.\n", __func__, __LINE__);
            return false;
        }

        buffer->capacity    = new_capacity;
        buffer->content     = new_content;
    }

    return true;
}

bool string_buffer_append(string_buffer_t *buffer, const char *str) {
    size_t length = strlen(str);
    if(!string_buffer_realloc(buffer, length)) {
        fprintf(stderr, "%s:%d: Failed to reallocate additional memory for string buffer.\n", __func__, __LINE__);
        return false;
    }

    strcpy(buffer->content + buffer->length, str);
    buffer->length += length;
    buffer->content[buffer->length] = '\0';
    
    return true;
}

char *string_substring(char *input, size_t inclusive_start, size_t exclusive_end)
{
    if (inclusive_start > strlen(input))
    {
        fprintf(stderr, "The starting position %zu exceeds the length %zu of the input string \"%s\"\n", inclusive_start, exclusive_end, input);
        return NULL;
    }

    size_t result_length = exclusive_end - inclusive_start;
    char *result = malloc(result_length + 1);
    if (result == NULL)
    {
        fprintf(stderr, "%s:%d: Failed allopcating memory for result: %s\n", __func__, __LINE__, strerror(errno));
        return NULL;
    }

    strncpy(result, input + inclusive_start, exclusive_end);
    result[result_length] = '\0';
    return result;
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

bool string_has_straight(const char *str) {
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

char *string_replace(char *str, const char *old_str, const char *new_str) {
    if(!str || !old_str || !new_str) {
        return str;
    }

    char *ptr = strstr(str, old_str);
    if(!ptr) {
        return str;
    }

    size_t str_length       = strlen(str);
    size_t old_length       = strlen(old_str);
    size_t new_length       = strlen(new_str);

    if(new_length > old_length) {
        size_t result_length    = str_length + (new_length - old_length);
        char *temp              = malloc(result_length + 1); // +1 for \0

        if(!temp) {
            fprintf(stderr, "%s:%d: Failed to allocate memory for temporary string\n", __func__, __LINE__);
            return str;
        }

        if(ptr != str) {
            memcpy(temp, str, ptr - str);  // Write all of str up until the occurrence of old_str into the temp string.
        }
        
        temp[ptr - str] = '\0';         // Clear the temp string of anything after the occurrence of old_str.
        strcat(temp, new_str);          // Add new_str to the temp string.
        strcat(temp, ptr + old_length); // Add old_str to the temp string.
        
        char *result = realloc(str, result_length + 1);
        if(!result) {
            fprintf(stderr, "%s:%d: Failed to allocate additional memory for resulting string\n", __func__, __LINE__);
            free(temp);
            return str;
        }

        str = result;
        strcpy(str, temp);              // Move the temp string into the original string.
        free(temp);
    }
    else {
        if(old_length != new_length) {
            memmove(ptr + new_length, ptr + old_length, strlen(ptr + old_length) + 1);
        }
        memcpy(ptr, new_str, new_length);
    }

    return str;
}

char *string_replace_all(char *str, const char *old_str, const char *new_str) {
    if(!str || !old_str || !new_str) {
        return str;
    }

    while(strstr(str, old_str) != NULL) {
        str = string_replace(str, old_str, new_str);
    }

    return str;
}

char *string_replace_at(const char *str, const char *new_str, size_t pos, size_t length) {
    size_t str_length = strlen(str);
    size_t new_length = strlen(new_str);
    size_t result_length    = str_length - length + new_length;
    char *result            = malloc(result_length);
    strcpy(result, str);

    if(length != new_length) {
        memmove(result + pos + new_length, result + pos + length, strlen(result + pos + length) + 1);
    }
    memcpy(result + pos, new_str, new_length);
    return result;
}

char *string_prepend_char(char **str, const char c) {
    size_t length   = strlen(*str);
    char *new_str   = malloc(length + 2); // Add room for the new character and the NULL terminator.
    new_str[0]      = c;
    memcpy(new_str + 1, *str, length);
    new_str[length + 1] = '\0';
    *str = new_str;
    return *str;
}

char *string_slice(const char *str, size_t start, size_t end) {
    if(str == NULL) {
        fprintf(stderr, "%s:%d: str is NULL\n", __func__, __LINE__);
        return NULL;
    }

    size_t len = strlen(str);

    if(len == 0) {
        return NULL;
    }

    if(start == end) {
        return NULL;
    }
    
    if(start >= len) {
        fprintf(stderr, "%s:%d: invalid starting position (%zu)\n", __func__, __LINE__, start);
        return NULL;
    }

    if(end >= len) {
        fprintf(stderr, "%s:%d: invalid ending position (%zu)\n", __func__, __LINE__, end);
        return NULL;
    }

    size_t slice_len = end - start;
    char *slice = malloc(slice_len + 1);
    
    if(slice == NULL) {
        fprintf(stderr, "%s:%d: failed to allocate memory for slice\n", __func__, __LINE__);
        return NULL;
    }

    strncpy(slice, str + start, slice_len);
    slice[slice_len] = '\0';
    return slice;
}
