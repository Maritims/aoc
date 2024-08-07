#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "string4c.h"

int string_compare_asc(const void *str1, const void *str2) {
    return strcmp(*(const char**)str1, *(const char**)str2);
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
    result[strlen(result)] = '\0';

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
    sprintf(result + strlen(result), "\"");
    result[strlen(result)] = '\0';

    return result;
}

StringBuffer* string_buffer_create(size_t total_size) {
    StringBuffer* buffer = malloc(sizeof(StringBuffer));
    if(buffer == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to allocate memory for buffer\n", __FILE__, __func__, __LINE__);
        return NULL;
    }
    
    buffer->total_size = total_size;
    buffer->actual_length = 0;
    buffer->content = calloc(buffer->total_size, sizeof(char));
    if(buffer->content == NULL) {
        fprintf(stderr, "%s:%d: Failed to allocate memory for buffer content.\n", __func__, __LINE__);
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
            fprintf(stderr, "%s:%d: Failed to allocate additional memory for buffer content.\n", __func__, __LINE__);
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
        fprintf(stderr, "%s:%d: Failed to reallocate additional memory for string buffer.\n", __func__, __LINE__);
        return false;
    }

    strcpy(buffer->content + buffer->actual_length, str);
    buffer->actual_length += length;
    
    return true;
}

void string_substring(char **result, char *str, size_t inclusive_start, size_t exclusive_end)
{
    if (inclusive_start > strlen(str))
    {
        fprintf(stderr, "The starting position %zu exceeds the length %zu of the input string \"%s\"\n", inclusive_start, exclusive_end, str);
        return;
    }

    size_t length = exclusive_end - inclusive_start;

    *result = malloc(length + 1);
    if (*result == NULL)
    {
        fprintf(stderr, "%s:%d: Failed allopcating memory for result: %s\n", __func__, __LINE__, strerror(errno));
        return;
    }

    strncpy(*result, str + inclusive_start, exclusive_end);
    (*result)[length] = '\0';
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
    char *slice = malloc(slice_len);
    
    if(slice == NULL) {
        fprintf(stderr, "%s:%d: failed to allocate memory for slice\n", __func__, __LINE__);
        return NULL;
    }

    strncpy(slice, str + start, slice_len);
    return slice;
}
