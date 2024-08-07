#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "json/lexer.h"
#include "json/parser.h"
#include "json/token.h"

static void json_array_destroy(json_array_t *array);

static void json_object_destroy(json_object_t *destroy);

static void json_array_add(json_array_t *array, json_node_t *node) {
    if(array->size >= array->capacity) {
        size_t new_capacity = array->capacity * 2;
        json_node_t **new_nodes = realloc(array->nodes, sizeof(json_node_t*) * new_capacity);
        if(new_nodes == NULL) {
            fprintf(stderr, "%s:%s:%d: failed to allocate additional memory for array elements\n", __FILE__, __func__, __LINE__);
            exit(EXIT_FAILURE);
        }
        array->nodes = new_nodes;
        array->capacity = new_capacity;
    }

    array->nodes[array->size++] = node;
}

static void json_object_add(json_object_t *object, char *key, json_node_t *node) {
    if(object->size >= object->capacity) {
        size_t new_capacity = object->capacity * 2;
        json_object_entry_t **new_entries = realloc(object->entries, sizeof(json_object_entry_t*) * new_capacity);
        if(new_entries == NULL) {
            fprintf(stderr, "%s:%s:%d: failed to allocate additional memory for object entries\n", __FILE__, __func__, __LINE__);
            exit(EXIT_FAILURE);
        }
        object->entries = new_entries;
        object->capacity = new_capacity;
    }

    json_object_entry_t *entry = malloc(sizeof(json_object_entry_t));
    entry->key = strdup(key);
    entry->value = node;

    object->entries[object->size++] = entry;
}

json_node_t *json_object_get(json_object_t *object, char *key) {
    for(size_t i = 0; i < object->size; i++) {
        if(strcmp(key, object->entries[i]->key) == 0) {
            #ifndef NDEBUG
            printf("%s:%s:%d: found object entry by key %s at index %zu\n", __FILE__, __func__, __LINE__, key, i);
            #endif
            return object->entries[i]->value;
        }
    }
    #ifndef NDEBUG
    printf("%s:%s:%d: no object entry with key %s was found\n", __FILE__, __func__, __LINE__, key);
    #endif
    return NULL;
}

void json_node_destroy(json_node_t *node) {
    switch(node->type) {
        case JSON_NODE_TYPE_ARRAY:
            json_array_destroy((json_array_t*)node->value);
        break;
        case JSON_NODE_TYPE_OBJECT:
            json_object_destroy((json_object_t*)node->value);
        break;
        case JSON_NODE_TYPE_STRING:
        case JSON_NODE_TYPE_NUMBER:
        case JSON_NODE_TYPE_BOOL:
            free(node->value);
        break;
        case JSON_NODE_TYPE_UNDEFINED: // Nothing to do here.
        break;
    }

    free(node);
}

static void json_array_destroy(json_array_t *array) {
    for(size_t i = 0; i < array->size; i++) {
        json_node_destroy(array->nodes[i]);
    }
    free(array->nodes);
    free(array);
}

static void json_object_destroy(json_object_t *object) {
    for(size_t i = 0; i < object->size; i++) {
        json_object_entry_t *entry = object->entries[i];
        json_node_destroy(entry->value);
        free(entry->key);
        free(entry);
    }
    free(object->entries);
    free(object);
}

json_node_t *json_parse(json_token_t *tokens, size_t *current_token_index, bool is_root) {
    if (is_root && (tokens[*current_token_index].type != JSON_TOKEN_TYPE_LEFT_BRACE && tokens[*current_token_index].type != JSON_TOKEN_TYPE_LEFT_BRACKET)) {
        fprintf(stderr, "%s:%s:%d: expected array or object, but the first token was neither a left bracket nor a left brace (token_type = %d/%s)\n", __FILE__, __func__, __LINE__, tokens[*current_token_index].type, json_token_type_to_string(tokens[*current_token_index].type));
        return NULL;
    }

    json_node_t *node = malloc(sizeof(json_node_t));
    node->type = JSON_NODE_TYPE_UNDEFINED;
    node->value = NULL;
    void *temp = NULL;

    switch(tokens[*current_token_index].type) {
        case JSON_TOKEN_TYPE_LEFT_BRACKET:
            node->type = JSON_NODE_TYPE_ARRAY;
            node->value = json_parse_array(tokens, current_token_index);
        break;
        case JSON_TOKEN_TYPE_LEFT_BRACE:
            node->type = JSON_NODE_TYPE_OBJECT;
            node->value = json_parse_object(tokens, current_token_index);
        break;
        case JSON_TOKEN_TYPE_STRING:
            node->type = JSON_NODE_TYPE_STRING;
            size_t len = strlen(tokens[*current_token_index].value.string_value);
            temp = malloc(len + 1);
            strncpy(temp, tokens[*current_token_index].value.string_value, len);
            ((char*)temp)[len] = '\0';
            node->value = temp;
        break;
        case JSON_TOKEN_TYPE_NUMBER:
            node->type  = JSON_NODE_TYPE_NUMBER;
            temp        = malloc(sizeof(int));
            *(int*)temp = tokens[*current_token_index].value.int_value;
            node->value = temp;
        break;
        case JSON_TOKEN_TYPE_BOOL:
            node->type      = JSON_NODE_TYPE_BOOL;
            temp            = malloc(sizeof(bool));
            *(bool*)temp    = tokens[*current_token_index].value.boolean_value;
            node->value     = temp;
        break;
        default: // No handling necessary for remaining token types.
        break;
    }

    (*current_token_index)++;

    return node;
}

json_array_t *json_parse_array(json_token_t *tokens, size_t *current_token_index) {
    if(tokens[*current_token_index].type != JSON_TOKEN_TYPE_LEFT_BRACKET) {
        fprintf(stderr, "%s:%s:%d: the first token should be a left bracket when dealing with an array.\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    (*current_token_index)++; // Move past the left bracket.

    json_array_t *array = malloc(sizeof(json_array_t));
    array->capacity = 10;
    array->size = 0;
    array->nodes = calloc(array->capacity, sizeof(json_node_t*));

    while(true) {
        if(tokens[*current_token_index].type == JSON_TOKEN_TYPE_RIGHT_BRACKET) {
            return array;
        }

        json_node_t *node = json_parse(tokens, current_token_index, false);
        json_array_add(array, node);

        if (tokens[*current_token_index].type == JSON_TOKEN_TYPE_RIGHT_BRACKET) {
            return array;
        }

        if (tokens[*current_token_index].type != JSON_TOKEN_TYPE_COMMA) {
            fprintf(stderr, "%s:%s:%d: expected comma after object in array, but the current token is %s\n", __FILE__, __func__, __LINE__, json_token_type_to_string(tokens[*current_token_index].type));
            return NULL;
        }

        (*current_token_index)++; // Move past the comma.
    }

    fprintf(stderr, "%s:%s:%d: expected end of array, but the current token type is %d\n", __FILE__, __func__, __LINE__, tokens[*current_token_index].type);

    for(size_t i = 0; i < array->size; i++) {
        free(array[i].nodes[i]->value);
    }
    free(array->nodes);
    free(array);

    return NULL;
}


json_object_t *json_parse_object(json_token_t *tokens, size_t *current_token_index) {
    if(tokens[*current_token_index].type != JSON_TOKEN_TYPE_LEFT_BRACE) {
        fprintf(stderr, "%s:%s:%d: the first token should be a left brace when dealing with an object\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    (*current_token_index)++; // Move past the left brace.

    json_object_t *object = malloc(sizeof(json_object_t));
    object->capacity = 10;
    object->size = 0;
    object->entries = calloc(object->capacity, sizeof(json_object_entry_t*));

    while(true) {
        if(tokens[*current_token_index].type == JSON_TOKEN_TYPE_RIGHT_BRACE) { // Are we done?
            return object;
        }

        json_token_type_t json_key_type = tokens[*current_token_index].type;
        char *json_key_string_value     = tokens[*current_token_index].value.string_value;
        if(json_key_type != JSON_TOKEN_TYPE_STRING) {
            printf("Expected string key, but got %d (value: %s)\n", json_key_type, json_key_string_value);
            return NULL;
        }

        if(json_key_string_value == NULL) {
            printf("The string key is NULL.\n");
            return NULL;
        }

        (*current_token_index)++; // Move past the key.
        char *token_string_value = tokens[*current_token_index].value.string_value;

        if(tokens[*current_token_index].type != JSON_TOKEN_TYPE_COLON) { // Is there a colon?
            fprintf(stderr, "%s:%s:%d: expected colon, but got %d (value: %s)\n", __FILE__, __func__, __LINE__, tokens[*current_token_index].type, token_string_value);
            return NULL;
        }

        (*current_token_index)++; // Move past the colon.

        // We should be at the value now. Parse it.
        json_node_t *node = json_parse(tokens, current_token_index, false);
        json_object_add(object, json_key_string_value, node);

        // Are we done?
        if (tokens[*current_token_index].type == JSON_TOKEN_TYPE_RIGHT_BRACE) {
            return object;
        }

        if (tokens[*current_token_index].type != JSON_TOKEN_TYPE_COMMA) {
            fprintf(stderr, "%s:%s:%d: expected comma after pair in object, but got %d\n", __FILE__, __func__, __LINE__, tokens[*current_token_index].type);
            json_object_destroy(object);
            return NULL;
        }

        (*current_token_index)++; // Move past the comma.
    }

    fprintf(stderr, "%s:%s:%d: expected end of object\n", __FILE__, __func__, __LINE__);
    return NULL;
}

json_node_t *json_parse_string(char *str) {
    size_t length = 0;
    json_token_t *tokens = json_lex(str, &length);
    size_t current_token_index = 0;

    return json_parse(tokens, &current_token_index, true);
}
