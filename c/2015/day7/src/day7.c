#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc.h"
#include "debug.h"
#include "hashtable.h"
#include "string4c.h"

#define DEBUG 1

// Recursive function for resolving an instruction from Little Bobby's booklet.
uint16_t resolve(HashTable *table, char *key, HashTable *cache)
{
    // We finally arrived at an actual value and not another reference!
    if (string_is_numeric(key))
    {
        return (uint16_t)atoi(key);
    }
    
    // But is it in the cache?
    HashTableEntry *cached_entry = hashtable_get(cache, key);
    if(cached_entry != NULL)
    {
        uint16_t cached_value = *(uint16_t*)cached_entry->value;
        return cached_value;
    }

    HashTableEntry *entry = hashtable_get(table, key);
    size_t length = 0;
    char **command = string_split(entry->value, " ", &length);
    uint16_t result;

    if (length == 1)
    {
        // Assignment
        result = resolve(table, command[0], cache);
    }
    else if (length == 2)
    {
        // NOT
        result = ~resolve(table, command[1], cache);
    }
    else if (length == 3)
    {
        // AND, OR, LSHIFT, RSHIFT
        if (strcmp(command[1], "AND") == 0)
        {
            result = resolve(table, command[0], cache) & resolve(table, command[2], cache);
        }
        else if (strcmp(command[1], "OR") == 0)
        {
            result = resolve(table, command[0], cache) | resolve(table, command[2], cache);
        }
        else if (strcmp(command[1], "LSHIFT") == 0)
        {
            result = resolve(table, command[0], cache) << resolve(table, command[2], cache);
        }
        else if (strcmp(command[1], "RSHIFT") == 0)
        {
            result = resolve(table, command[0], cache) >> resolve(table, command[2], cache);
        }
    }

    // Cache the resulting value.
    hashtable_put(cache, key, &result, sizeof(uint16_t), 0);
    return result;
}

int main(int argc, char *argv[])
{
    Solution *solution = solution_create(2015, 7);

    size_t length;
    char **lines = get_file_as_lines(argv[1], &length);

    HashTable *table = hashtable_create(length);
    HashTable *cache = hashtable_create(length);

    for (size_t i = 0; i < length; i++)
    {
        char *line = lines[i];
        size_t number_of_elements = 0;
        char **elements = string_split(line, "->", &number_of_elements);
        char *trimmed_value = string_trim(elements[0]);
        char *trimmed_key = string_trim(elements[1]);
        hashtable_put(table, string_trim(trimmed_key), string_trim(trimmed_value), strlen(trimmed_value), 0);

        free(trimmed_value);
        free(elements);
    }

    uint16_t value = resolve(table, "a", cache);
    sprintf(solution->part_one.result, "%u", value);
    solution_part_finalize(&solution->part_one);

    HashTableEntry *entry = hashtable_put(table, "b", solution->part_one.result, strlen(solution->part_one.result), 0);
    hashtable_destroy(cache);
    cache = hashtable_create(length);
    value = resolve(table, "a", cache);
    sprintf(solution->part_two.result, "%u", value);
    solution_part_finalize(&solution->part_two);

    solution_finalize(solution);
    solution_print(solution);

    free(lines);
    free(solution);
    hashtable_destroy(table);
    hashtable_destroy(cache);

    return 0;
}