#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "aoc.h"

solution_t *solution_create(uint32_t year, uint32_t day) {
    solution_t *solution = malloc(sizeof(solution_t));

    solution->start_time = clock();
    solution->year = year;
    solution->day = day;

    for(int i = 0; i < 2; i++) {
        solution->parts[i].start_time = clock();
        solution->parts[i].result[0] = '\0';
        solution->parts[i].expected_result[0] = '\0';    
    }
    printf("---- Advent of Code %d: Day %02d ----\n", solution->year, solution->day);
    return solution;
}

static bool solution_part_has_result(solution_t *solution, int part_number) {
    if(strlen(solution->parts[part_number].result)) {
        fprintf(stderr, "%s:%d: The result of part %d has already been written\n", __func__, __LINE__, part_number + 1);
        return true;
    }
    return false;
}

static void solution_part_finalize(solution_t *solution, int part_number, char *expected_result) {
    sprintf(solution->parts[part_number].expected_result, "%s", expected_result);
    solution->parts[part_number].stop_time = clock();
    solution->parts[part_number].elapsed_seconds = (double)(solution->parts[part_number].stop_time - solution->parts[part_number].start_time) / CLOCKS_PER_SEC;

    printf(
        "Part %d........: %s (%s) (%f s)\n",
        part_number + 1,
        solution->parts[part_number].result,
        strcmp(solution->parts[part_number].result, solution->parts[part_number].expected_result) == 0 ? "✔": "x",
        solution->parts[part_number].elapsed_seconds
    );
}

void solution_part_finalize_with_int(solution_t *solution, int part_number, int result, char *expected_result) {
    if(solution_part_has_result(solution, part_number)) {
        return;
    }

    sprintf(solution->parts[part_number].result, "%d", result);
    solution_part_finalize(solution, part_number, expected_result);
}

void solution_part_finalize_with_ui(solution_t *solution, int part_number, uint64_t result, char *expected_result) {
    if(solution_part_has_result(solution, part_number)) {
        return;
    }

    sprintf(solution->parts[part_number].result, "%lu", result);
    solution_part_finalize(solution, part_number, expected_result);
}

void solution_part_finalize_with_str(solution_t *solution, int part_number, char *result, char *expected_result) {
    if(solution_part_has_result(solution, part_number)) {
        return;
    }
    
    sprintf(solution->parts[part_number].result, "%s", result);
    solution_part_finalize(solution, part_number, expected_result);
}

solution_exit_code_t solution_finalize_and_destroy(solution_t *solution)
{
    solution->stop_time = clock();
    solution->elapsed_seconds = (double)(solution->stop_time - solution->start_time) / CLOCKS_PER_SEC;

    printf("Total duration: %f s\n", solution->elapsed_seconds);

    solution_exit_code_t exit_code = SOLUTION_SUCCESS;
    if(strcmp(solution->parts[0].result, solution->parts[0].expected_result) != 0) {
        exit_code = SOLUTION_PART_ONE_INCORRECT;
    }
    else if(strcmp(solution->parts[1].result, solution->parts[1].expected_result) != 0) {
        exit_code = SOLUTION_PART_TWO_INCORRECT;
    }

    free(solution);
    return exit_code;
}
