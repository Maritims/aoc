#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "json/lexer.h"
#include "json/parser.h"
#include "json/token.h"

void assert_size_and_type(const char *test_name, json_node_t *node, json_node_type_t expected_type, size_t expected_size) {
    if(node->type != expected_type) {
        printf("%s failed: type != %d (%d)\n", test_name, expected_type, node->type);
        exit(EXIT_FAILURE);
    }

    size_t size;

    switch(expected_type) {
        case JSON_NODE_TYPE_ARRAY:
            size    = ((json_array_t*)node->value)->size;
            break;
        case JSON_NODE_TYPE_OBJECT:
            size    = ((json_object_t*)node->value)->size;
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

void test_empty_object() {
    size_t number_of_tokens = 0;
    size_t current_token_index = 0;
    json_token_t *tokens = json_lex("{}", &number_of_tokens);
    json_node_t *node = json_parse(tokens, &current_token_index, true);

    assert_size_and_type(__func__, node, JSON_NODE_TYPE_OBJECT, 0);

    printf("%s passed\n", __func__);
    json_node_destroy(node);
    free(tokens);
}

void test_json_lex(char *str, json_token_t *expected, size_t expected_len) {
    size_t len = 0;
    json_token_t *tokens = json_lex(str, &len);

    if(len != expected_len) {
        printf("%s(\"%s\") failed: length != %zu (%zu)\n", __func__, str, expected_len, len);
        exit(EXIT_FAILURE);
    }

    for(size_t i = 0; i < len; i++) {
        if(tokens[i].type != expected[i].type) {
            printf("%s(\"%s\") failed: tokens[%zu].type != %d (%d)\n", __func__, str, i, expected[i].type, tokens[i].type);
            exit(EXIT_FAILURE);
        }

        if(tokens[i].type == JSON_TOKEN_TYPE_STRING && strcmp(tokens[i].value.string_value, expected[i].value.string_value) != 0) {
            printf("%s(\"%s\") failed: tokens[%zu].value.string_value != %s (%s)\n", __func__, str, i, expected[i].value.string_value, tokens[i].value.string_value);
            exit(EXIT_FAILURE);
        }
    }

    printf("%s(\"%s\") passed\n", __func__, str);
    free(tokens);
}

void test_basic_object() {
    size_t number_of_tokens = 0;
    size_t current_token_index = 0;
    json_token_t *tokens = json_lex("{\"foo\": \"bar\"}", &number_of_tokens);
    json_node_t *node = json_parse(tokens, &current_token_index, true);

    assert_size_and_type(__func__, node, JSON_NODE_TYPE_OBJECT, 1);

    json_object_t *object = (json_object_t*)node->value;
    if(object == NULL) {
        printf("%s failed: object as null\n", __func__);
        exit(EXIT_FAILURE);
    }

    json_node_t *foo = json_object_get(object, "foo");
    if(foo == NULL) {
        printf("%s failed: foo was null\n", __func__);
        exit(EXIT_FAILURE);
    }

    if(foo->value == NULL) {
        printf("%s failed: string value was null\n", __func__);
        exit(EXIT_FAILURE);
    }

    if(strcmp((char*)foo->value, "bar") != 0) {
        printf("%s failed: string value != bar (%s)\n", __func__, (char*)foo->value);
        exit(EXIT_FAILURE);
    }

    printf("%s passed\n", __func__);
    json_node_destroy(node);
    free(tokens);
}

void test_basic_object_with_single_character_property() {
    size_t number_of_tokens = 0;
    size_t current_token_index = 0;
    json_token_t *tokens = json_lex("{\"f\": \"b\"}", &number_of_tokens);
    json_node_t *node = json_parse(tokens, &current_token_index, true);

    assert_size_and_type(__func__, node, JSON_NODE_TYPE_OBJECT, 1);

    json_object_t *object = (json_object_t*)node->value;
    json_node_t *f = json_object_get(object, "f");
    if(f == NULL) {
        printf("%s failed: f == NULL\n", __func__);
        exit(EXIT_FAILURE);
    }

    if(f->value == NULL) {
        printf("%s failed: string value was null\n", __func__);
        exit(EXIT_FAILURE);
    }

    if(strcmp((char*)f->value, "b") != 0) {
        printf("%s failed: string value != b (%s)\n", __func__, (char*)node->value);
        exit(EXIT_FAILURE);
    }

    printf("%s passed\n", __func__);
    json_node_destroy(node);
    free(tokens);
}

void test_number_object() {
    size_t number_of_tokens = 0;
    size_t current_token_index = 0;
    json_token_t *tokens = json_lex("{\"foo\": 1}", &number_of_tokens);
    json_node_t *node = json_parse(tokens, &current_token_index, true);

    assert_size_and_type(__func__, node, JSON_NODE_TYPE_OBJECT, 1);

    json_object_t *object = (json_object_t*)node->value;
    json_node_t *foo = json_object_get(object, "foo");
    if(foo == NULL) {
        printf("%s failed: foo == NULL\n", __func__);
        exit(EXIT_FAILURE);
    }

    if(*(int*)foo->value != 1) {
        printf("%s failed: int value != 1 (%d)\n", __func__, *(int*)foo->value);
        exit(EXIT_FAILURE);
    }

    printf("%s passed\n", __func__);
    json_node_destroy(node);
    free(tokens);
}

void test_empty_array() {
    size_t number_of_tokens = 0;
    size_t current_token_index = 0;
    json_token_t *tokens = json_lex("{\"foo\": []}", &number_of_tokens);
    json_node_t *node = json_parse(tokens, &current_token_index, true);

    assert_size_and_type(__func__, node, JSON_NODE_TYPE_OBJECT, 1);

    json_object_t *object = (json_object_t*)node->value;
    json_node_t *foo = json_object_get(object, "foo");

    assert_size_and_type(__func__, foo, JSON_NODE_TYPE_ARRAY, 0);

    printf("%s passed\n", __func__);
    json_node_destroy(node);
    free(tokens);
}

void test_basic_array() {
    size_t number_of_tokens = 0;
    size_t current_token_index = 0;
    json_token_t *tokens = json_lex("{\"foo\": [1,2,\"three\"]}", &number_of_tokens);
    json_node_t *node = json_parse(tokens, &current_token_index, true);

    assert_size_and_type(__func__, node, JSON_NODE_TYPE_OBJECT, 1);

    json_object_t *object = (json_object_t*)node->value;
    json_node_t *foo = json_object_get(object, "foo");

    assert_size_and_type(__func__, foo, JSON_NODE_TYPE_ARRAY, 3);

    printf("%s passed\n", __func__);
    json_node_destroy(node);
    free(tokens);
}

void test_nested_object() {
    size_t number_of_tokens = 0;
    size_t current_token_index = 0;
    json_token_t *tokens = json_lex("{\"foo\": {\"bar\": 2}}", &number_of_tokens);

    json_node_t *root_node = json_parse(tokens, &current_token_index, true);
    assert_size_and_type(__func__, root_node, JSON_NODE_TYPE_OBJECT, 1);
    json_object_t *root_object = (json_object_t*)root_node->value;

    json_node_t *foo_node = json_object_get(root_object, "foo");
    assert_size_and_type(__func__, foo_node, JSON_NODE_TYPE_OBJECT, 1);
    json_object_t *foo_object = (json_object_t*)foo_node->value;

    json_node_t *bar_node = json_object_get(foo_object, "bar");
    int bar_number = *(int*)bar_node->value;

    if(bar_number != 2) {
        printf("%s failed: number != 2 (%d)\n", __func__, bar_number);
        exit(EXIT_FAILURE);
    }
    
    printf("%s passed\n", __func__);
    json_node_destroy(root_node);
    free(tokens);
}

void test_bool(char *bool_value, bool expected) {
    char json_string[1024];
    sprintf(json_string, "{\"foo\": %s}", bool_value);
    size_t number_of_tokens = 0;
    size_t current_token_index = 0;
    json_token_t *tokens = json_lex(json_string, &number_of_tokens);

    json_node_t *root_node = json_parse(tokens, &current_token_index, true);
    assert_size_and_type(__func__, root_node, JSON_NODE_TYPE_OBJECT, 1);
    json_object_t *root_object = (json_object_t*)root_node->value;

    json_node_t *foo_node = json_object_get(root_object, "foo");
    bool foo_bool = *(bool*)foo_node->value;

    if(foo_bool != expected) {
        printf("%s(\"%s\") failed: generic_value_get_bool(foo) != %d\n", __func__, bool_value, expected);
        exit(EXIT_FAILURE);
    }

    printf("%s passed\n", __func__);
    json_node_destroy(root_node);
    free(tokens);
}

void test_null() {
    size_t number_of_tokens = 0;
    size_t current_token_index = 0;
    json_token_t *tokens = json_lex("{\"foo\": null}", &number_of_tokens);
    
    json_node_t *root_node = json_parse(tokens, &current_token_index, true);
    assert_size_and_type(__func__, root_node, JSON_NODE_TYPE_OBJECT, 1);
    json_object_t *root_object = (json_object_t*)root_node->value;

    json_node_t *foo_node = json_object_get(root_object, "foo");
    if(foo_node->value != NULL) {
        printf("%s failed: foo wasn't null\n", __func__);
        exit(EXIT_FAILURE);
    }

    printf("%s passed\n", __func__);
    json_node_destroy(root_node);
    free(tokens);
}

void test_object_with_multiple_properties() {
    size_t number_of_tokens = 0;
    size_t current_token_index = 0;
    json_token_t *tokens = json_lex("{\"e\": \"green\", \"a\": 77}", &number_of_tokens);

    json_node_t *root_node = json_parse(tokens, &current_token_index, true);
    assert_size_and_type(__func__, root_node, JSON_NODE_TYPE_OBJECT, 2);
    json_object_t *root_object = (json_object_t*)root_node->value;

    json_node_t *e_node = json_object_get(root_object, "e"); 
    if(e_node == NULL) {
        printf("%s failed: e was null\n", __func__);
        exit(EXIT_FAILURE);
    }
    if(strcmp((char*)e_node->value, "green") != 0) {
        printf("%s failed: e != green (%s)\n", __func__, (char*)e_node->value);
        exit(EXIT_FAILURE);
    }

    json_node_t *a_node = json_object_get(root_object, "a");
    int a_number = *(int*)a_node->value;
    if(a_number != 77) {
        printf("%s failed: a != 77 (%d)\n", __func__, a_number);
        exit(EXIT_FAILURE);
    }

    printf("%s passed\n", __func__);
    json_node_destroy(root_node);
    free(tokens);
}

int main(void) {
    test_json_lex("{}", (json_token_t[]){
        (json_token_t){
            .type = JSON_TOKEN_TYPE_LEFT_BRACE,
            .value.string_value = "{"
        },
        (json_token_t){
            .type = JSON_TOKEN_TYPE_RIGHT_BRACE,
            .value.string_value = "}"
        }
    }, 2);
    test_json_lex("{\"foo\": \"bar\"}", (json_token_t[]){
        (json_token_t){
            .type = JSON_TOKEN_TYPE_LEFT_BRACE,
            .value.string_value = "{"
        },
        (json_token_t){
            .type = JSON_TOKEN_TYPE_STRING,
            .value.string_value = "foo"
        },
        (json_token_t){
            .type = JSON_TOKEN_TYPE_COLON,
            .value.string_value = ":"
        },
        (json_token_t){
            .type = JSON_TOKEN_TYPE_STRING,
            .value.string_value = "bar"
        },
        (json_token_t){
            .type = JSON_TOKEN_TYPE_RIGHT_BRACE,
            .value.string_value = "}"
        }
    }, 5);
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
