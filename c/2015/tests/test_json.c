#include "json/token.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "generics/array.h"
#include "generics/object.h"
#include "generics/types.h"
#include "generics/value.h"
#include "json/lexer.h"
#include "json/parser.h"

void assert_size_and_type(const char *test_name, generic_value_t *value, generic_value_type_t expected_type, size_t expected_size) {
    if(value == NULL) {
        printf("%s failed: value was null\n", test_name);
        exit(EXIT_FAILURE);
    }

    generic_value_type_t type = generic_value_get_type(value);
    if(type != expected_type) {
        printf("%s failed: type != %d (%d)\n", test_name, expected_type, type);
        exit(EXIT_FAILURE);
    }

    generic_array_t *array;
    generic_object_t *object;
    size_t size;

    switch(expected_type) {
        case GENERIC_VALUE_TYPE_ARRAY:
            array   = generic_value_get_array(value);
            size    = generic_array_get_size(array);
            break;
        case GENERIC_VALUE_TYPE_OBJECT:
            object  = generic_value_get_object(value);
            size    = generic_object_get_size(object);
            break;
        default:
            printf("%s failed: %s received an unexpected type (%d)\n", test_name, __func__, expected_type);
            exit(EXIT_FAILURE);
            break;
    }
    if(size != expected_size) {
        printf("%s failed: size != %zu (%zu)\n", test_name, expected_size, size);
        exit(EXIT_FAILURE);
    }
}

void free_tokens(json_token_t **tokens, size_t length) {
    for(size_t i = 0; i < length; i++) {
        json_token_destroy(tokens[i]);
    }
    free(tokens);
}

void test_empty_object() {
    size_t number_of_tokens = 0;
    json_token_t **tokens   = json_lex("{}", &number_of_tokens);
    //json_token_t **start    = tokens;
    generic_value_t *value  = json_parse(&tokens, true);

    assert_size_and_type(__func__, value, GENERIC_VALUE_TYPE_OBJECT, 0);

    printf("%s passed\n", __func__);
    generic_value_destroy(value);
    //free_tokens(start, number_of_tokens);
}

void test_basic_object() {
    size_t number_of_tokens = 0;
    json_token_t **tokens   = json_lex("{\"foo\": \"bar\"}", &number_of_tokens);
    //json_token_t **start    = tokens;
    generic_value_t *value  = json_parse(&tokens, true);

    assert_size_and_type(__func__, value, GENERIC_VALUE_TYPE_OBJECT, 1);

    generic_object_t *object    = generic_value_get_object(value);
    generic_value_t *foo        = generic_object_get(object, "foo");
    if(foo == NULL) {
        printf("%s failed: generic_object_get(object, \"foo\") == NULL\n", __func__);
        exit(EXIT_FAILURE);
    }

    char *bar = generic_value_get_string(foo);
    if(bar == NULL) {
        printf("%s failed: string value was null\n", __func__);
        exit(EXIT_FAILURE);
    }

    if(strcmp(bar, "bar") != 0) {
        printf("%s failed: string value != bar (%s)\n", __func__, bar);
        exit(EXIT_FAILURE);
    }

    printf("%s passed\n", __func__);
    generic_value_destroy(value);
    //free_tokens(start, number_of_tokens);
}

void test_basic_object_with_single_character_property() {
    size_t number_of_tokens = 0;
    json_token_t **tokens   = json_lex("{\"f\": \"b\"}", &number_of_tokens);
    generic_value_t *value  = json_parse(&tokens, true);

    assert_size_and_type(__func__, value, GENERIC_VALUE_TYPE_OBJECT, 1);

    generic_object_t *object    = generic_value_get_object(value);
    generic_value_t *f          = generic_object_get(object, "f");
    if(f == NULL) {
        printf("%s failed: generic_object_get(object, \"f\") == NULL\n", __func__);
        exit(EXIT_FAILURE);
    }

    char *b = generic_value_get_string(f);
    if(b == NULL) {
        printf("%s failed: string value was null\n", __func__);
        exit(EXIT_FAILURE);
    }

    if(strcmp(b, "b") != 0) {
        printf("%s failed: string value != b (%s)\n", __func__, b);
        exit(EXIT_FAILURE);
    }

    printf("%s passed\n", __func__);
    generic_value_destroy(value);
}

void test_number_object() {
    size_t number_of_tokens = 0;
    json_token_t **tokens   = json_lex("{\"foo\": 1}", &number_of_tokens);
    generic_value_t *value  = json_parse(&tokens, true);

    assert_size_and_type(__func__, value, GENERIC_VALUE_TYPE_OBJECT, 1);

    generic_object_t *object    = generic_value_get_object(value);
    generic_value_t *foo        = generic_object_get(object, "foo");
    if(foo == NULL) {
        printf("%s failed: generic_object_get(object, \"foo\") == NULL\n", __func__);
        exit(EXIT_FAILURE);
    }

    int number = generic_value_get_int(foo);
    if(number != 1) {
        printf("%s failed: int value != 1 (%d)\n", __func__, number);
        exit(EXIT_FAILURE);
    }

    printf("%s passed\n", __func__);
    generic_value_destroy(value);
}

void test_empty_array() {
    size_t number_of_tokens = 0;
    json_token_t **tokens   = json_lex("{\"foo\": []}", &number_of_tokens);
    generic_value_t *value  = json_parse(&tokens, true);

    assert_size_and_type(__func__, value, GENERIC_VALUE_TYPE_OBJECT, 1);

    generic_object_t *object    = generic_value_get_object(value);
    generic_value_t *foo        = generic_object_get(object, "foo");

    assert_size_and_type(__func__, foo, GENERIC_VALUE_TYPE_ARRAY, 0);

    printf("%s() passed\n", __func__);
    generic_value_destroy(value);
}

void test_basic_array() {
    size_t number_of_tokens = 0;
    json_token_t **tokens   = json_lex("{\"foo\": [1,2,\"three\"]}", &number_of_tokens);
    generic_value_t *value  = json_parse(&tokens, true);

    assert_size_and_type(__func__, value, GENERIC_VALUE_TYPE_OBJECT, 1);

    generic_object_t *object    = generic_value_get_object(value);
    generic_value_t *foo        = generic_object_get(object, "foo");

    assert_size_and_type(__func__, foo, GENERIC_VALUE_TYPE_ARRAY, 3);

    printf("%s passed\n", __func__);
    generic_value_destroy(value);
}

void test_nested_object() {
    size_t number_of_tokens = 0;
    json_token_t **tokens   = json_lex("{\"foo\": {\"bar\": 2}}", &number_of_tokens);
    generic_value_t *value  = json_parse(&tokens, true);

    assert_size_and_type(__func__, value, GENERIC_VALUE_TYPE_OBJECT, 1);

    generic_object_t *object        = generic_value_get_object(value);
    generic_value_t *foo            = generic_object_get(object, "foo");

    assert_size_and_type(__func__, foo, GENERIC_VALUE_TYPE_OBJECT, 1);

    generic_object_t *foo_object    = generic_value_get_object(foo);
    generic_value_t *bar            = generic_object_get(foo_object, "bar");
    int number                      = generic_value_get_int(bar);
    if(number != 2) {
        printf("%s failed: number != 2 (%d)\n", __func__, number);
        exit(EXIT_FAILURE);
    }
    
    printf("%s passed\n", __func__);
    generic_value_destroy(value);
}

void test_bool(char *bool_value, bool expected) {
    char json_string[1024];
    sprintf(json_string, "{\"foo\": %s}", bool_value);
    size_t number_of_tokens = 0;
    json_token_t **tokens   = json_lex(json_string, &number_of_tokens);
    generic_value_t *value  = json_parse(&tokens, true);

    assert_size_and_type(__func__, value, GENERIC_VALUE_TYPE_OBJECT, 1);

    generic_object_t *object    = generic_value_get_object(value);
    generic_value_t *foo        = generic_object_get(object, "foo");
    bool foo_value              = generic_value_get_bool(foo);

    if(foo_value != expected) {
        printf("%s(\"%s\") failed: generic_value_get_bool(foo) != %d\n", __func__, bool_value, expected);
        exit(EXIT_FAILURE);
    }

    printf("%s passed\n", __func__);
    generic_value_destroy(value);
}

void test_null() {
    size_t number_of_tokens = 0;
    json_token_t **tokens   = json_lex("{\"foo\": null}", &number_of_tokens);
    generic_value_t *value  = json_parse(&tokens, true);

    assert_size_and_type(__func__, value, GENERIC_VALUE_TYPE_OBJECT, 1);

    generic_object_t *object    = generic_value_get_object(value);
    generic_value_t *foo        = generic_object_get(object, "foo");

    if(foo != NULL) {
        printf("%s failed: foo wasn't null\n", __func__);
        exit(EXIT_FAILURE);
    }

    printf("%s passed\n", __func__);
    generic_value_destroy(value);
}

void test_object_with_multiple_properties() {
    size_t number_of_tokens = 0;
    json_token_t **tokens   = json_lex("{\"e\": \"green\", \"a\": 77}", &number_of_tokens);
    generic_value_t *value  = json_parse(&tokens, true);

    assert_size_and_type(__func__, value, GENERIC_VALUE_TYPE_OBJECT, 2);

    generic_object_t *object    = generic_value_get_object(value);
    generic_value_t *e          = generic_object_get(object, "e");
    char *e_value               = generic_value_get_string(e);
    
    if(e_value == NULL) {
        printf("%s failed: e was null\n", __func__);
        exit(EXIT_FAILURE);
    }
    
    if(strcmp(e_value, "green") != 0) {
        printf("%s failed: e != green (%s)\n", __func__, e_value);
        exit(EXIT_FAILURE);
    }

    generic_value_t *a  = generic_object_get(object, "a");
    int a_value         = generic_value_get_int(a);
    if(a_value != 77) {
        printf("%s failed: a != 77 (%d)\n", __func__, a_value);
        exit(EXIT_FAILURE);
    }

    printf("%s passed\n", __func__);
    generic_value_destroy(value);
}

int main(void) {
    test_empty_object();
    test_basic_object();
    test_basic_object_with_single_character_property();
    test_number_object();
    test_empty_array();
    test_basic_array();
    test_nested_object();
    test_bool("true", true);
    test_bool("false", false);
    test_null();
    test_object_with_multiple_properties();

    printf("All tests passed\n");
    return EXIT_SUCCESS;
}
