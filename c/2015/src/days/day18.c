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

    conway_t *conway = conway_create(lines, number_of_lines, '#', 100, NULL, 0);
    int part_one = conway_play_all_rounds(conway);
    conway_destroy(conway);

    uint32_t **always_enabled_cells = grid_get_corners(number_of_lines, conway_get_grid_cols(conway));
    conway = conway_create(lines, number_of_lines, '#', 100, always_enabled_cells, 4);
    int part_two = conway_play_all_rounds(conway);

    solution_part_finalize_with_int(solution, 0, part_one, "1061");
    solution_part_finalize_with_int(solution, 1, part_two, "1006");

    grid_destroy_corners(always_enabled_cells);
    conway_destroy(conway);
    FREE_ARRAY(lines, number_of_lines);
    return solution_finalize_and_destroy(solution);
}
