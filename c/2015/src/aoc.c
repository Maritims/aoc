#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "aoc.h"

void solution_part_init(Solution *solution, int part_number) {
    solution->parts[part_number].start_time = clock();
    solution->parts[part_number].result[0] = '\0';
    solution->parts[part_number].expected_result[0] = '\0';
}

void solution_create(Solution *solution, uint32_t year, uint32_t day) {
    solution->start_time = clock();
    solution->year = year;
    solution->day = day;
    solution_part_init(solution, 0);
    solution_part_init(solution, 1);

    printf("---- Advent of Code %d: Day %02d ----\n", solution->year, solution->day);
}

static bool solution_part_has_result(Solution *solution, int part_number) {
    if(strlen(solution->parts[part_number].result)) {
        fprintf(stderr, "%s:%d: The result of part %d has already been written\n", __func__, __LINE__, part_number + 1);
        return true;
    }
    return false;
}

static void solution_part_finalize(Solution *solution, int part_number, char *expected_result) {
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

void solution_part_finalize_with_int(Solution *solution, int part_number, int result, char *expected_result) {
    if(solution_part_has_result(solution, part_number)) {
        return;
    }

    sprintf(solution->parts[part_number].result, "%d", result);
    solution_part_finalize(solution, part_number, expected_result);
}

void solution_part_finalize_with_ui(Solution *solution, int part_number, uint64_t result, char *expected_result) {
    if(solution_part_has_result(solution, part_number)) {
        return;
    }

    sprintf(solution->parts[part_number].result, "%lu", result);
    solution_part_finalize(solution, part_number, expected_result);
}

void solution_part_finalize_with_str(Solution *solution, int part_number, char *result, char *expected_result) {
    if(solution_part_has_result(solution, part_number)) {
        return;
    }
    
    sprintf(solution->parts[part_number].result, "%s", result);
    solution_part_finalize(solution, part_number, expected_result);
}

SolutionExitCode solution_finalize(Solution *solution)
{
    solution->stop_time = clock();
    solution->elapsed_seconds = (double)(solution->stop_time - solution->start_time) / CLOCKS_PER_SEC;

    printf("Total duration: %f s\n", solution->elapsed_seconds);

    SolutionExitCode exit_code = SOLUTION_SUCCESS;
    if(strcmp(solution->parts[0].result, solution->parts[0].expected_result) != 0) {
        exit_code = SOLUTION_PART_ONE_INCORRECT;
    }
    else if(strcmp(solution->parts[1].result, solution->parts[1].expected_result) != 0) {
        exit_code = SOLUTION_PART_TWO_INCORRECT;
    }

    return exit_code;
}
