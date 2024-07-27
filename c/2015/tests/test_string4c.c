#include <stdbool.h>
#include <stdio.h>

#include "string4c.h"

void test_string_contains_non_overlapping_pair(const char *str, bool expected_result) {
    if(string_contains_non_overlapping_pair(str) != expected_result) {
        printf("%s failed: string_contains_non_overlapping_pair(\"%s\") != %d\n", __func__, str, expected_result);
        exit(EXIT_FAILURE);
    }

    printf("%s(\"%s\") passed\n", __func__, str);
}

void test_string_has_straight_of_n(const char *str, int n, bool expected_result) {
    if(string_has_straight_of_n(str, n) != expected_result) {
        printf("%s failed: string_has_straight_of_n(\"%s\", %d) != %d\n", __func__, str, n, expected_result);
        exit(EXIT_FAILURE);
    }

    printf("%s(\"%s\", %d) passed\n", __func__, str, n);
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

    printf("All tests passed\n");
    return EXIT_SUCCESS;
}
