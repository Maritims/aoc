#include <stdio.h>

#include "aoc.h"

int main(int argc, char *argv[]) {
    Solution *solution = solution_create(2015, 14);

    /*
     * Solve part 1
     * char *part_one = ...;
     * sprintf(solution->part_one.result, "%s", part_one);
     */
    solution_part_finalize(&solution->part_one);

    /* 
     * Solve part 2
     * char *part_two = ...;
     * sprintf(solution->part_two.result, "%s", part_two);
     */
    solution_part_finalize(&solution->part_two);

    solution_finalize(solution);
    solution_print(solution);

    return 0;
}
