#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aoc.h"
#include "file4c.h"
#include "hashtable.h"
#include "string4c.h"
#include "testing/assertions.h"

uint16_t resolve(HashTable *table, char *key, HashTable *cache) {
    // We finally arrived at an actual value and not another reference!
    if (string_is_numeric(key)) {
        return (uint16_t)atoi(key);
    }
    
    // But is it in the cache?
    HashTableEntry *cached_entry = hashtable_get(cache, key);
    if(cached_entry != NULL) {
        void *value = hashtable_entry_get_value(cached_entry);
        char *cached_value = (char*)value;
        return (uint16_t)atoi(cached_value);
    }

    HashTableEntry *entry   = hashtable_get(table, key);
    void *value             = hashtable_entry_get_value(entry);
    size_t number_of_tokens = 0;
    char **tokens = string_split(&number_of_tokens, value, " ");
    uint16_t result = 0;

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
            uint16_t lhs = resolve(table, tokens[0], cache);
            uint16_t rhs = resolve(table, tokens[2], cache);
            result = lhs >> rhs;
        }
    }

    for(size_t i = 0; i < number_of_tokens; i++) {
        free(tokens[i]);
    }
    free(tokens);

    // Cache the resulting value.
    char *buffer = string_from_number(result);
    size_t buffer_length = strlen(buffer);
    hashtable_put(cache, key, buffer, buffer_length);
    free(buffer);
    return result;
}

void parse_lines(char **lines, size_t number_of_lines, HashTable *table) {
    for (size_t i = 0; i < number_of_lines; i++)
    {
        size_t number_of_tokens = 0;
        char **tokens = string_split(&number_of_tokens, lines[i], "->");
        char *trimmed_value = string_trim(tokens[0]);
        char *trimmed_key = string_trim(tokens[1]);
        hashtable_put(table, trimmed_key, trimmed_value, strlen(trimmed_value));

        free(trimmed_key);
        free(trimmed_value);
        for(size_t i = 0; i < number_of_tokens; i++) {
            free(tokens[i]);
        }
        free(tokens);
    }
}

void test_resolve(char *key, uint16_t expected_result) {
    size_t number_of_lines = 0;
    char **lines = file_read_all_lines(&number_of_lines, "data/day7_test.txt");
    HashTable *table = hashtable_create(8);
    HashTable *cache = hashtable_create(8);

    parse_lines(lines, number_of_lines, table);
    
    uint16_t result = resolve(table, key, cache);
    assert_primitive_equality(expected_result, result, "resolve(table, \"%s\", cache) != %d (%d)\n", key, expected_result, result);
    printf("%s(\"%s\", %d) passed\n", __func__, key, expected_result);

    hashtable_destroy(table);
    hashtable_destroy(cache);
    FREE_ARRAY(lines, number_of_lines);
}

int main(int argc, char *argv[])
{
    (void)argc;

    test_resolve("d", 72);
    test_resolve("e", 507);
    test_resolve("f", 492);
    test_resolve("g", 114);
    test_resolve("h", 65412);
    test_resolve("i", 65079);
    test_resolve("x", 123);
    test_resolve("y", 456);

    solution_t *solution = solution_create(2015, 7);
    size_t number_of_lines = 0;
    char **lines = file_read_all_lines(&number_of_lines, argv[1]);

    HashTable *table = hashtable_create(number_of_lines);
    HashTable *cache = hashtable_create(number_of_lines);

    parse_lines(lines, number_of_lines, table);

    uint16_t value = resolve(table, "a", cache);
    solution_part_finalize_with_int(solution, 0, value, "16076");

    hashtable_put(table, "b", solution->parts[0].result, strlen(solution->parts[0].result));
    hashtable_destroy(cache);
    cache = hashtable_create(number_of_lines);
    value = resolve(table, "a", cache);
    solution_part_finalize_with_int(solution, 1, value, "2797");

    FREE_ARRAY(lines, number_of_lines);
    hashtable_destroy(table);
    hashtable_destroy(cache);
    return solution_finalize_and_destroy(solution);
}
