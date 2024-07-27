#include <stdio.h>
#include <string.h>

#include "json4c.h"

bool verify_key_value_pair(KeyValuePair* key_value_pair, const char* expected_key, bool should_have_value) {
    if(key_value_pair) {
        printf("Parameter \"key_value_pair\" cannot be NULL\n");
        return false;
    }

    if(expected_key == NULL) {
        printf("Parameter \"expected_key\" cannot be NULL\n");
        return false;
    }

    if(strlen(expected_key) == 0) {
        printf("Parameter \"expected_key\" cannot be empty\n");
        return false;
    }

    if(should_have_value && key_value_pair->value == NULL) {
        printf("Expected value but value was NULL\n");
        return false;
    }

    return true;
}

bool verify_type_and_pair_size(char *result, GenericValue* generic_value, GenericValueType expected_generic_value_type, size_t expected_pair_size) {
    if (generic_value == NULL) {
        sprintf(result, "%s", "Parameter \"generic_value\" cannot be NULL");
        return false;
    }

    if (generic_value->type != expected_generic_value_type) {
        sprintf(result, "type: expected %d, received %d", expected_generic_value_type, generic_value->type);
        return false;
    }

    if (generic_value->data.object_value != NULL && generic_value->data.object_value->size != expected_pair_size) {
        sprintf(result, "size: expected %zu, received %zu\n", expected_pair_size, generic_value->data.object_value->size);
        return false;
    }

    if(generic_value->data.array_value != NULL && generic_value->data.array_value->size != expected_pair_size) {
        sprintf(result, "size: expected %zu, received %zu\n", expected_pair_size, generic_value->data.array_value->size);
        return false;
    }

    return true;
}

void test_empty_object() {
    size_t number_of_tokens = 0;
    JsonToken* tokens = lex("{}", &number_of_tokens);
    GenericValue* gv = parse(&tokens, true);
    char result[1024];

    if (!verify_type_and_pair_size(result, gv, TYPE_OBJECT, 0)) {
        printf("%s() failed: type and pair size verification failed with reason \"%s\"\n", __func__, result);
        exit(EXIT_FAILURE);
    }

    printf("%s(): passed\n", __func__);
}

void test_basic_object() {
    size_t number_of_tokens = 0;
    JsonToken* tokens = lex("{\"foo\": \"bar\"}", &number_of_tokens);
    GenericValue* generic_value = parse(&tokens, true);
    char result[1024];

    if (generic_value == NULL) {
        printf("%s() failed: parse returned NULL\n", __func__);
        exit(EXIT_FAILURE);
    }

    if (!verify_type_and_pair_size(result, generic_value, TYPE_OBJECT, 1)) {
        printf("i%s() failure: Type and pair size verification failed with reason \"%s\"\n", __func__, result);
        exit(EXIT_FAILURE);
    }

    KeyValuePair* kvp = generic_value->data.object_value->pairs[0];
    KeyValuePair expected_kvp = {
        .key = "foo",
        .value = &(GenericValue) {
            .type = TYPE_STRING,
            .data.string_value = "bar"
        }
    };
    if (!generic_compare_kvp(kvp, &expected_kvp)) {
        printf("%s() failed: expected key: %s, received key: %s, expected value: %s, received value: %s\n", __func__, expected_kvp.key, kvp->key, expected_kvp.value->data.string_value, kvp->value->data.string_value);
        exit(EXIT_FAILURE);
    }

    printf("%s() passed\n", __func__);
}

void test_basic_object_with_single_character_property() {
    size_t number_of_tokens = 0;
    JsonToken* tokens = lex("{\"f\": \"b\"}", &number_of_tokens);
    GenericValue* gv = parse(&tokens, true);
    if(gv == NULL) {
        printf("%s() failed: parse returned NULL\n");
        exit(EXIT_FAILURE);
    }
    char result[1024];

    if(!verify_type_and_pair_size(result, gv, TYPE_OBJECT, 1)) {
        printf("%s() failed: type and pair size verification failed with reason \"%s\"\n", __func__, result);
        exit(EXIT_FAILURE);
    }

    KeyValuePair* kvp = gv->data.object_value->pairs[0];
    KeyValuePair expected_kvp = {
        .key = "f",
        .value = &(GenericValue) {
            .type = TYPE_STRING,
            .data.string_value = "b"
        }
    };
    if(!generic_compare_kvp(kvp, &expected_kvp)) {
        printf("%s() failed: expected key: %s, received key: %s, expected value: %s, received value: %s\n", __func__, expected_kvp.key, kvp->key, expected_kvp.value->data.string_value, kvp->value->data.string_value);
        exit(EXIT_FAILURE);
    }

    printf("%s() passed\n", __func__);
}

void test_number_object() {
    size_t number_of_tokens = 0;
    JsonToken* tokens = lex("{\"foo\": 1}", &number_of_tokens);
    GenericValue* generic_value = parse(&tokens, true);
    char result[1024];

    if (!verify_type_and_pair_size(result, generic_value, TYPE_OBJECT, 1)) {
        printf("%s() failed: type and pair size verification failed with reason \"%s\"\n", __func__, result);
        exit(EXIT_FAILURE);
    }

    KeyValuePair* kvp = generic_value->data.object_value->pairs[0];
    KeyValuePair expected_kvp = {
        .key = "foo",
        .value = &(GenericValue) {
            .type = TYPE_INT,
            .data.int_value = 1
        }
    };
    if (!generic_compare_kvp(kvp, &expected_kvp)) {
        printf("%s() failed: key value pairs are not equal\n", __func__);
        exit(EXIT_FAILURE);
    }

    printf("%s() passed\n", __func__);
}

void test_empty_array() {
    size_t number_of_tokens = 0;
    JsonToken* tokens = lex("{\"foo\": []}", &number_of_tokens);
    GenericValue* generic_value = parse(&tokens, true);

    KeyValuePair* kvp = generic_get_pair_from_object(generic_value->data.object_value, "foo");
    if(kvp == NULL) {
        printf("%s() failed: no key value pair with key \"foo\" was found\n\n", __func__);
        exit(EXIT_FAILURE);
    }

    GenericValue *kvp_value = (GenericValue*)kvp->value;
    GenericArray *generic_array = kvp_value->data.array_value;
    if(generic_array->size != 0) {
        printf("%s() failed: Array size was %zu, but expected 0\n\n", __func__, generic_array->size);
        exit(EXIT_FAILURE);
    }

    printf("%s() passed\n", __func__);
}

void test_basic_array() {
    size_t number_of_tokens = 0;
    JsonToken* tokens = lex("{\"foo\": [1,2,\"three\"]}", &number_of_tokens);
    GenericValue* generic_value = parse(&tokens, true);

    KeyValuePair *kvp = generic_get_pair_from_object(generic_value->data.object_value, "foo");
    if(kvp == NULL) {
        printf("%s() failed: no key value pair with key \"foo\" could be found\n\n", __func__);
        exit(EXIT_FAILURE);
    }

    GenericValue *kvp_value = (GenericValue*)kvp->value;
    GenericArray *generic_array = kvp_value->data.array_value;
    if(generic_array->size != 3) {
        printf("%s() failed: array size was %zu, but expected0\n\n", __func__, generic_array->size);
        exit(EXIT_FAILURE);
    }

    printf("%s() passed\n", __func__);
}

void test_nested_object() {
    size_t number_of_tokens = 0;
    JsonToken* tokens = lex("{\"foo\": {\"bar\": 2}}", &number_of_tokens);
    GenericValue* generic_value = parse(&tokens, true);

    KeyValuePair *kvp_foo = generic_get_pair_from_object(generic_value->data.object_value, "foo");
    GenericValue *gv_foo = (GenericValue*)kvp_foo->value;
    GenericObject *foo = gv_foo->data.object_value;

    KeyValuePair *kvp_bar = generic_get_pair_from_object(foo, "bar");
    GenericValue *gv_bar = (GenericValue*)kvp_bar->value;
    int bar = gv_bar->data.int_value;

    if(bar != 2) {
        printf("%s() failed: foo.bar was %d, but expected 2\n\n", __func__, bar);
        exit(EXIT_FAILURE);
    }

    printf("%s() passed\n", __func__);
}

void test_true() {
    size_t number_of_tokens = 0;
    JsonToken* tokens = lex("{\"foo\": true}", &number_of_tokens);
    GenericValue* generic_value = parse(&tokens, true);
    char result[1024];

    if(!verify_type_and_pair_size(result, generic_value, TYPE_OBJECT, 1)) {
        printf("%s() failed: type and pair size verification failed with reason \"%s\"\n", __func__, result);
        exit(EXIT_FAILURE);
    }

    KeyValuePair* kvp_foo = generic_value->data.object_value->pairs[0];
    GenericValue* gv_foo = kvp_foo->value;

    if(gv_foo->data.boolean_value != true) {
        printf("%s() failed: expected foo to be true, but it was false.\n\n", __func__);
        exit(EXIT_FAILURE);
    }

    printf("%s() passed\n", __func__);
}

void test_false() {
    size_t number_of_tokens = 0;
    JsonToken* tokens = lex("{\"foo\": false}", &number_of_tokens);
    GenericValue* generic_value = parse(&tokens, true);
    KeyValuePair* kvp_foo = generic_value->data.object_value->pairs[0];
    GenericValue* gv_foo = kvp_foo->value;

    if(gv_foo->data.boolean_value != false) {
        printf("%s() failed: expected foo to be false, but it was true.\n\n", __func__);
        exit(EXIT_FAILURE);
    }

    printf("%s() passed\n", __func__);
}

void test_null() {
    size_t number_of_tokens = 0;
    JsonToken* tokens = lex("{\"foo\": null}", &number_of_tokens);
    GenericValue* generic_value = parse(&tokens, true);
    KeyValuePair* kvp_foo = generic_value->data.object_value->pairs[0];
    GenericValue* gv_foo = kvp_foo->value;

    if(gv_foo != NULL) {
        printf("%s() failed: expected foo to be NULL, but it wasn't.\n\n", __func__);
        exit(EXIT_FAILURE);
    }

    printf("%s() passed\n", __func__);
}

void test_object_with_multiple_properties() {
    size_t number_of_tokens = 0;
    JsonToken *tokens = lex("{\"e\": \"green\", \"a\": 77}", &number_of_tokens);
    GenericValue *gv = parse(&tokens, true);
    if(gv == NULL) {
        printf("%s() failed: parse returned NULL\n", __func__);
        exit(EXIT_FAILURE);
    }
    char result[1024];

    if(!verify_type_and_pair_size(result, gv, TYPE_OBJECT, 2)) {
        printf("%s() failed: type and pair size verification failed with reason \"%s\"\n", result);
        exit(EXIT_FAILURE);
    }

    KeyValuePair *kvp_one = gv->data.object_value->pairs[0];
    KeyValuePair expected_kvp_one = {
        .key = "e",
        .value = &(GenericValue) {
            .type = TYPE_STRING,
            .data.string_value = "green"
        }
    };
    if(!generic_compare_kvp(kvp_one, &expected_kvp_one)) {
        printf("%s() failed: expected key: %s, received key: %s, expected value: %s, received value: %s\n", __func__, expected_kvp_one.key, kvp_one->key, expected_kvp_one.value->data.string_value, kvp_one->value->data.string_value);
        exit(EXIT_FAILURE);
    }

    KeyValuePair *kvp_two = gv->data.object_value->pairs[1];
    KeyValuePair expected_kvp_two = {
        .key = "a",
        .value = &(GenericValue) {
            .type = TYPE_INT,
            .data.int_value = 77
        }
    };
    if(!generic_compare_kvp(kvp_two, &expected_kvp_two)) {
        printf("%s() failed: expected key: %s, received key: %s, expected value: %d, received value: %d\n", __func__, expected_kvp_two.key, kvp_two->key, expected_kvp_two.value->data.int_value, kvp_two->value->data.int_value);
        exit(EXIT_FAILURE);
    }

    printf("%s() passed\n", __func__);
}

int main(void) {
    test_empty_object();
    test_basic_object();
    test_basic_object_with_single_character_property();
    test_number_object();
    test_empty_array();
    test_basic_array();
    test_nested_object();
    test_true();
    test_false();
    test_null();
    test_object_with_multiple_properties();

    printf("All tests passed\n");
    return EXIT_SUCCESS;
}
