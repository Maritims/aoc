#include <limits.h>
#include <stdio.h>
#include "generics/array.h"
#include "generics/types.h"
#include "generics/value.h"

struct generic_array_t {
    size_t              capacity;
    generic_value_t**   elements;
    size_t              size;
};

generic_array_t* generic_array_create(size_t capacity) {
    generic_array_t *a = malloc(sizeof(generic_array_t)); 
    if(a == NULL) {
        fprintf(stderr, "%s:%s:%d: Failed to allocate memory for array\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    a->elements = calloc(capacity, sizeof(generic_value_t*));
    if(a->elements == NULL) {
        fprintf(stderr, "%s:%s:%d: Failed to allocate memory for array elements\n", __FILE__, __func__, __LINE__);
        free(a);
        return NULL;
    }

    a->capacity     = capacity;
    a->size         = 0;

    return a;
}

void generic_array_destroy(generic_value_t *wrapped_array) {
    if(wrapped_array == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter wrapped_array was null\n", __FILE__, __func__, __LINE__);
        return;
    }

    generic_array_t *array = generic_value_get_array(wrapped_array);
    if(array == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to unwrap array\n", __FILE__, __func__, __LINE__);
        return;
    }

    for(size_t i = 0; i < array->size; i++) {
        generic_value_t *element = array->elements[i];
        generic_value_destroy(element);
    }

    free(array->elements);
    free(array);
}

size_t generic_array_get_size(generic_array_t *array) {
    if(array == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter array was null\n", __FILE__, __func__, __LINE__);
        return INT_MIN;
    }

    return array->size;
}

generic_value_t* generic_array_get(generic_array_t *a, size_t pos) {
    if(a == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter a was null\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    if(pos >= a->size) {
        fprintf(stderr, "%s:%s:%d: index %zu was out of bounds\n", __FILE__, __func__, __LINE__, pos);
        return NULL;
    }

    return a->elements[pos];
}

void generic_array_add(generic_array_t *array, generic_value_t *value) {
    if(value == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter value was null\n", __FILE__, __func__, __LINE__);
        return;
    }

    if(array->size >= array->capacity) {
        size_t new_capacity             = array->capacity * 2;
        generic_value_t **new_elements  = realloc(array->elements, new_capacity * sizeof(generic_value_t*));
        if(new_elements == NULL) {
            fprintf(stderr, "%s:%s:%d: failed to allocate additional memory for array elements\n", __FILE__, __func__, __LINE__);
            return;
        }

        array->capacity = new_capacity;
        array->elements = new_elements;
    }

    array->elements[array->size++] = value;
}
