#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generics/array.h"
#include "generics/object.h"
#include "generics/value.h"
#include "string4c.h"

union generic_value_data_t {
    bool                boolean_value;
    int                 int_value;
    char*               string_value;
    generic_array_t*    array_value;
    generic_object_t*   object_value;
};

struct generic_value_t {
    generic_value_data_t data;
    generic_value_type_t type;
    void (*destructor)(struct generic_value_t*);
};

generic_value_t* generic_value_create() {
    generic_value_t *v = malloc(sizeof(generic_value_t));
    if(v == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to allocate memory for generic value\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    return NULL;
}

void generic_value_destroy(generic_value_t *v) {
    if(v == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter v was null\n", __FILE__, __func__, __LINE__);
        return;
    }

    switch(v->type) {
        case GENERIC_VALUE_TYPE_ARRAY:
            generic_value_get_destructor(v)(v);
        break;
        case GENERIC_VALUE_TYPE_OBJECT:
            generic_value_get_destructor(v)(v);
        break;
        case GENERIC_VALUE_TYPE_STRING:
            free(v->data.string_value);
        break;
        default:
            break;
    }

    free(v);
}

generic_value_t *generic_value_create_array(generic_array_t *array) {
    if(array == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter array was null\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    generic_value_t *value = generic_value_create();
    if(value == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to create wrapper\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    generic_value_set_type(value, GENERIC_VALUE_TYPE_ARRAY);
    generic_value_set_array(value, array);
    generic_value_set_destructor(value, generic_array_destroy);

    return value;
}

generic_value_t* generic_value_create_bool(bool value) {
    generic_value_t *v = generic_value_create();
    if(v == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to create wrapper\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    generic_value_set_type(v, GENERIC_VALUE_TYPE_BOOL);
    generic_value_set_bool(v, value);
    generic_value_set_destructor(v, generic_value_destroy);

    return v;
}

generic_value_t *generic_value_create_int(int value) {
    generic_value_t *v = generic_value_create();
    if(v == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to create wrapper\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    generic_value_set_type(v, GENERIC_VALUE_TYPE_INT);
    generic_value_set_int(v, value);
    generic_value_set_destructor(v, generic_value_destroy);

    return v;
}

generic_value_t *generic_value_create_object(generic_object_t *object) {
    if(object == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter object was null\n", __FILE__, __func__, __LINE__);
        return NULL;
    }
}

generic_value_t *generic_value_create_string(char *value) {
    generic_value_t *v = generic_value_create();
    if(v == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to create wrapper\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    generic_value_set_type(v, GENERIC_VALUE_TYPE_STRING);
    generic_value_set_string(v, value);
    generic_value_set_destructor(v, generic_object_destroy);

    return v;
}

void (*generic_value_get_destructor(generic_value_t *value))(generic_value_t *value) {
    if(value == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter value was null\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    return value->destructor;
}

void generic_value_set_destructor(generic_value_t *v, void (*destructor)(generic_value_t *)) {
    if(v == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter v was null\n", __FILE__, __func__, __LINE__);
        return;
    }

    if(destructor == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter destructor was null\n", __FILE__, __func__, __LINE__);
        return;
    }

    v->destructor = destructor;
}

generic_array_t *generic_value_get_array(generic_value_t *v) {
    if(v == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter v was null\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    if(v->type != GENERIC_VALUE_TYPE_ARRAY) {
        fprintf(stderr, "%s:%s:%d: value type was %d\n", __FILE__, __func__, __LINE__, v->type);
        return NULL;
    }

    return v->data.array_value;
}

void generic_value_set_array(generic_value_t *v, generic_array_t *a) {
    if(v == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter v was null\n", __FILE__, __func__, __LINE__);
        return;
    }

    if(v->type != GENERIC_VALUE_TYPE_ARRAY) {
        fprintf(stderr, "%s:%s:%d: attemped to assign array to value with type = %d\n", __FILE__, __func__, __LINE__, v->type);
        return;
    }

    if(a == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter a was null\n", __FILE__, __func__, __LINE__);
        return;
    }

    v->data = (generic_value_data_t){
        .array_value = a
    };
}

bool generic_value_get_bool(generic_value_t *v) {
    if(v == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter v was null\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    if(v->type != GENERIC_VALUE_TYPE_BOOL) {
        fprintf(stderr, "%s:%s:%d: value type was %d\n", __FILE__, __func__, __LINE__, v->type);
        return NULL;
    }

    return v->data.boolean_value;
}

void generic_value_set_bool(generic_value_t *v, bool value) {
    if(v == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter v was null\n", __FILE__, __func__, __LINE__);
        return;
    }

    if(v->type != GENERIC_VALUE_TYPE_BOOL) {
        fprintf(stderr, "%s:%s:%d: attempted to assign bool to value with type = %d\n", __FILE__, __func__, __LINE__, v->type);
        return;
    }

    v->data = (generic_value_data_t){
        .boolean_value = value
    };
}

int generic_value_get_int(generic_value_t *v) {
    if(v == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter v was null\n", __FILE__, __func__, __LINE__);
        return INT_MIN;
    }

    if(v->type != GENERIC_VALUE_TYPE_INT) {
        fprintf(stderr, "%s:%s:%d: value type was %d\n", __FILE__, __func__, __LINE__, v->type);
        return INT_MIN;
    }

    return v->data.int_value;
}

void generic_value_set_int(generic_value_t *v, int value) {
    if(v == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter v was null\n", __FILE__, __func__, __LINE__);
        return;
    }

    if(v->type != GENERIC_VALUE_TYPE_INT) {
        fprintf(stderr, "%s:%s:%d: attempted to assign int to value with type = %d\n", __FILE__, __func__, __LINE__, v->type);
        return;
    }

    v->data = (generic_value_data_t){
        .int_value = value
    };
}

generic_object_t *generic_value_get_object(generic_value_t *v) {
    if(v == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter v was null\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    if(v->type != GENERIC_VALUE_TYPE_OBJECT) {
        fprintf(stderr, "%s:%s:%d: value type was %d\n", __FILE__, __func__, __LINE__, v->type);
        return NULL;
    }

    return v->data.object_value;
}

void generic_value_set_object(generic_value_t *v, generic_object_t *o) {
    if(v == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter v was null\n", __FILE__, __func__, __LINE__);
        return;
    }

    if(v->type != GENERIC_VALUE_TYPE_OBJECT) {
        fprintf(stderr, "%s:%s:%d: attempted to assign object to value with type = %d\n", __FILE__, __func__, __LINE__, v->type);
        return;
    }

    if(o == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter o was null\n", __FILE__, __func__, __LINE__);
        return;
    }

    v->data = (generic_value_data_t){
        .object_value = o
    };
}

char *generic_value_get_string(generic_value_t *v) {
    if(v == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter v was null\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    if(v->type != GENERIC_VALUE_TYPE_STRING) {
        fprintf(stderr, "%s:%s:%d: value type was %d\n", __FILE__, __func__, __LINE__, v->type);
        return NULL;
    }

    return v->data.string_value;
}

void generic_value_set_string(generic_value_t *v, char *value) {
    if(v == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter v was null\n", __FILE__, __func__, __LINE__);
        return;
    }

    if(v->type != GENERIC_VALUE_TYPE_STRING) {
        fprintf(stderr, "%s:%s:%d: attempted to assign string to value with type = %d\n", __FILE__, __func__, __LINE__, v->type);
        return;
    }

    v->data = (generic_value_data_t){
        .string_value = value
    };
}

generic_value_type_t generic_value_get_type(generic_value_t *v) {
    return v->type;
}

void generic_value_set_type(generic_value_t *v, generic_value_type_t t) {
    v->type = t;
}

char *generic_value_to_string(generic_value_t *v) {
    if(v == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter v was null\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    StringBuffer* buffer = string_buffer_create(1024);
    if(!buffer) {
        fprintf(stderr, "%s:%s:%d: failed to create string buffer\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    char temp[32];

    generic_value_type_t type = generic_value_get_type(v);
    switch(type) {
        case GENERIC_VALUE_TYPE_BOOL:
            string_buffer_append(buffer, generic_value_get_bool(v) ? "true" : "false");
            break;
        case GENERIC_VALUE_TYPE_INT:
            snprintf(temp, sizeof(temp), "%d", generic_value_get_int(v));
            string_buffer_append(buffer, temp);
            temp[0] = '\0';
            break;
        case GENERIC_VALUE_TYPE_STRING:
            string_buffer_append(buffer, "\"");
            string_buffer_append(buffer, generic_value_get_string(v));
            string_buffer_append(buffer, "\"");
            break;
        case GENERIC_VALUE_TYPE_OBJECT:
            string_buffer_append(buffer, "{\n");
            generic_object_t *object                = generic_value_get_object(v);
            generic_object_iterator_t *iterator     = generic_object_iterator_create(object);
            generic_object_key_value_pair_t *kvp    = NULL;

            while((kvp = generic_object_iterator_next(iterator)) != NULL) {
                char *kvp_key               = generic_object_kvp_get_key(kvp);
                generic_value_t *kvp_value  = generic_object_kvp_get_value(kvp);

                string_buffer_append(buffer, "\"");
                string_buffer_append(buffer, kvp_key);
                string_buffer_append(buffer, "\":");
                char* value_str = generic_value_to_string(kvp_value);
                if(value_str) {
                    string_buffer_append(buffer, value_str);
                    free(value_str);
                }

                size_t current_iterator_index   = generic_object_iterator_get_current_index(iterator);
                size_t object_size              = generic_object_get_size(object);

                if(current_iterator_index < object_size - 1) {
                    string_buffer_append(buffer, ",\n");
                }
            }
            string_buffer_append(buffer, "\n}");
            break;
        case GENERIC_VALUE_TYPE_ARRAY:
            string_buffer_append(buffer, "[");
            generic_array_t *array  = generic_value_get_array(v);
            size_t array_size       = generic_array_get_size(array);

            for(size_t i = 0; i < array_size; i++) {
                generic_value_t *element    = generic_array_get(array, i);
                char* value_str             = generic_value_to_string(element);

                if(value_str) {
                    string_buffer_append(buffer, value_str);
                    free(value_str);
                }
                
                if(i < array_size - 1) {
                    string_buffer_append(buffer, ",");
                }
            }
            string_buffer_append(buffer, "]");
            break;
        default:
            fprintf(stderr, "%s:%d: Unsupported type: %d\n", __func__, __LINE__, type);
            exit(EXIT_FAILURE);
        break;
    }

    char* buffer_content = strdup(buffer->content);
    if(!buffer_content) {
        fprintf(stderr, "%s:%s:%d: failed to duplicate buffer content for return\n", __FILE__, __func__, __LINE__);
        free(buffer);
        return NULL;
    }
    return buffer_content;
}
