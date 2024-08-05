#ifndef GENERIC_OBJECT_H
#define GENERIC_OBJECT_H

#include <stdlib.h>
#include "generics/types.h"

typedef struct generic_object_key_value_pair_t generic_object_key_value_pair_t;

typedef struct generic_object_iterator_t generic_object_iterator_t;

generic_object_t*   generic_object_create(size_t capacity);
void                generic_object_destroy(generic_value_t *wrapped_object);
generic_value_t*    generic_object_get(generic_object_t *object, const char *key);
size_t              generic_object_get_size(generic_object_t *object);
void                generic_object_add(generic_object_t *object, const char *key, generic_value_t *value);

generic_object_key_value_pair_t*    generic_object_kvp_get(generic_object_t *object, size_t i);
char*                               generic_object_kvp_get_key(generic_object_key_value_pair_t *kvp);
generic_value_t*                    generic_object_kvp_get_value(generic_object_key_value_pair_t *kvp);

generic_object_iterator_t*          generic_object_iterator_create(generic_object_t *object);
size_t                              generic_object_iterator_get_current_index(generic_object_iterator_t *iterator);
generic_object_key_value_pair_t*    generic_object_iterator_next(generic_object_iterator_t *iterator);

#endif
