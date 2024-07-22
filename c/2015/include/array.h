#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>

typedef struct
{
	size_t capacity;
	size_t size;
	void **elements;
} Array;

Array *array_create(size_t initial_capacity, size_t element_size);

void array_destroy(Array *array);

void *array_get(Array *array, size_t i);

void array_push(Array *array, void **element, size_t element_size);

int array_contains(Array *array, void *element, int (*comparator)(void *, void *));

#endif
