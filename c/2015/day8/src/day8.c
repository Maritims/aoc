#include <stdio.h>
#include <string.h>

#include "aoc.h"
#include "string4c.h"

int main(int argc, char *argv[])
{
    Solution *solution = solution_create(2015, 8);

    size_t length;
    char **lines = get_file_as_lines(argv[1], &length);
    size_t total_for_part_one = 0;
    size_t total_for_part_two = 0;

    for(size_t i = 0; i < length; i++)
    {
        char *line = lines[i];

        size_t escaped_length = strlen(line);
        char *unescaped_line = string_unescape(line);
        size_t unescaped_length = strlen(unescaped_line) - 2;
        //printf("%s (%zu) -> %s (%zu)\n", line, escaped_length, unescaped_line, unescaped_length);
        total_for_part_one += escaped_length - unescaped_length;
        free(unescaped_line);

        unescaped_length = strlen(line);
        char *escaped_line = string_escape(line);
        escaped_length = strlen(escaped_line);
        printf("%s (%zu) -> %s (%zu)\n", escaped_line, escaped_length, unescaped_line, unescaped_length);
        total_for_part_two += escaped_length - unescaped_length;
        free(escaped_line);

        free(line);
    }

    sprintf(solution->part_one.result, "%zu", total_for_part_one);
    solution_part_finalize(&solution->part_one);
    sprintf(solution->part_two.result, "%zu", total_for_part_two);
    solution_part_finalize(&solution->part_two);

    solution_print(solution);

    free(lines);
    free(solution);
}