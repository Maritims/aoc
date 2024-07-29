#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "array4c.h"

#define IMPLEMENT_ARRAY_FOR(TStruct, TElement)  \
    struct TStruct {                            \
        size_t capacity;                        \
        size_t size;                            \
        TElement *elements;                     \
    }

IMPLEMENT_ARRAY_FOR(array_void_t, void*);
IMPLEMENT_ARRAY_FOR(array_int_t, int*);
IMPLEMENT_ARRAY_FOR(array_int_2d_t, array_int_t*);

static array_void_t *array_create(size_t capacity, size_t array_size, size_t element_size) {
    array_void_t *array = malloc(array_size);
    if(array == NULL) {
        fprintf(stderr, "%s:%d: Failed to allocate memory for array\n", __func__, __LINE__);
        return NULL;
    }

    array->capacity = capacity;
    array->size     = 0;
    array->elements = calloc(capacity, element_size);
    if(array->elements == NULL) {
        fprintf(stderr, "%s:%d: Failed to allocate additional memory for array elements\n", __func__, __LINE__);
        free(array);
        return NULL;
    }

    return array;
}

static void array_destroy(array_void_t *array) {
    for(size_t i = 0; i < array->size; i++) {
        free(array->elements[i]);
    }
    free(array->elements);
}

static void *array_get(array_void_t *array, size_t pos) {
    if(pos >= array->size) {
        fprintf(stderr, "%s:%d: The position %zu is outside of the bounds of the array\n", __func__, __LINE__, pos);
        return NULL;
    }

    if(array->elements[pos] == NULL) {
        fprintf(stderr, "%s:%d: The element at position %zu is NULL\n", __func__, __LINE__, pos);
        return NULL;
    }

    return array->elements[pos];
}

static ARRAY_RESULT array_push(array_void_t *array, void *element, size_t element_size) {
	if (array->size >= array->capacity) {
		size_t new_capacity = array->capacity + 10;
		void **temp = realloc(array->elements, new_capacity * element_size);
		if (temp == NULL) {
            return ARRAY_RESULT_UNABLE_TO_ALLOCATE_ADDITIONAL_MEMORY;
		}

		array->elements = temp;
		for (size_t i = array->capacity; i < new_capacity; i++) {
			array->elements[i] = NULL;
		}
		array->capacity = new_capacity;
	}

	array->elements[array->size++] = element;
    return ARRAY_RESULT_SUCCESS;
}

static ARRAY_RESULT array_pop(void **result, array_void_t *array) {
    if(array->size == 0) {
        return ARRAY_RESULT_NOOP;
    }

    if(result != NULL) {
        *result = array->elements[array->size - 1];
    }
    
    array->elements[array->size - 1] = NULL;
    array->size -= 1;

    return ARRAY_RESULT_SUCCESS;
}

static int array_contains(array_void_t *array, void *element, int (*comparator)(void *, void *))
{
	for (size_t i = 0; i < array->size; i++)
	{
        void *current_element = array_get(array, i);
		int comparison = comparator(element, current_element);
		if (comparison == 0)
		{
			return 0;
		}
	}

	return -1;
}

static size_t array_get_size(array_void_t *array) {
    return array->size;
}

static size_t array_get_capacity(array_void_t *array) {
    return array->capacity;
}

#define IMPLEMENT_ARRAY_OPERATIONS_FOR(T, S)                                        \
    T* T##_create(size_t capacity) {                                                \
        return (T*)array_create(capacity, sizeof(T), sizeof(S));                    \
    }                                                                               \
    void T##_destroy(T *array) {                                                    \
        array_destroy((array_void_t*)array);                                        \
    }                                                                               \
    S* T##_get(T *array, size_t pos) {                                              \
        return (S*)array_get((array_void_t*)array, pos);                            \
    }                                                                               \
    ARRAY_RESULT T##_push(T *array, S *element) {                                   \
        return array_push((array_void_t*)array, element, sizeof(S));                \
    }                                                                               \
    ARRAY_RESULT T##_pop(S **result, T *array) {                                    \
        return array_pop((void**)result, (array_void_t*)array);                     \
    }                                                                               \
    int T##_contains(T *array, S *element, int (*comparator)(void*, void*)) {       \
        return array_contains((array_void_t*)array, (void*)element, comparator);    \
    }                                                                               \
    size_t T##_get_size(T *array) {                                                 \
        return array_get_size((array_void_t*)array);                                \
    }                                                                               \
    size_t T##_get_capacity(T *array) {                                             \
        return array_get_capacity((array_void_t*)array);                            \
    }

IMPLEMENT_ARRAY_OPERATIONS_FOR(array_int_t, int);
IMPLEMENT_ARRAY_OPERATIONS_FOR(array_int_2d_t, array_int_t);
