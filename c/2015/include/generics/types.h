#ifndef GENERIC_TYPES_H
#define GENERIC_TYPES_H

#include <stdbool.h>
#include <stdlib.h>

typedef enum generic_value_type_t {
    GENERIC_VALUE_TYPE_UNDEFINED = 1,
    GENERIC_VALUE_TYPE_BOOL,
    GENERIC_VALUE_TYPE_INT,
    GENERIC_VALUE_TYPE_STRING,
    GENERIC_VALUE_TYPE_OBJECT,
    GENERIC_VALUE_TYPE_ARRAY
} generic_value_type_t;

typedef struct generic_value_t {
    void*                   data;
    generic_value_type_t    type;
    void (*destructor)(void*);
} generic_value_t;

typedef struct generic_array_t {
    generic_value_t**   elements;
    size_t              capacity;
    size_t              size;
} generic_array_t;

typedef struct generic_object_key_value_pair_t {
    char                    key[1024];
    generic_value_t         value;
} generic_object_key_value_pair_t;

typedef struct generic_object_t {
    generic_object_key_value_pair_t*    pairs;
    size_t                              capacity;
    size_t                              size;
} generic_object_t;

#endif
