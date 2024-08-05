#include <stdio.h>
#include <string.h>

#include "debug.h"
#include "../generics4c.h"
#include "string4c.h"

#define DEBUG 1

struct GenericArray {
    GenericValue**  elements;
    size_t          size;
    size_t          capacity;
};

struct GenericObject {
    KeyValuePair**  pairs;
    size_t          size;
    size_t          capacity;
};

struct GenericValue {
    GenericValueData data;
    GenericValueType type;
    void (*destructor)(struct GenericValue*);
};

struct KeyValuePair {
    char*           key;
    GenericValue*   value;
};

static void destroy_generic_value(GenericValue* generic_value);

// Creators
GenericValue* generic_create_bool(bool value) {
    GenericValue* generic_value = malloc(sizeof(GenericValue));
    generic_value->type = GENERIC_TYPE_BOOL;
    generic_value->data.boolean_value = value;
    generic_value->destructor = destroy_generic_value;
    return generic_value;
}

GenericValue* generic_create_int(int value) {
    GenericValue* generic_value = malloc(sizeof(GenericValue));
    generic_value->type = GENERIC_TYPE_INT;
    generic_value->data.int_value = value;
    generic_value->destructor = destroy_generic_value;
    return generic_value;
}

GenericValue* generic_create_string(const char* value) {
    if(value == NULL) {
        debug_print("The parameter \"value\" cannot be NULL.\n", "");
        return NULL;
    }

    GenericValue* generic_value = malloc(sizeof(GenericValue));
    if(generic_value == NULL) {
        printf("Failed to allocate memory for the generic value\n");
        return NULL;
    }

    generic_value->type = GENERIC_TYPE_STRING;
    generic_value->data.string_value = strdup(value);
    
    if(generic_value->data.string_value == NULL) {
        printf("Failed to duplicate the value for the generic value\n");
        free(generic_value);
        return NULL;
    }

    generic_value->destructor = destroy_generic_value;
    return generic_value;
}

GenericArray* generic_create_array(size_t capacity) {
    GenericArray *ga = malloc(sizeof(GenericArray));
    if(ga == NULL) {
        fprintf(stderr, "%s:%d: Failed to allocate memory for array\n", __func__, __LINE__);
        return NULL;
    }

    ga->elements = calloc(capacity, sizeof(GenericValue*));
    if(ga->elements == NULL) {
        fprintf(stderr, "%s:%d: Failed to allocate memory for array elements\n", __func__, __LINE__);
        free(ga);
        return NULL;
    }

    ga->size = 0;
    ga->capacity = capacity;
    
    return ga;
}

GenericValue *generic_create_array_value(GenericArray *value) {
    if(value == NULL) {
        fprintf(stderr, "%s:%d: The parameter \"value\" cannot be NULL", __func__, __LINE__);
        return NULL;
    }

    GenericValue *gv = malloc(sizeof(GenericValue));
    if(gv == NULL) {
        fprintf(stderr, "%s:%d: Failed to allocate memory for the generic value\n", __func__, __LINE__);
        return NULL;
    }

    gv->type = GENERIC_TYPE_ARRAY;
    gv->data.array_value = value;
    gv->destructor = destroy_generic_value;

    //printf("%s:%d: type = %d\n", __func__, __LINE__, gv->type);
    
    return gv;
}

GenericObject* generic_create_object(size_t capacity) {
    if(capacity == 0) {
        printf("Parameter \"capacity\" cannot be %zu\n", capacity);
        return NULL;
    }

    GenericObject* generic_object = malloc(sizeof(GenericObject));
    if(generic_object == NULL) {
        printf("Failed to allocate memory for generic object\n");
        return NULL;
    }

    generic_object->pairs = calloc(capacity, sizeof(KeyValuePair*));
    if(generic_object->pairs == NULL) {
        printf("Failed to allocate memory for generic object pairs.\n");
        return NULL;
    }

    generic_object->size = 0;
    generic_object->capacity = capacity;
    return generic_object;
}

GenericValue *generic_create_object_value(GenericObject *value) {
    if(value == NULL) {
        debug_print("The parameter \"value\" cannot be NULL.\n", "");
        return NULL;
    }

    GenericValue *gv = malloc(sizeof(GenericValue));
    if(gv == NULL) {
        printf("Failed to allocate memory for the generic value\n");
        return NULL;
    }

    gv->type = GENERIC_TYPE_OBJECT;
    gv->data.object_value = value;
    gv->destructor = destroy_generic_value;
    return gv;
}

// Destructors
void destroy_generic_value(GenericValue *gv) {
    if (!gv) {
        return;
    }

    switch (gv->type) {
        case GENERIC_TYPE_STRING:
            free(gv->data.string_value);
        break;
        case GENERIC_TYPE_OBJECT:
            generic_destroy_object(gv->data.object_value);
        break;
        case GENERIC_TYPE_ARRAY:
            generic_destroy_array(gv->data.array_value);
        break;
    }

    free(gv);
}

void generic_destroy_array(GenericArray* generic_array) {
    for (size_t i = 0; i < generic_array->size; i++) {
        free(generic_array->elements[i]);
    }
    free(generic_array->elements);
    free(generic_array);
}

void generic_destroy_object(GenericObject *go) {
    for (size_t i = 0; i < go->size; i++) {
        free(go->pairs[i]->key);
        go->pairs[i]->value->destructor(go->pairs[i]->value);
        free(go->pairs[i]);
    }
    free(go->pairs);
    free(go);
}

// Getters
GenericArray *generic_get_array(GenericValue *gv) {
    if(gv->type != GENERIC_TYPE_ARRAY) {
        fprintf(stderr, "%s:%d: Attempted to retrieve array value from generic value with type %d\n", __func__, __LINE__, gv->type);
        return NULL;
    }

    return gv->data.array_value;
}

char *generic_get_string(GenericValue *gv) {
    if(gv == NULL) {
        fprintf(stderr, "%s:%d: Parameter \"gv\" cannot be NULL\n", __func__, __LINE__);
        return NULL;
    }

    if(gv->type != GENERIC_TYPE_STRING) {
        fprintf(stderr, "%s:%d: Attempted to retrieve string value from generic value with type %d\n", __func__, __LINE__, gv->type);
        return NULL;
    }

    return gv->data.string_value;
}

void *generic_get_anything(GenericValue *gv) {
    if(gv->data.array_value != NULL) {
        printf("%s:%d: Array!\n", __func__, __LINE__);
        return gv->data.array_value;
    }

    if(gv->data.string_value != NULL) {
        printf("%s:%d: String!\n", __func__, __LINE__);
        return gv->data.string_value;
    }

    if(gv->data.object_value != NULL) {
        printf("%s:%d: Object!\n", __func__, __LINE__);
        return gv->data.object_value;
    }

    return NULL;
}

// Arrays
size_t generic_get_array_size(GenericArray *ga) {
    return ga->size;
}

void generic_add_to_array(GenericArray* generic_array, GenericValue* generic_value) {
    if(generic_array == NULL) {
        fprintf(stderr, "%s:%d: Parameter \"generic_array\" cannot be NULL\n", __func__, __LINE__);
        return;
    }

    if (generic_array->size >= generic_array->capacity) {
        size_t new_capacity = generic_array->capacity * 2;
        GenericValue **new_elements = realloc(generic_array->elements, new_capacity * sizeof(GenericValue*));
        if(new_elements == NULL) {
            fprintf(stderr, "%s:%d: Failed to allocate additional memory with capacity for %zu array elements in total\n", __func__, __LINE__, new_capacity);
            return;
        }

        generic_array->capacity = new_capacity;
        generic_array->elements = new_elements;
    }

    generic_array->elements[generic_array->size++] = generic_value;
}

GenericValue *generic_get_from_array(GenericArray *ga, size_t pos) {
    if(ga == NULL) {
        fprintf(stderr, "%s:%d: Parameter \"ga\" cannot be NULL\n", __func__, __LINE__);
        return NULL;
    }

    if(pos >= ga->size) {
        fprintf(stderr, "%s:%d: Index %zu is out of bounds\n", __func__, __LINE__, pos);
        return NULL;
    }

    return ga->elements[pos];
}

// Objects
void generic_add_to_object(GenericObject* generic_object, const char* key, GenericValue* generic_value) {
    if(key == NULL) {
        printf("%s:%s(): Parameter \"key\" cannot be NULL\n", __FILE__, __func__);
        return;
    }

    if(strlen(key) == 0) {
        printf("%s:%s(): Parameter \"key\" cannot be empty\n", __FILE__, __func__);
        return;
    }

    if (generic_object->size >= generic_object->capacity) {
        generic_object->capacity *= 2;
        generic_object->pairs = realloc(generic_object->pairs, generic_object->capacity * sizeof(KeyValuePair*));
        if(generic_object->pairs == NULL) {
            printf("Failed to allocate additional memory for key value pairs.\n");
            return;
        }
    }

    KeyValuePair* key_value_pair = malloc(sizeof(KeyValuePair));
    if(key_value_pair == NULL) {
        printf("Failed to allocate memory for key value pair.\n");
        return;
    }

    key_value_pair->key = strdup(key);
    if(key_value_pair->key == NULL) {
        printf("Failed to duplicate key for key value pair.\n");
        return;
    }

    key_value_pair->value = generic_value;
    generic_object->pairs[generic_object->size++] = key_value_pair;
}

GenericValue *generic_get_from_object(GenericObject *go, const char *key) {
    KeyValuePair *kvp = generic_get_pair_from_object(go, key);
    return kvp ? kvp->value : NULL;
}

// Values
GenericValueType generic_get_type(GenericValue *gv) {
    return gv->type;
}

// Deprecated
KeyValuePair* generic_get_pair_from_object(GenericObject* generic_object, const char* key) {
    if(generic_object == NULL) {
        fprintf(stderr, "Parameter \"generic_object\" cannot be NULL\n");
        return NULL;
    }

    if(key == NULL) {
        fprintf(stderr, "Parameter \"key\" cannot be NULL\n");
        return NULL;
    }

    if(strlen(key) == 0) {
        fprintf(stderr, "Parameter \"key\" cannot be empty\n");
        return NULL;
    }

    for(size_t i = 0; i < generic_object->size; i++) {
        if(strcmp(key, generic_object->pairs[i]->key) == 0) {
            return generic_object->pairs[i];
        }
    }

    return NULL;
}

// Convenience
void generic_print_value(GenericValue *gv) {
    if(gv == NULL) {
        fprintf(stderr, "%s:%d: Parameter \"gv\" cannot be NULL\n", __func__, __LINE__);
        return;
    }

    char *str = generic_value_to_string(gv);
    printf("%s\n", str);
    free(str);
}

char *generic_value_to_string(GenericValue *generic_value) {
    if(!generic_value) {
        fprintf(stderr, "%s:%d: Parameter \"generic_value\" cannot be NULL.\n", __func__, __LINE__);
        return NULL;
    }

    StringBuffer* buffer = string_buffer_create(1024);
    if(!buffer) {
        fprintf(stderr, "%s:%d: Failed to allocate string buffer.\n", __func__, __LINE__);
        return NULL;
    }

    switch(generic_value->type) {
        case GENERIC_TYPE_BOOL:
            string_buffer_append(buffer, generic_value->data.boolean_value ? "true" : "false");
            break;
        case GENERIC_TYPE_INT:
            char temp[32];
            snprintf(temp, sizeof(temp), "%d", generic_value->data.int_value);
            string_buffer_append(buffer, temp);
            break;
        case GENERIC_TYPE_STRING:
            string_buffer_append(buffer, "\"");
            string_buffer_append(buffer, generic_value->data.string_value);
            string_buffer_append(buffer, "\"");
            break;
        case GENERIC_TYPE_OBJECT:
            string_buffer_append(buffer, "{\n");
            for(size_t i = 0; i < generic_value->data.object_value->size; i++) {
                KeyValuePair* kvp = generic_value->data.object_value->pairs[i];
                string_buffer_append(buffer, "\"");
                string_buffer_append(buffer, kvp->key);
                string_buffer_append(buffer, "\":");
                char* value_str = generic_value_to_string(kvp->value);
                if(value_str) {
                    string_buffer_append(buffer, value_str);
                    free(value_str);
                }
                if(i < generic_value->data.object_value->size - 1) {
                    string_buffer_append(buffer, ",\n");
                }
            }
            string_buffer_append(buffer, "\n}");
            break;
        case GENERIC_TYPE_ARRAY:
            string_buffer_append(buffer, "[");
            for(size_t i = 0; i < generic_value->data.array_value->size; i++) {
                char* value_str = generic_value_to_string(generic_value->data.array_value->elements[i]);
                if(value_str) {
                    string_buffer_append(buffer, value_str);
                    free(value_str);
                }
                if(i < generic_value->data.array_value->size - 1) {
                    string_buffer_append(buffer, ",");
                }
            }
            string_buffer_append(buffer, "]");
            break;
        default:
            fprintf(stderr, "%s:%d: Unsupported type: %d\n", __func__, __LINE__, generic_value->type);
            exit(EXIT_FAILURE);
        break;
    }

    char* buffer_content = strdup(buffer->content);
    if(!buffer_content) {
        debug_print("Unable to duplicate buffer content for return.\n", "");
        free(buffer);
        return NULL;
    }
    return buffer_content;
}

bool generic_compare_value(GenericValue* gv1, GenericValue* gv2) {
    if (gv1->type != gv2->type) {
        return false;
    }

    switch (gv1->type) {
        case GENERIC_TYPE_BOOL:
            return gv1->data.boolean_value == gv2->data.boolean_value;
        case GENERIC_TYPE_INT:
            return gv1->data.int_value == gv2->data.int_value ? true : false;
        case GENERIC_TYPE_STRING:
            return strcmp(gv1->data.string_value, gv2->data.string_value) == 0 ? true : false;
    }

    fprintf(stderr, "Unsupported type: %d\n", gv1->type);
    return false;
}

bool generic_compare_kvp(KeyValuePair* kvp1, KeyValuePair* kvp2) {
    return strcmp(kvp1->key, kvp2->key) == 0 && generic_compare_value(kvp1->value, kvp2->value) == true ? true : false;
}
