#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc.h"
#include "file4c.h"
#include "hashtable.h"
#include "string4c.h"

#define DEBUG 1

// Recursive function for resolving an instruction from Little Bobby's booklet.
uint16_t resolve(HashTable *table, char *key, HashTable *cache)
{
    // We finally arrived at an actual value and not another reference!
    if (string_is_numeric(key)) {
        return (uint16_t)atoi(key);
    }
    
    // But is it in the cache?
    HashTableEntry *cached_entry = hashtable_get(cache, key);
    if(cached_entry != NULL) {
        uint16_t cached_value = *(uint16_t*)cached_entry->value;
        return cached_value;
    }

    HashTableEntry *entry = hashtable_get(table, key);
    char **tokens;
    size_t number_of_tokens;
    string_split(&tokens, &number_of_tokens, entry->value, " ");
    uint16_t result;

    if (number_of_tokens == 1) {
        // Assignment
        result = resolve(table, tokens[0], cache);
    }
    else if (number_of_tokens == 2) {
        // NOT
        result = ~resolve(table, tokens[1], cache);
    }
    else if (number_of_tokens == 3) {
        // AND, OR, LSHIFT, RSHIFT
        if (strcmp(tokens[1], "AND") == 0) {
            result = resolve(table, tokens[0], cache) & resolve(table, tokens[2], cache);
        }
        else if (strcmp(tokens[1], "OR") == 0) {
            result = resolve(table, tokens[0], cache) | resolve(table, tokens[2], cache);
        }
        else if (strcmp(tokens[1], "LSHIFT") == 0) {
            result = resolve(table, tokens[0], cache) << resolve(table, tokens[2], cache);
        }
        else if (strcmp(tokens[1], "RSHIFT") == 0) {
            result = resolve(table, tokens[0], cache) >> resolve(table, tokens[2], cache);
        }
    }

    // Cache the resulting value.
    hashtable_put(cache, key, &result, sizeof(uint16_t), 0);
    return result;
}

int main(int argc, char *argv[])
{
    Solution solution;
    char **lines;
    size_t number_of_lines;

    solution_create(&solution, 2015, 7);
    file_read_all_lines(&lines, &number_of_lines, argv[1]);

    HashTable *table = hashtable_create(number_of_lines);
    HashTable *cache = hashtable_create(number_of_lines);

    for (size_t i = 0; i < number_of_lines; i++)
    {
        char *line = lines[i];
        char **tokens;
        size_t number_of_tokens = 0;

        string_split(&tokens, &number_of_tokens, line, "->");
        char *trimmed_value = string_trim(tokens[0]);
        char *trimmed_key = string_trim(tokens[1]);
        hashtable_put(table, string_trim(trimmed_key), string_trim(trimmed_value), strlen(trimmed_value), 0);

        free(trimmed_value);
        free(tokens);
    }

    uint16_t value = resolve(table, "a", cache);
    sprintf(solution.part_one.result, "%u", value);
    solution_part_finalize(&solution.part_one, "16076");

    HashTableEntry *entry = hashtable_put(table, "b", solution.part_one.result, strlen(solution.part_one.result), 0);
    hashtable_destroy(cache);
    cache = hashtable_create(number_of_lines);
    value = resolve(table, "a", cache);
    sprintf(solution.part_two.result, "%u", value);
    solution_part_finalize(&solution.part_two, "2797");

    solution_finalize(&solution);
    solution_print(&solution);

    free(lines);
    hashtable_destroy(table);
    hashtable_destroy(cache);

    return 0;
}
