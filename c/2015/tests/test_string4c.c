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

int main() {
    test_string_contains_non_overlapping_pair("aabaa", true);
    test_string_contains_non_overlapping_pair("cqcq", true);
    test_string_contains_non_overlapping_pair("aa", false);
    test_string_contains_non_overlapping_pair("aaa", false);
    test_string_contains_non_overlapping_pair("aaaa", false);

    printf("All tests passed\n");
    return EXIT_SUCCESS;
}
