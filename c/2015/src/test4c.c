#include <stdbool.h>

#include "test4c.h"

void test_string_string(const char *name_of_function_to_test, char *input, const char *expected_result, char* (*function_to_test)(char *input)) {
	char *output = function_to_test(input);
	bool success = strcmp(output, expected_result) == 0;
    printf("%s(%s): %s - expected: %s, received: %s\n", name_of_function_to_test, input, success ? "OK" : "Not OK", expected_result, output);
}

void test_string_boolean(const char *name_of_function_to_test, char *input, bool expected_result, bool (*function_to_test)(char *input)) {
	bool output = function_to_test(input);
	bool success = output == expected_result;
    printf("%s(%s): %s - expected: %d, received: %d\n", name_of_function_to_test, input, success ? "OK" : "Not OK", expected_result, output);
}

void test_string_int(const char *name_of_function_to_test, char *input, int expected_result, int (*function_to_test)(char *input)) {
    int output = function_to_test(input);
    bool success = output == expected_result;
    printf("%s(%s): %s - expected: %d, received: %d\n", name_of_function_to_test, input, success ? "OK" : "Not OK", expected_result, output);
}