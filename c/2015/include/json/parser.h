#include <stdbool.h>
#include <stdio.h>
#include "json/token.h"

typedef enum json_node_type_t {
    JSON_NODE_TYPE_UNDEFINED = 1,
    JSON_NODE_TYPE_ARRAY,
    JSON_NODE_TYPE_BOOL,
    JSON_NODE_TYPE_OBJECT,
    JSON_NODE_TYPE_NUMBER,
    JSON_NODE_TYPE_STRING,
} json_node_type_t;

typedef struct json_node_t {
    json_node_type_t type;
    void* value;
} json_node_t;

typedef struct json_array_t {
    size_t capacity;
    size_t size;
    json_node_t** nodes;
} json_array_t;

typedef struct json_object_entry_t {
    char *key;
    json_node_t *value;
} json_object_entry_t;

typedef struct json_object_t {
    size_t capacity;
    size_t size;
    json_object_entry_t **entries;
} json_object_t;

void json_node_destroy(json_node_t *node);

json_node_t *json_object_get(json_object_t *object, char *key);

/**
 * Parse JSON tokens recursively. Determines whether to parse an object, array or primitive value and delegates to the appropriate parser method.
 * @param tokens JSON token array.
 * @param current_token_index Position in array.
 * @return The resulting generic value object.
 */
json_node_t *json_parse(json_token_t *tokens, size_t *current_token_index, bool is_root);

/**
 * Parser method for JSON arrays. Expects the current token to be the beginning of a JSON array.
 * @param tokens JSON token array.
 * @param current_token_index Position in array.
 * @return The resulting generic array object.
 */
json_array_t *json_parse_array(json_token_t *tokens, size_t *current_token_index);

/**
 * Parser method for JSON objects. Expects the current token to be the beginning of a JSON object.
 * @param tokens JSON token array.
 * @param current_token_index Position in array.
 * @return The resulting generic object.
 */
json_object_t *json_parse_object(json_token_t *tokens, size_t *current_token_index);

/**
 * Parse a JSON string into a generic value object.
 * @param result A pointer to a heap allocated generic value to hold the parsed result.
 * @param str The JSON string to parse.
 */
json_node_t *json_parse_string(char *str);
