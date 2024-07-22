#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"

Array *array_create(size_t initial_capacity, size_t element_size)
{
	Array *array = malloc(sizeof(Array));
	if (array == NULL)
	{
		fprintf(stderr, "Unable to allocate memory for array\n");
		return NULL;
	}

	array->capacity = initial_capacity;
	array->size = 0;
	array->elements = calloc(initial_capacity, element_size);
	if (array->elements == NULL)
	{
		fprintf(stderr, "Unable to allocate memory for array elements\n");
		return NULL;
	}

	return array;
}

void array_destroy(Array *array)
{
	for (size_t i = 0; i < array->size; i++)
	{
		free(array->elements[i]);
	}
	free(array->elements);
	free(array);
}

void *array_get(Array *array, size_t i)
{
	if (i >= array->size)
	{
		fprintf(stderr, "Index %d is out of bounds\n", i);
		return NULL;
	}

	if (array->elements[i] == NULL)
	{
		fprintf(stderr, "No element exists at index %zu\n", i);
		return NULL;
	}

	return array->elements[i];
}

void array_push(Array *array, void **element, size_t element_size)
{
	if (array->size >= array->capacity)
	{
		size_t new_capacity = array->capacity + 10;
		void **temp = realloc(array->elements, new_capacity * element_size);
		if (temp == NULL)
		{
			fprintf(stderr, "Unable to allocate additional memory for array elements\n");
			return;
		}

		array->elements = temp;
		for (uint32_t i = array->capacity; i < new_capacity; i++)
		{
			array->elements[i] = NULL;
		}
		array->capacity = new_capacity;
	}

	array->elements[array->size++] = element;
}

int array_contains(Array *array, void *element, int (*comparator)(void *, void *))
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
