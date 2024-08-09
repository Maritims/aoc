#include <stdio.h>
#include <string.h>

#include "aoc.h"
#include "file4c.h"
#include "string4c.h"

int main(int argc, char *argv[]) {
    (void)argc;

    solution_t *solution = solution_create(2015, 8);
    size_t number_of_lines = 0;
    char **lines = file_read_all_lines(&number_of_lines, argv[1]);

    size_t total_for_part_one = 0;
    size_t total_for_part_two = 0;
    for(size_t i = 0; i < number_of_lines; i++)
    {
        size_t escaped_length = strlen(lines[i]);
        char *unescaped_line = string_unescape(lines[i]);
        size_t unescaped_length = strlen(unescaped_line) - 2;
        total_for_part_one += escaped_length - unescaped_length;

        unescaped_length = strlen(lines[i]);
        char *escaped_line = string_escape(lines[i]);
        escaped_length = strlen(escaped_line);
        total_for_part_two += escaped_length - unescaped_length;

        free(lines[i]);
        free(unescaped_line);
        free(escaped_line);
    }

    solution_part_finalize_with_int(solution, 0, total_for_part_one, "1333");
    solution_part_finalize_with_int(solution, 1, total_for_part_two, "2046");

    free(lines);
    
    return solution_finalize_and_destroy(solution);
}
