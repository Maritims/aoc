#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc.h"
#include "file4c.h"
#include "hashtable.h"
#include "string4c.h"
#include "test4c.h"

#define DEBUG 1

uint32_t get_vowels(const char *input) {
    uint32_t vowels = 0;
    for (uint32_t i = 0; i < strlen(input); i++)
    {
        if (input[i] == 'a' || input[i] == 'e' || input[i] == 'i' || input[i] == 'o' || input[i] == 'u')
        {
            vowels++;
        }
    }
    return vowels;
}

uint32_t has_repeating_characters(const char *input) {
    for (uint32_t i = 0; i < strlen(input); i++) {
        if (i > 0 && input[i - 1] == input[i]) {
            return 1;
        }
    }
    return 0;
}

char **get_forbidden_strings(const char *input, size_t *out_length) {
    char **buffer = 0;
    uint32_t current_buffer_length = 0;
    uint32_t current_buffer_position = 0;
    for (uint32_t i = 0; i < strlen(input); i++)
    {
        if (i > 0 && ((input[i - 1] == 'a' && input[i] == 'b') ||
                      (input[i - 1] == 'c' && input[i] == 'd') ||
                      (input[i - 1] == 'p' && input[i] == 'q') ||
                      (input[i - 1] == 'x' && input[i] == 'y')))
        {
            buffer = realloc(buffer, (++current_buffer_length) * sizeof(*buffer));
            buffer[current_buffer_position] = calloc(2, sizeof(char));
            buffer[current_buffer_position][0] = input[i - 1];
            buffer[current_buffer_position][1] = input[i];
            current_buffer_position++;
        }
    }
    *out_length = current_buffer_length;
    return buffer;
}

uint32_t is_nice_line_in_part_one(const char *line) {
    if (get_vowels(line) < 3 || has_repeating_characters(line) == 0) {
        return 0;
    }

    size_t number_of_forbidden_strings = 0;
    char **buffer = get_forbidden_strings(line, &number_of_forbidden_strings);
    if (number_of_forbidden_strings > 0) {
        for(size_t i = 0; i < number_of_forbidden_strings; i++) {
            free(buffer[i]);
        }
        free(buffer);
        return 0;
    }

    free(buffer);
    return 1;
}

uint32_t is_nice_line_in_part_two(const char *line) {
    uint32_t is_first_criteria_met = 0;
    size_t line_length = strlen(line);

    /* A line is nice if it contains at least one letter which repeats with exactly one letter between them, like xyx, abcdefeghi (efe), or even aaa. */
    for (uint32_t i = 2; i < line_length; i++) {
        if (line[i - 2] == line[i]) {
            //printf("Line %s: First criteria is met because %c%c%c\n", line, line[i - 2], line[i - 1], line[i]);
            is_first_criteria_met = 1;
            break;
        }
    }
    if (is_first_criteria_met != 1) {
        return 0;
    }

    /* It contains a pair of any two letters that appears at least twice in the string without overlapping, like xyxy (xy) or aabcdefgaa (aa), but not like aaa (aa, but it overlaps). */
    HashTable *hashtable = hashtable_create(10);
    if(hashtable == NULL) {
        return 0;
    }

    if(!string_contains_non_overlapping_pair(line)) {
        hashtable_destroy(hashtable);
        return 0;
    
    }

    HashTableIterator *iterator = hashtable_create_iterator(hashtable);
    HashTableEntry *entry       = hashtable_iterator_next(iterator);
    while(entry != NULL) {
        void *value = hashtable_entry_get_value(entry);
        if ((*(uint64_t *)value) > 1) {
            //printf("Second criteria is met because there are %lu occurrences of %s in %s\n", *(uint64_t*)entry->value, entry->key, line);
            break;
        }
    }
   
    hashtable_destroy(hashtable);
    hashtable_iterator_destroy(iterator);

    return 1;
}

void solve_part_one(char **lines, size_t length, solution_t *solution)
{
    int nice_lines = 0;
    for(size_t i = 0; i < length; i++)
    {
        nice_lines += is_nice_line_in_part_one(lines[i]);
    }
    solution_part_finalize_with_int(solution, 0, nice_lines, "258");
}

void solve_part_two(char **lines, size_t length, solution_t *solution)
{
    int nice_lines = 0;
    for(size_t i = 0; i < length; i++)
    {
        nice_lines += is_nice_line_in_part_two(lines[i]);
    }
    solution_part_finalize_with_int(solution, 1, nice_lines, "53");
}

void test_is_nice_line_in_part_two(TestResults *test_results, const char *str, uint32_t expectation)
{
    uint32_t success = is_nice_line_in_part_two(str) == expectation ? 1 : 0;
    test_results->total++;
    test_results->succeeded += success;

    printf("%s(%s, %d): %s\n", __func__, str, expectation, success ? "OK" : "Not OK");
}

void test()
{
    TestResults test_results = {.total = 0, .succeeded = 0};
    
    test_is_nice_line_in_part_two(&test_results, "qjhvhtzxzqqjkmpb", 1);
    test_is_nice_line_in_part_two(&test_results, "xxyxx", 1);
    test_is_nice_line_in_part_two(&test_results, "uurcxstgmygtbstg", 0);
    test_is_nice_line_in_part_two(&test_results, "ieodomkazucvgmuy", 0);
    test_is_nice_line_in_part_two(&test_results, "aaabcdefg", 0);
    test_is_nice_line_in_part_two(&test_results, "dieatyxxxlvhneoj", 0);
    test_is_nice_line_in_part_two(&test_results, "ueihvxviirnooomi", 0);
    
    printf("%d tests were executed, %d tests succeeded, %d tests failed.\n", test_results.total, test_results.succeeded, test_results.total - test_results.succeeded);
}

int main(int argc, char *argv[]) {
    (void)argc;
    test();

    solution_t *solution = solution_create(2015, 5);
    size_t number_of_lines = 0;
    char **lines = file_read_all_lines(&number_of_lines, argv[1]);

    solve_part_one(lines, number_of_lines, solution);
    solve_part_two(lines, number_of_lines, solution);

    file_destroy_all_lines(lines, number_of_lines);
    return solution_finalize_and_destroy(solution);
}
