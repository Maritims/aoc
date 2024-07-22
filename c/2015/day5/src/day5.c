#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc.h"
#include "get_file.h"
#include "hashtable.h"
#include "testutils.h"

#define DEBUG 1

uint32_t get_vowels(const char *input)
{
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

uint32_t has_repeating_characters(const char *input)
{
    uint32_t repetitions = 0;
    for (uint32_t i = 0; i < strlen(input); i++)
    {
        if (i > 0 && input[i - 1] == input[i])
        {
            return 1;
        }
    }
    return 0;
}

char **get_forbidden_strings(const char *input, size_t *out_length)
{
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

uint32_t is_nice_line_in_part_one(const char *line)
{
    if (get_vowels(line) < 3 || has_repeating_characters(line) == 0)
    {
        return 0;
    }

    size_t number_of_forbidden_strings = 0;
    char **forbidden_strings = get_forbidden_strings(line, &number_of_forbidden_strings);
    if (number_of_forbidden_strings > 0)
    {
        return 0;
    }

    return 1;
}

uint32_t is_nice_line_in_part_two(const char *line)
{
    uint32_t is_first_criteria_met = 0;
    uint32_t is_second_criteria_met = 0;

    /* A line is nice if it contains at least one letter which repeats with exactly one letter between them, like xyx, abcdefeghi (efe), or even aaa. */
    for (uint32_t i = 2; i < strlen(line); i++)
    {
        if (line[i - 2] == line[i])
        {
            //printf("Line %s: First criteria is met because %c%c%c\n", line, line[i - 2], line[i - 1], line[i]);
            is_first_criteria_met = 1;
            break;
        }
    }
    if (is_first_criteria_met != 1)
    {
        return 0;
    }

    /* It contains a pair of any two letters that appears at least twice in the string without overlapping, like xyxy (xy) or aabcdefgaa (aa), but not like aaa (aa, but it overlaps). */
    HashTable *hashtable = hashtable_create(10);
    if(hashtable == NULL)
    {
        return 0;
    }

    char last_three[3];
    for (uint32_t i = 0; i < strlen(line) - 1; i++)
    {
        last_three[i % 3] = line[i];
        if(last_three[0] == last_three[1] && last_three[1] == last_three[2])
        {
            return 0;
        }

        char *pair = calloc(2, sizeof(char));
        sprintf(pair, "%c%c", line[i], line[i + 1]);
        
        HashTableEntry *entry = hashtable_get(hashtable, pair);
        if (entry == NULL)
        {
            uint64_t *value = malloc(sizeof(uint64_t));
            *value = 1;
            entry = hashtable_put(hashtable, pair, value, sizeof(uint64_t), 0);
            if(entry == NULL)
            {
                return 0;
            }
        }
        else
        {
            (*(uint64_t*)entry->value)++;
        }
    }

    for (size_t i = 0; i < hashtable->capacity; i++)
    {
        HashTableEntry *entry = hashtable->entries[i];
        while(entry != NULL) {
            if ((*(uint64_t *)entry->value) > 1)
            {
                //printf("Second criteria is met because there are %lu occurrences of %s in %s\n", *(uint64_t*)entry->value, entry->key, line);
                is_second_criteria_met = 1;
                break;
            }
            entry = entry->next;
        }
    }
    hashtable_destroy(hashtable);

    return is_first_criteria_met == 1 && is_second_criteria_met == 1 ? 1 : 0;
}

void solve_part_one(char **lines, size_t length, SolutionPart *solution_part)
{
    int nice_lines = 0;
    for(int i = 0; i < length; i++)
    {
        nice_lines += is_nice_line_in_part_one(lines[i]);
    }
    sprintf(solution_part->result, "%d", nice_lines);
    solution_part_finalize(solution_part);
}

void solve_part_two(char **lines, size_t length, SolutionPart *solution_part)
{
    int nice_lines = 0;
    for(int i = 0; i < length; i++)
    {
        nice_lines += is_nice_line_in_part_two(lines[i]);
    }
    sprintf(solution_part->result, "%d", nice_lines);
    solution_part_finalize(solution_part);
}

int test_is_nice_line_in_part_two(TestResults *test_results, const char *str, int expectation)
{
    int success = is_nice_line_in_part_two(str) == expectation ? 1 : 0;
    test_results->total++;
    test_results->succeeded += success;

    printf("%s(%s, %d): %s\n", __func__, str, expectation, success ? "OK" : "Not OK");
}

int test()
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

int main(int argc, char *argv[])
{
    test();

    Solution *solution = solution_create(2015, 5);

    size_t length = 0;
    char **lines = get_file_as_lines(argv[1], &length);

    solve_part_one(lines, length, &(solution->part_one));
    solve_part_two(lines, length, &(solution->part_two));

    solution_finalize(solution);
    solution_print(solution);

    return 0;

    /*
    size_t number_of_lines;
    char **lines = split_to_lines(file_content, &number_of_lines);
    uint32_t nice_lines_in_part_one = 0;
    uint32_t nice_lines_in_part_two = 0;

    for (uint32_t line_number = 0; line_number < number_of_lines; line_number++)
    {
        if (is_nice_line_in_part_one(lines[line_number]) == 1)
        {
            nice_lines_in_part_one++;
        }
        if (is_nice_line_in_part_two(lines[line_number]) == 1)
        {
            nice_lines_in_part_two++;
        }
    }

    printf("Part 1: %d out of %zu are nice lines\nPart 2: %d out of %zu are nice lines\n", nice_lines_in_part_one, number_of_lines, nice_lines_in_part_two, number_of_lines);
    */
    return 0;
}