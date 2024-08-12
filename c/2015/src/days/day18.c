#include <limits.h>
#include <stdio.h>

#include "aoc.h"
#include "conway.h"
#include "file4c.h"
#include "grid.h"

int main(int argc, char *argv[]) {
    (void)argc;

    solution_t *solution = solution_create(2015, 18);
    size_t number_of_lines = 0;
    char **lines = file_read_all_lines(&number_of_lines, argv[1]);
    grid_t *grid = grid_parse(lines, number_of_lines, '#');

    conway_t* conway    = conway_create(grid, 100, NULL, 0);
    int part_one        = conway_play_all_rounds(conway);
    conway_destroy(conway);

    grid = grid_parse(lines, number_of_lines, '#');
    grid_point_t **corners = calloc(4, sizeof(grid_point_t*));
    corners[0] = grid_get_top_left_corner(grid);
    corners[1] = grid_get_top_right_corner(grid);
    corners[2] = grid_get_bottom_left_corner(grid);
    corners[3] = grid_get_bottom_right_corner(grid);
    conway = conway_create(grid, 100, corners, 4);
    int part_two = conway_play_all_rounds(conway);

    solution_part_finalize_with_int(solution, 0, part_one, "1061");
    solution_part_finalize_with_int(solution, 1, part_two, "1006");

    conway_destroy(conway);
    FREE_ARRAY(lines, number_of_lines);
    return solution_finalize_and_destroy(solution);
}
