#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct TestResults {
	int total;
	int succeeded;
} TestResults;

#define TestCaseForInputWithExpectation(TInput, TExpectedResult)	\
	struct {								\
		TInput input;						\
		TExpectedResult expectedResult;		\
	}

void test_string_string(const char *name_of_function_to_test, char *input, const char *expected_result, char* (*function_to_test)(char *input));

void test_string_boolean(const char *name_of_function_to_test, char *input, bool expected_result, bool (*function_to_test)(char *input));

void test_string_int(const char *name_of_function_to_test, char *input, int expected_result, int (*function_to_test)(char *input));