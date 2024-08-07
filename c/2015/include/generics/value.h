#ifndef GENERIC_VALUE_H
#define GENERIC_VALUE_H

#include <stdbool.h>
#include "generics/types.h"

typedef enum generic_value_type_t {
    GENERIC_VALUE_TYPE_UNDEFINED = 1,
    GENERIC_VALUE_TYPE_BOOL,
    GENERIC_VALUE_TYPE_INT,
    GENERIC_VALUE_TYPE_STRING,
    GENERIC_VALUE_TYPE_OBJECT,
    GENERIC_VALUE_TYPE_ARRAY
} generic_value_type_t;

typedef union generic_value_data_t generic_value_data_t;

generic_value_t *generic_value_create();

void generic_value_destroy(generic_value_t *v);

generic_value_t *generic_value_create_array(generic_array_t *array);

generic_value_t *generic_value_create_bool(bool value);

generic_value_t *generic_value_create_int(int value);

generic_value_t *generic_value_create_object(generic_object_t *object);

generic_value_t *generic_value_create_string(char *value);

void (*generic_value_get_destructor(generic_value_t *v))(generic_value_t *v);

void generic_value_set_destructor(generic_value_t *v, void (*destructor)(generic_value_t*));

generic_array_t *generic_value_get_array(generic_value_t *v);

void generic_value_set_array(generic_value_t *v, generic_array_t *a);

bool generic_value_get_bool(generic_value_t *v);

void generic_value_set_bool(generic_value_t *v, bool value);

int generic_value_get_int(generic_value_t *v);

void generic_value_set_int(generic_value_t *v, int value);

generic_object_t *generic_value_get_object(generic_value_t *v);

void generic_value_set_object(generic_value_t *v, generic_object_t *o);

char *generic_value_get_string(generic_value_t *v);

void generic_value_set_string(generic_value_t *v, char *value);

generic_value_type_t generic_value_get_type(generic_value_t *v);

void generic_value_set_type(generic_value_t *v, generic_value_type_t t);

/**
 * Get a string representation of v.
 * @param v The value.
 * @return A string representation upon success, otherwise NULL. It's the responsibility of the caller to free the memory allocated for the string representation.
 */
char *generic_value_to_string(generic_value_t *v);

/**
 * Get a string representation of a value type.
 * @param type Value type.
 * @return A string representation upon success, otherwise null.
 */
const char *generic_value_type_to_string(generic_value_type_t type);

#endif
