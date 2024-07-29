#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>

typedef enum ARRAY_RESULT {
    ARRAY_RESULT_SUCCESS,
    ARRAY_RESULT_FAILURE,
    ARRAY_RESULT_UNABLE_TO_ALLOCATE_MEMORY,
    ARRAY_RESULT_UNABLE_TO_ALLOCATE_ADDITIONAL_MEMORY,
    ARRAY_RESULT_INDEX_OUT_OF_BOUNDS,
    ARRAY_RESULT_NO_ELEMENT_AT_INDEX,
    ARRAY_RESULT_NOOP
} ARRAY_RESULT;

typedef struct array_void_t array_void_t;
typedef struct array_int_t array_int_t;
typedef struct array_int_2d_t array_int_2d_t;

#define DEFINE_ARRAY_OPERATIONS(T, S)                                       \
    T* T##_create(size_t capacity);                                         \
    void T##_destroy(T *array);                                             \
    S* T##_get(T *array, size_t pos);                                       \
    ARRAY_RESULT T##_push(T *array, S *element);                            \
    ARRAY_RESULT T##_pop(S **result, T *array);                             \
    int T##_contains(T *array, S *element, int (*comparator)(void*, void*));\
    size_t T##_get_size(T *array);                                          \
    size_t T##_get_capacity(T *array);

DEFINE_ARRAY_OPERATIONS(array_int_t, int);
DEFINE_ARRAY_OPERATIONS(array_int_2d_t, array_int_t);

#endif
