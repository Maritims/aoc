#include <stdio.h>
#include <string.h>

#include "aoc.h"
#include "file4c.h"
#include "string4c.h"

int main(int argc, char *argv[]) {
    (void)argc;
    Solution solution;
    char **lines;
    size_t number_of_lines;
    size_t total_for_part_one = 0;
    size_t total_for_part_two = 0;

    solution_create(&solution, 2015, 8);
    file_read_all_lines(&lines, &number_of_lines, argv[1]);

    for(size_t i = 0; i < number_of_lines; i++)
    {
        char *line = lines[i];
        char *escaped_line;
        char *unescaped_line;
        size_t escaped_length;
        size_t unescaped_length;


        escaped_length = strlen(line);
        unescaped_line = string_unescape(line);
        unescaped_length = strlen(unescaped_line) - 2;
        total_for_part_one += escaped_length - unescaped_length;

        unescaped_length = strlen(line);
        escaped_line = string_escape(line);
        escaped_length = strlen(escaped_line);
        total_for_part_two += escaped_length - unescaped_length;

        free(line);
        free(unescaped_line);
        free(escaped_line);
    }

    solution_part_finalize_with_int(&solution, 0, total_for_part_one, "1333");
    solution_part_finalize_with_int(&solution, 1, total_for_part_two, "2046");

    free(lines);
    
    return solution_finalize(&solution);
}
