#include <stdio.h>

#include "aoc.h"
#include "file4c.h"

int main(int argc, char *argv[]) {
    Solution solution;
    char *file_content;
    int part_one;
    int part_two;
    
    solution_create(&solution, 2015, X);
    file_read_all_text(&file_content, argv[1]);

    solution_part_finalize_with_int(&solution, 0, part_one, "");
    solution_part_finalize_with_int(&solution, 1, part_two, "");

    return solution_finalize(&solution);
}
