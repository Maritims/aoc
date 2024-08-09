#include <stdio.h>
#include "aoc.h"
#include "file4c.h"
#include "math4c.h"

int main(int argc, char *argv[]) {
    (void)argc;

    solution_t *solution    = solution_create(2015, 17);
    size_t number_of_lines  = 0;
    char **lines            = file_read_all_lines(&number_of_lines, argv[1]);
    int liters              = atoi(lines[0]);
    int containers[number_of_lines];

    free(lines[0]);
    for(size_t i = 1; i < number_of_lines; i++) {
        containers[i] = atoi(lines[i]);
        free(lines[i]);
    }
    free(lines);

    size_t results_size                 = 0;
    size_t *results_column_sizes_ptr    = NULL;
    int **results                       = math_sets_compute_subsets(containers, number_of_lines, &results_size, &results_column_sizes_ptr);

    int part_one                                    = 0;
    size_t minimum_number_of_containers_required    = INT_MAX;
    int part_two                                    = INT_MAX;

    for(size_t i = 0; i < results_size; i++) {
        int subset_sum = 0;

        for(size_t j = 0; j < results_column_sizes_ptr[i]; j++) {
            subset_sum += results[i][j]; // The value at results[i][j] is sometimes uninitialised. This causes Valgrind to report problems with conditional jumps. It seems like this only happens whenever j is 0.
        }

        if(subset_sum == liters) {
            part_one++;
            if(minimum_number_of_containers_required > results_column_sizes_ptr[i]) {
                minimum_number_of_containers_required = results_column_sizes_ptr[i];
                part_two = 1;
            }
            else if(minimum_number_of_containers_required == results_column_sizes_ptr[i]) {
                part_two++;
            }
        }

        free(results[i]);
    }

    free(results_column_sizes_ptr);
    free(results);

    solution_part_finalize_with_int(solution, 0, part_one, "1638");
    solution_part_finalize_with_int(solution, 1, part_two, "17");

    return solution_finalize_and_destroy(solution);
}
