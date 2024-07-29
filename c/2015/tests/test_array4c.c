#include <limits.h>
#include <stdio.h>

#include "array4c.h"

void test_array_int_t_create(size_t capacity) {
    array_int_t *result = array_int_t_create(capacity);

    if(result != ARRAY_RESULT_SUCCESS) {
        printf("%s failed: array_int_t_create(&array, %zu) != %d\n", __func__, capacity, ARRAY_RESULT_SUCCESS);
        exit(EXIT_FAILURE);
    }

    if(array_int_t_get_size(result) != 0) {
        printf("%s failed: array->size != 0\n", __func__);
        exit(EXIT_FAILURE);
    }

    if(array_int_t_get_capacity(result) != capacity) {
        printf("%s failed: array->capacity != %zu\n", __func__, capacity);
        exit(EXIT_FAILURE);
    }

    array_int_t_destroy(result);

    printf("%s passed\n", __func__);
}

void test_array_int_t_push() {
    array_int_t *array = array_int_t_create(10);
    int *element = malloc(sizeof(int));
    *element = 1;
    ARRAY_RESULT result = array_int_t_push(array, element);

    if(result != ARRAY_RESULT_SUCCESS) {
        printf("%s failed: array_int_t_push(array, %d) != %d\n", __func__, *element, ARRAY_RESULT_SUCCESS);
        exit(EXIT_FAILURE);
    }

    if(array_int_t_get_size(array) != 1) {
        printf("%s failed: array->size != 1\n", __func__);
        exit(EXIT_FAILURE);
    }

    if(*array_int_t_get(array, 1) != 1) {
        printf("%s failed: array->elements[0] != 1\n", __func__);
        exit(EXIT_FAILURE);
    }

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
