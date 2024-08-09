#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "testing/assertions.h"
#include "string4c.h"

void test_string_from_number(uint64_t n, char *expected_result) {
    // assign, act
    char *result = string_from_number(n);

    // assert
    assert_string_equality(expected_result, result, "string_from_number(%lu) != %s (%s)\n", n, expected_result, result);
    printf("%s(%lu, \"%s\") passed\n", __func__, n, expected_result);
    free(result);
}

void test_string_is_numeric(char *str, bool expected_result) {
    // assign, act
    bool result = string_is_numeric(str);

    // assert
    assert_primitive_equality(expected_result, result, "string_is_numeric(\"%s\") != %d\n", str, expected_result);
    printf("%s(\"%s\") passed\n", __func__, str);
}

void test_string_split(char *str, char *delimiter, size_t expected_result_length, char **expected_result) {
    // assign
    size_t result_length = 0;

    // act
    char **result = string_split(&result_length, str, delimiter);

    // assert
    assert_primitive_equality(expected_result_length, result_length, "string_split(result_length, \"%s\", \"%s\") gave an unexpected result length: %zu\n", str, delimiter, result_length);

    for(size_t i = 0; i < result_length; i++) {
        assert_not_null(expected_result[i], "no element was found in the expected_result array at index %zu\n", i);
        assert_not_null(result[i], "no element was found in the result array at index %zu\n", i);
        assert_string_equality(expected_result[i], result[i], "\"%s\" at index %zu differs from the expected element, \"%s\"\n", result[i], i, expected_result[i]);
    }

    printf("%s(\"%s\", \"%s\", %zu) passed\n", __func__, str, delimiter, expected_result_length);
    for(size_t i = 0; i < result_length; i++) {
        free(result[i]);
    }
    free(result);
}

void test_string_buffer_append(const char *s1, const char *s2, const char *expected_result) {
    // assign
    string_buffer_t *sb = string_buffer_create(10);

    // act
    string_buffer_append(sb, s1);
    string_buffer_append(sb, s2);

    // assert
    assert_string_equality(expected_result, sb->content, "string_buffer_append(sb, \"%s\") resulted in unexpected content in string buffer. Expected %s, but got %s\n", s2, expected_result, sb->content); 
    printf("%s(\"%s\", \"%s\", \"%s\") passed\n", __func__, s1, s2, expected_result);
    string_buffer_destroy(sb);
}

void test_string_unescape(const char *str, const char *expected_result) {
    // assign, act
    char *result = string_unescape(str);

    // assert
    assert_string_equality(expected_result, result, "string_unescape(\"%s\") != %s (%s)", str, expected_result, result);
    printf("%s(\"%s\") passed\n", __func__, str);
    free(result);
}

void test_string_escape(const char *str, const char *expected_result) {
    // assign, act
    char *result = string_escape(str);

    // assert
    assert_string_equality(expected_result, result, "string_escape(\"%s\") != %s (%s)", str, expected_result, result);
    printf("%s(\"%s\") passed\n", __func__, str);
    free(result);
}

void test_string_trim(char *str, char *expected_result) {
    // assign, act
    char *result = string_trim(str);

    // assert
    assert_string_equality(expected_result, result, "string_trim(\"%s\") != %s\n", str, expected_result);
    printf("%s(\"%s\", \"%s\") passed\n", __func__, str, expected_result);
    free(result);
}

void test_string_contains_non_overlapping_pair(const char *str, bool expected_result) {
    bool result = string_contains_non_overlapping_pair(str);
    assert_primitive_equality(result, expected_result, "expected %d, but got %d\n", expected_result, result);
    printf("%s(\"%s\") passed\n", __func__, str);
}

void test_string_has_straight_of_n(const char *str, int n, bool expected_result) {
    bool result = string_has_straight(str);
    assert_primitive_equality(result, expected_result, "expected %d, but got %d\n", expected_result, result);
    printf("%s(\"%s\", %d) passed\n", __func__, str, n);
}

void test_string_replace(const char *str, const char *old_str, const char *new_str, const char *expected) {
    char *result = strdup(str);
    result = string_replace(result, old_str, new_str);

    assert_string_equality(expected, result, "string_replace(\"%s\", \"%s\", \"%s\") != %s (%s)\n", result, old_str, new_str, expected, result);

    free(result);
    printf("%s(\"%s\", \"%s\", \"%s\") passed\n", __func__, str, old_str, new_str);
}

void test_string_replace_all(const char *str, const char *old_str, const char *new_str, const char *expected) {
    char *result = strdup(str);
    result = string_replace_all(result, old_str, new_str);

    assert_string_equality(expected, result, "string_replace_all(\"%s\", \"%s\", \"%s\") != %s (%s)\n", str, old_str, new_str, expected, result);

    free(result);
    printf("%s(\"%s\", \"%s\", \"%s\") passed\n", __func__, str, old_str, new_str);
}

void test_string_replace_at(const char *str, const char *new_str, size_t pos, size_t length, char *expected) {
    char *result = string_replace_at(str, new_str, pos, length);
    assert_string_equality(expected, result, "string_replace_at(\"%s\", \"%s\", %zu, %zu) != %s (%s)\n", str, new_str, pos, length, expected, result);
    printf("%s(\"%s\", \"%s\", %zu, %zu) passed\n", __func__, str, new_str, pos, length);
}

void test_string_slice(const char *str, size_t start, size_t end, char *expected) {
    // act, assign
    char *slice = string_slice(str, start, end);

    // assert
    assert_string_equality(expected, slice, "string_slice(\"%s\", %zu, %zu) != %s (was %s)\n", str, start, end, expected, slice);
    printf("%s(\"%s\", %zu, %zu, \"%s\") passed\n", __func__, str, start, end, expected);
    free(slice);
}

int main() {
    test_string_from_number(0, "0");
    test_string_from_number(1, "1");
    test_string_from_number(12398, "12398");

    test_string_is_numeric("foo", false);
    test_string_is_numeric("foo123", false);
    test_string_is_numeric("123", true);
    
    test_string_split("he ll o wo rld", " ", 5, (char *[]){
        (char*){ "he" },
        (char*){ "ll" },
        (char*){ "o" },
        (char*){ "wo" },
        (char*){ "rld" }
    });
   
    test_string_buffer_append("foo", "bar", "foobar");

    test_string_unescape("\\\"\\\"", "\"\"");
    test_string_unescape("\\\"abc\\\"", "\"abc\"");
    test_string_unescape("aaa\\\"aaa", "aaa\"aaa");
    test_string_unescape("\\x27", "'");

    test_string_escape("\"\"", "\"\\\"\\\"\"");
    test_string_escape("\"abc\"", "\"\\\"abc\\\"\"");
    test_string_escape("\"aaa\\\"aaa\"", "\"\\\"aaa\\\\\\\"aaa\\\"\"");
    test_string_escape("\"\\x27\"", "\"\\\"\\\\x27\\\"\"");
    
    test_string_split("foobar", "", 0, (char *[]){});
    
    test_string_trim("foobar", "foobar");
    test_string_trim(" foobar", "foobar");
    test_string_trim("  foobar", "foobar");
    test_string_trim("foobar ", "foobar");
    test_string_trim("foobar  ", "foobar");
    test_string_trim("foo bar", "foo bar");
    test_string_trim("foo bar ", "foo bar");
    
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
