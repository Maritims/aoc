#ifndef GENERIC_ARRAY_H
#define GENERIC_ARRAY_H

#include <stdlib.h>
#include "generics/types.h"

generic_array_t*    generic_array_create(size_t capacity);
void                generic_array_destroy(generic_value_t *wrapped_array);
size_t              generic_array_get_size(generic_array_t *array);
generic_value_t*    generic_array_get(generic_array_t *array, size_t position);
void                generic_array_add(generic_array_t *array, generic_value_t *value);

#endif
