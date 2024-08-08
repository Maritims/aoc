#include <limits.h>
#include <stdio.h>

#include "testing/assertions.h"
#include "array4c.h"

void test_array_int_t_create(size_t capacity) {
    array_int_t *result = array_int_t_create(capacity);
    assert_not_null(result, "%s\n", "array could not be created, the result was null");

    size_t size = array_int_t_get_size(result);
    assert_primitive_equality(size, 0, "array->size was %zu\n", size);
    
    size_t actual_capacity = array_int_t_get_capacity(result);
    assert_primitive_equality(actual_capacity, capacity, "array->capacity != %zu\n", actual_capacity);

    array_int_t_destroy(result);
    printf("%s passed\n", __func__);
}

void test_array_int_t_push() {
    array_int_t *array = array_int_t_create(10);
    assert_not_null(array, "%s\n", "array could not be created, the result was null");

    int *element = malloc(sizeof(int));
    *element = 1;
    ARRAY_RESULT result = array_int_t_push(array, element);
    assert_primitive_equality(result, ARRAY_RESULT_SUCCESS, "array_int_t_push(array, %d) != %d", *element, ARRAY_RESULT_SUCCESS);

    size_t size = array_int_t_get_size(array);
    assert_primitive_equality(size, 1, "array->size != %d", 1);

    void *get_result = array_int_t_get(array, 0);
    assert_not_null(get_result, "%s\n", "array_int_t_get(array, 0) returned null");
    assert_primitive_equality(*(int*)get_result, *element, "%s\n", "array_int_t_get(array, 0) != 1");

    array_int_t_destroy(array);
    printf("%s passed\n", __func__);
}

void test_array_int_t_pop() {
    array_int_t *array = array_int_t_create(10);
    int *element1 = malloc(sizeof(int));
    int *element2 = malloc(sizeof(int));
    *element1 = 1;
    *element2 = 2;
    array_int_t_push(array, element1);
    array_int_t_push(array, element2);

    int *result = malloc(sizeof(int));
    if(array_int_t_pop(&result, array) != ARRAY_RESULT_SUCCESS) {
        printf("%s failed: array_int_t_pop(array) != %d\n", __func__, ARRAY_RESULT_SUCCESS);
        exit(EXIT_FAILURE);
    }

    if(*result != *element2) {
        printf("%s failed: result != %d (%d)\n", __func__, *element2, *result);
        exit(EXIT_FAILURE);
    }

    array_int_t_destroy(array);

    printf("%s passed\n", __func__);
}

int main() {
    test_array_int_t_create(10);
    test_array_int_t_push();
    test_array_int_t_pop();

    printf("All tests passed\n");
}
