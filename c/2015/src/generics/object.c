#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "generics/object.h"
#include "generics/value.h"
#include "generics/types.h"

struct generic_object_key_value_pair_t {
    char*               key;
    generic_value_t*    value;
};

struct generic_object_t {
    generic_object_key_value_pair_t**   pairs;
    size_t                              capacity;
    size_t                              size;
};

struct generic_object_iterator_t {
    size_t                              current_index;
    generic_object_t*                   object;
    generic_object_key_value_pair_t*    current_entry;
};

generic_object_t *generic_object_create(size_t capacity) {
    if(capacity == 0) {
        fprintf(stderr, "%s:%s:%d: parameter capacity was 0\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    generic_object_t *o = malloc(sizeof(generic_object_t));
    if(o == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to allocate memory for object\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    o->pairs = calloc(capacity, sizeof(generic_object_key_value_pair_t*));
    if(o->pairs == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to allocate memory for object pairs\n", __FILE__, __func__, __LINE__);
        free(o);
        return NULL;
    }

    o->capacity = capacity;
    o->size = 0;

    return o;
}

void generic_object_destroy(generic_value_t *wrapped_object) {
    if(wrapped_object == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter wrapped_object was null\n", __FILE__, __func__, __LINE__);
        return;
    }

    generic_object_t *object = generic_value_get_object(wrapped_object);
    if(object == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to unwrap object\n", __FILE__, __func__, __LINE__);
        return;
    }

    for(size_t i = 0; i < object->size; i++) {
        generic_object_key_value_pair_t *kvp    = object->pairs[i];
        generic_value_t *kvp_value              = kvp->value;

        free(kvp->key);
        generic_value_destroy(kvp_value);
        free(kvp);
    }

    free(object->pairs);
    free(object);
}

generic_value_t *generic_object_get(generic_object_t *o, const char *p) {
    if(o == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter o was null\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    if(p == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter p was null\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    if(strlen(p) == 0) {
        fprintf(stderr, "%s:%s:%d: parameter p was empty\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    for(size_t i = 0; i < o->size; i++) {
        if(strcmp(p, o->pairs[i]->key) != 0) {
            continue;
        }
        
        generic_object_key_value_pair_t *kvp = o->pairs[i];
        return kvp->value;
    }

    return NULL;
}

size_t generic_object_get_size(generic_object_t *object) {
    if(object == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter object was null\n", __FILE__, __func__, __LINE__);
        return INT_MIN;
    }

    return object->size;
}

void generic_object_add(generic_object_t *o, const char *p, generic_value_t *v) {
    if(o == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter o was null\n", __FILE__, __func__, __LINE__);
        return;
    }

    if(p == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter p was null\n", __FILE__, __func__, __LINE__);
        return;
    }

    if(strlen(p) == 0) {
        fprintf(stderr, "%s:%s:%d: parameter p was empty\n", __FILE__, __func__, __LINE__);
        return;
    }

    if(o->size >= o->capacity) {
        size_t new_capacity = o->capacity * 2;
        generic_object_key_value_pair_t **new_pairs = realloc(o->pairs, new_capacity * sizeof(generic_object_key_value_pair_t*));
        if(new_pairs == NULL) {
            fprintf(stderr, "%s:%s:%d: failed to allocate additional memory for object key value pairs\n", __FILE__, __func__, __LINE__);
            return;
        }

        o->capacity = new_capacity;
        o->pairs    = new_pairs;
    }

    generic_object_key_value_pair_t *kvp = malloc(sizeof(generic_object_key_value_pair_t));
    if(kvp == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to allocate memory for object key value pair\n", __FILE__, __func__, __LINE__);
        return;
    }

    kvp->key = strdup(p);
    if(kvp->key == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to duplicate key for object key value pair\n", __FILE__, __func__, __LINE__);
        return;
    }

    kvp->value = v;
    o->pairs[o->size++] = kvp;
}

generic_object_key_value_pair_t *generic_object_kvp_get(generic_object_t *object, size_t i) {
    if(object == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter object was null\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    if(i >= object->size) {
        fprintf(stderr, "%s:%s:%d: index %zu was out of bounds\n", __FILE__, __func__, __LINE__, i);
        return NULL;
    }

    return object->pairs[i];
}

char *generic_object_kvp_get_key(generic_object_key_value_pair_t *kvp) {
    if(kvp == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter kvp was null\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    return kvp->key;
}

generic_value_t *generic_object_kvp_get_value(generic_object_key_value_pair_t *kvp) {
    if(kvp == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter kvp was null\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    return kvp->value;
}

generic_object_iterator_t *generic_object_iterator_create(generic_object_t *o) {
    if(o == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter o was null\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    generic_object_iterator_t *iterator = malloc(sizeof(generic_object_iterator_t));
    if(iterator == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to allocate memory for iterator\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    iterator->current_index = 0;
    iterator->object = o;
    iterator->current_entry = NULL;

    return iterator;
}

size_t generic_object_iterator_get_current_index(generic_object_iterator_t *iterator) {
    if(iterator == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter iterator was null\n", __FILE__, __func__, __LINE__);
        return INT_MIN;
    }

    return iterator->current_index;
}

generic_object_key_value_pair_t *generic_object_iterator_next(generic_object_iterator_t *iterator) {
    if(iterator == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter iterator was null\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    generic_object_t *object = iterator->object;
    while(iterator->current_index < object->capacity) {
        if(iterator->current_entry == NULL) {
            iterator->current_entry = object->pairs[iterator->current_index];
        }

        if(iterator->current_entry != NULL) {
            return iterator->current_entry;
        }

        iterator->current_index++;
    }

    return NULL;
}
