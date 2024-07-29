#include <limits.h>
#include <stdio.h>

#include "aoc.h"
#include "conway.h"
#include "file4c.h"
#include "grid.h"

int main(int argc, char *argv[]) {
    Solution solution;
    solution_create(&solution, 2015, 18);
    
    char **lines;
    size_t number_of_lines;
    size_t number_of_columns;
    
    file_read_all_lines(&lines, &number_of_lines, argv[1]);

    int part_one;
    int part_two;
    conway_t *conway;
    int **always_enabled_cells;

    conway = conway_create(lines, number_of_lines, '#', 100, NULL, 0);
    part_one = conway_play_all_rounds(conway);
    conway_destroy(conway);

    always_enabled_cells = grid_get_corners(number_of_lines, conway_get_grid_cols(conway));
    conway = conway_create(lines, number_of_lines, '#', 100, always_enabled_cells, 4);
    part_two = conway_play_all_rounds(conway);
    grid_destroy_corners(always_enabled_cells);
    conway_destroy(conway);
    file_destroy_all_lines(&lines, number_of_lines);

    solution_part_finalize_with_int(&solution, 0, part_one, "1061");
    solution_part_finalize_with_int(&solution, 1, part_two, "1006");

    return solution_finalize(&solution);
}
