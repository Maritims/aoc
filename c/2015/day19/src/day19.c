#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "aoc.h"
#include "file4c.h"
#include "generics4c.h"
#include "hashtable.h"
#include "string4c.h"

typedef struct {
    const char *key;
    size_t positions[1024];
    size_t count;
} KeyPosition;

KeyPosition **key_position_parse(char **keys, size_t keys_length, const char *str, bool verbose) {
    KeyPosition **key_positions = calloc(keys_length, sizeof(KeyPosition));

    if(key_positions == NULL) {
        fprintf(stderr, "%s:%d: Failed to allocate memory for key positions\n", __func__, __LINE__);
        return NULL;
    }

    for(size_t i = 0; i < keys_length; i++) {
        key_positions[i] = malloc(sizeof(KeyPosition));
        if(key_positions[i] == NULL) {
            fprintf(stderr, "%s:%d: Failed to allocate memory for key position at index %zu\n", __func__, __LINE__, i);
            for(size_t j = 0; j < i; j++) {
                free(key_positions[j]);
            }
        }

        const char *key     = keys[i];
        size_t key_length   = strlen(key);
        const char *pos     = str;
        size_t index        = 0;

        key_positions[i]->key    = key;
        key_positions[i]->count  = 0;

        while((pos = strstr(pos, key)) != NULL) {
            size_t position = pos - str;
            key_positions[i]->positions[index++] = position;
            key_positions[i]->count++;

            if(verbose) {
                printf("%s:%d: Key \"%s\" found at position %zu, occurrence #%zu\n", __func__, __LINE__, key, index - 1, key_positions[i]->count);
            }
            
            pos += key_length;
        }
    }

    return key_positions;
}

/**
 * parse_lines: Constructs the key table and translation map based on the lines from the input file by splitting them into tokens and storing specific tokens in the key table and translation map.
 * @param lines Lines from the input file.
 * @param number_of_lines The total number of lines from the input file.
 * @param key_table A double pointer to where the key table should be stored.
 * @param translation_map A double pointer to where the translation map should be stored.
 * @param translation_key_index The index in the tokens array describing where the translation key is contained.
 * @param translation_value_index The index in the tokens array describing where the translation value is contained.
 */
void parse_lines(char **lines, size_t number_of_lines, HashTable **key_table, GenericObject **translation_map, size_t translation_key_index, size_t translation_value_index) {
    *key_table          = hashtable_create(number_of_lines - 1);
    *translation_map    = generic_create_object(10);

    for(size_t i = 0; i < number_of_lines - 1; i++) {
        lines[i] = string_replace_all(lines[i], "Rn", "(");
        lines[i] = string_replace_all(lines[i], "Y", ",");
        lines[i] = string_replace_all(lines[i], "Ar", ")");

        char **tokens;
        size_t number_of_tokens;

        string_split(&tokens, &number_of_tokens, lines[i], " ");
        KeyValuePair *kvp = generic_get_pair_from_object(*translation_map, tokens[0]);
        
        if(kvp == NULL) {
            GenericArray *ga = generic_create_array(10);
            GenericValue *gv = generic_create_array_value(ga);
            generic_add_to_object(*translation_map, tokens[translation_key_index], gv);
            kvp = generic_get_pair_from_object(*translation_map, tokens[translation_key_index]);
        }

        hashtable_put_if_absent(*key_table, kvp->key, kvp->key, sizeof(char*), 0);
        
        GenericValue *gv_string = generic_create_string(tokens[translation_value_index]);
        GenericArray *gv_array = kvp->value->data.array_value;
        generic_add_to_array(gv_array, gv_string);

        for(size_t j = 0; j < number_of_tokens; j++) {
            free(tokens[j]);
        }

        free(tokens);
    }
}

int solve_part_one(char **lines, size_t number_of_lines) {
    GenericObject *translation_map;
    HashTable *key_table;
    parse_lines(lines, number_of_lines, &key_table, &translation_map, 0, 2);

    char *base                  = strdup(lines[number_of_lines - 1]);
    char **keys                 = hashtable_get_keys(key_table);
    size_t keys_length          = hashtable_get_size(key_table);
    KeyPosition **key_positions = key_position_parse(keys, keys_length, base, false);
    HashTable *results          = hashtable_create(10);

    for(size_t k = 0; k < keys_length; k++) {
        KeyPosition *key_position   = key_positions[k];
        size_t key_length           = strlen(key_position->key);
        KeyValuePair *kvp           = generic_get_pair_from_object(translation_map, key_position->key);
        GenericArray *array         = kvp->value->data.array_value;

        for(size_t p = 0; p < key_position->count; p++) {
            //printf("%s => \n", kvp->key);
            for(size_t t = 0; t < array->size; t++) {
                char *translation = array->elements[t]->data.string_value;
                char *result = string_replace_at(base, translation, key_position->positions[p], key_length);
                //printf("\t%s => %s\n", translation, result);
                hashtable_put(results, result, result, strlen(result), 0);

                free(result);
            }
        }
    }

    size_t result = hashtable_get_size(results);
    free(keys);
    free(key_positions);
    hashtable_destroy(results);

    return result;
}

int solve_part_two(char **lines, size_t number_of_lines) {
    GenericObject *translation_map;
    HashTable *key_table;
    parse_lines(lines, number_of_lines, &key_table, &translation_map, 2, 0);
    
    char *result    = strdup(lines[number_of_lines - 1]);
    result          = string_replace_all(result, "Rn", "(");
    result          = string_replace_all(result, "Y", ",");
    result          = string_replace_all(result, "Ar", ")");
    size_t step     = 0;

    while(strcmp(result, "e") != 0) {
        for(size_t i = 0; i < translation_map->size; i++) {
            KeyValuePair *kvp   = translation_map->pairs[i];
            result              = string_replace_all(result, kvp->key, kvp->value->data.array_value->elements[0]->data.string_value);
            step++;

            if(strcmp(result, "e") == 0) {
                return step;
            }
        }
    }

    return step;
}

int main(int argc, char *argv[]) {
    Solution solution;
    char **lines = NULL;
    size_t number_of_lines;
    
    solution_create(&solution, 2015, 19);
    file_read_all_lines(&lines, &number_of_lines, argv[1]);
    
    if(lines == NULL) {
        fprintf(stderr, "%s:%d: failed to read lines from file %s\n", __func__, __LINE__, argv[1]);
        return 1;
    }

    solution_part_finalize_with_int(&solution, 0, solve_part_one(lines, number_of_lines), "535");
    solution_part_finalize_with_int(&solution, 1, solve_part_two(lines, number_of_lines), "212");
    
    free(lines);

    return solution_finalize(&solution);
}
