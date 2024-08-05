#include <stdbool.h>
#include "generics/types.h"
#include "json/token.h"

/**
 * Parse JSON tokens recursively. Determines whether to parse an object, array or primitive value and delegates to the appropriate parser method.
 * @param tokens JSON token array.
 * @return The resulting generic value object.
 */
generic_value_t *json_parse(json_token_t **tokens, bool is_root);

/**
 * Parser method for JSON arrays. Expects the current token to be the beginning of a JSON array.
 * @param JSON token array.
 * @return The resulting generic array object.
 */
generic_array_t *json_parse_array(json_token_t **tokens);

/**
 * Parser method for JSON objects. Expects the current token to be the beginning of a JSON object.
 * @param tokens JSON token array.
 * @return The resulting generic object.
 */
generic_object_t *json_parse_object(json_token_t **tokens);

/**
 * Parse a JSON string into a generic value object.
 * @param result A pointer to a heap allocated generic value to hold the parsed result.
 * @param str The JSON string to parse.
 */
void json_parse_string(generic_value_t **result, char *str);
