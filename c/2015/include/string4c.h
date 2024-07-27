#ifndef STRING4C
#define STRING4C

#include <stdbool.h>
#include <stdlib.h>

bool string_is_numeric(char *str);

void string_split(char ***result, size_t *result__size, char *str, char *delimiter);

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

bool string_contains_non_overlapping_pair(const char *str);

bool string_has_straight_of_n(const char *str, int n);

bool string_has_any_needle(const char *str, char *needles, size_t number_of_needles);

#endif
