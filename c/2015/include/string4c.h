#ifndef STRING4C
#define STRING4C

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct string_buffer_t {
    size_t  capacity;
    size_t  length;
    char*   content;
} string_buffer_t;

/**
 * string_compare_asc: Comparator function for use with qsort.
 * param str1 The first string.
 * param str2 The second string.
 * return Returns the result of strcmp(str1, str2).
 */
int string_compare_asc(const void *str1, const void *str2);

char *string_from_number(int n);

bool string_is_numeric(char *str);

char **string_split(size_t *result_length, char *input, char *delimiter);

char *string_trim(char *str);

char *string_unescape(const char *str);

char *string_escape(const char *str);

string_buffer_t *string_buffer_create(size_t total_size);

bool string_buffer_realloc(string_buffer_t *buffer, size_t additional_length);

bool string_buffer_append(string_buffer_t *buffer, const char *str);

char *string_substring(char *input, size_t inclusive_start, size_t exclusive_end);

bool string_contains_non_overlapping_pair(const char *str);

bool string_has_straight(const char *str);

bool string_has_any_needle(const char *str, char *needles, size_t number_of_needles);

/**
 * string_replace: Replace the first occurrence of a string with another string.
 * @param str The original string.
 * @param old_str The string to replace.
 * @param new_str The replacement string.
 * @return Returns the string with the replacement.
 */
char *string_replace(char *str, const char *old_str, const char *new_str);

/**
 * string_replace_all: Replace all occurrences of a string with another string.
 * @param str The original string.
 * @param old_str The string to replace.
 * @param new_str The replacement string.
 * @return Returns the string with replacements.
 */
char *string_replace_all(char *str, const char *old_str, const char *new_str);

/**
 * string_replace_at: Replace a string at a specific position with another string.
 * @param str The original string.
 * @param new_str The replacement string.
 * @param pos The starting position for the string to be replaced.
 * @param length The amount of data to replace. Memory will be shifted if necessary to accommodate for the replacement.
 * @return Returns a copy of the original string.
 *
 * @param length The amount of data to replace. Memory will be shifted if necessary to accommodate for the replacement.
 */
char *string_replace_at(const char *str, const char *new_str, size_t pos, size_t length);

/**
 * string_prepend: Prepend a character to a string.
 * @param str A pointer to the string.
 * @param c The character.
 * @return A pointer to the modified string.
 */
char *string_prepend_char(char **str, const char c);

/**
 * string_slice: Get a slice of a string.
 * @param start Where to slice from, inclusive.
 * @param end How far to slice, exclusive.
 * @return The slice.
 */
char *string_slice(const char *str, size_t start, size_t end);

#endif
