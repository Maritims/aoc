#ifndef STRING4C
#define STRING4C

#include <stdbool.h>
#include <stdlib.h>

bool string_is_numeric(char *str);

char **string_split(char *str, char *delimiter, size_t *out_size);

char *string_trim(char *str);

char *string_unescape(const char *str);

char *string_escape(const char *str);

typedef struct StringBuffer {
    size_t total_size;
    size_t actual_length;
    char* content;
} StringBuffer;

StringBuffer *string_buffer_create(size_t total_size);

bool string_buffer_realloc(StringBuffer *buffer, size_t additional_length);

bool string_buffer_append(StringBuffer *buffer, const char *str);

size_t string_index_of(char *haystack, char needle);

char *string_substring(char *str, size_t inclusive_start, size_t exclusive_end);

#endif
