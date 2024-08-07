#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "string4c.h"

void test_string_contains_non_overlapping_pair(const char *str, bool expected_result) {
    if(string_contains_non_overlapping_pair(str) != expected_result) {
        printf("%s failed: string_contains_non_overlapping_pair(\"%s\") != %d\n", __func__, str, expected_result);
        exit(EXIT_FAILURE);
    }

    printf("%s(\"%s\") passed\n", __func__, str);
}

void test_string_has_straight_of_n(const char *str, int n, bool expected_result) {
    if(string_has_straight(str) != expected_result) {
        printf("%s failed: string_has_straight_of_n(\"%s\", %d) != %d\n", __func__, str, n, expected_result);
        exit(EXIT_FAILURE);
    }

    printf("%s(\"%s\", %d) passed\n", __func__, str, n);
}

void test_string_replace(const char *str, const char *old_str, const char *new_str, const char *expected) {
    char *result = strdup(str);
    result = string_replace(result, old_str, new_str);  
 
    if(strcmp(expected, result) != 0) {
        printf("%s failed: string_replace(\"%s\", \"%s\", \"%s\") != %s (%s)\n", __func__, result, old_str, new_str, expected, result);
        exit(EXIT_FAILURE);
    }

    free(result);
    printf("%s(\"%s\", \"%s\", \"%s\") passed\n", __func__, str, old_str, new_str);
}

void test_string_replace_all(const char *str, const char *old_str, const char *new_str, const char *expected) {
    char *result = strdup(str);
    result = string_replace_all(result, old_str, new_str);

    if(strcmp(expected, result) != 0) {
        printf("%s failed: string_replace_all(\"%s\", \"%s\", \"%s\") != %s (%s)\n", __func__, str, old_str, new_str, expected, result);
        exit(EXIT_FAILURE);
    }

    free(result);
    printf("%s(\"%s\", \"%s\", \"%s\") passed\n", __func__, str, old_str, new_str);
}

void test_string_replace_at(const char *str, const char *new_str, size_t pos, size_t length, char *expected) {
    char *result = string_replace_at(str, new_str, pos, length);
    if(strcmp(expected, result) != 0) {
        printf("%s failed: string_replace_at(\"%s\", \"%s\", %zu, %zu) != %s (%s)\n", __func__, str, new_str, pos, length, expected, result);
        exit(EXIT_FAILURE);
    }
    printf("%s(\"%s\", \"%s\", %zu, %zu) passed\n", __func__, str, new_str, pos, length);
}

void test_string_slice(const char *str, size_t start, size_t end, char *expected) {
    char *slice = string_slice(str, start, end);
    if(strcmp(slice, expected) != 0) {
        printf("%s failed: string_slice(\"%s\", %zu, %zu) != %s (was %s)\n", __func__, str, start, end, expected, slice);
        exit(EXIT_FAILURE);
    }

    printf("%s(\"%s\", %zu, %zu, \"%s\") passed\n", __func__, str, start, end, expected);
}

int main() {
    test_string_contains_non_overlapping_pair("aabaa", true);
    test_string_contains_non_overlapping_pair("cqcq", true);
    test_string_contains_non_overlapping_pair("aa", false);
    test_string_contains_non_overlapping_pair("aaa", false);
    test_string_contains_non_overlapping_pair("aaaa", false);

    test_string_has_straight_of_n("hijklmmn", 3, true);
    test_string_has_straight_of_n("abbceffg", 3, false);
    test_string_has_straight_of_n("abcdffaa", 3, true);
    test_string_has_straight_of_n("ghjaabcc", 3, true);

    test_string_replace("lorem ipsum, lorem ipsum", "lorem", "hello", "hello ipsum, lorem ipsum");
    test_string_replace("Hello World!", "Hello", "Hi", "Hi World!");
    test_string_replace("foobarbaz", "foo", "lorem", "lorembarbaz");
    test_string_replace_all("foobarbazfoobarbaz", "foo", "loremipsum", "loremipsumbarbazloremipsumbarbaz");
/*    test_string_replace_at("Hello World!", "Farewell", 0, 5, "Farewell World!");
    test_string_replace_at("Lorem Ipsum", "Dolor", 6, 1, "Lorem Dolorpsum");
    test_string_replace_at("HOH", "HO", 0, 1, "HOOH");
    test_string_replace_at("HOH", "HO", 2, 1, "HOHO");
    test_string_replace_at("HOH", "OH", 0, 1, "OHOH");
    test_string_replace_at("HOH", "OH", 2, 1, "HOOH");
    test_string_replace_at("HOH", "HH", 1, 1, "HHHH");*/

    test_string_slice("foo bar baz", 4, 7, "bar");

    printf("All tests passed\n");
    return EXIT_SUCCESS;
}
