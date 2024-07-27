#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "aoc.h"

void solution_create(Solution *solution, uint32_t year, uint32_t day)
{
    solution->start_time = clock();
    solution->year = year;
    solution->day = day;
    solution->part_one = (SolutionPart){
        .result = malloc(100),
        .expected_result = malloc(100)
    };
    solution->part_two = (SolutionPart){
        .result = malloc(100),
        .expected_result = malloc(100)
    };
}

void solution_part_finalize(SolutionPart *solution_part, char *expected_result)
{
    sprintf(solution_part->expected_result, "%s", expected_result);
    solution_part->stop_time = clock();
    solution_part->elapsed_seconds = (double)(solution_part->stop_time - solution_part->start_time) / CLOCKS_PER_SEC;
}

void solution_finalize(Solution *solution)
{
    solution->stop_time = clock();
    solution->elapsed_seconds = (double)(solution->stop_time - solution->start_time) / CLOCKS_PER_SEC;
}

void solution_print(Solution *solution)
{
    printf("--- Advent of Code %d: Day %02d ---\nPart 1: %s (%s) (%f s)\nPart 2: %s (%s) (%f s)\nTotal duration: %f s\n",
        solution->year,
        solution->day,
        solution->part_one.result,
        strcmp(solution->part_one.result, solution->part_one.expected_result) == 0 ? "✔" : "x",
        solution->part_one.elapsed_seconds,
        solution->part_two.result,
        strcmp(solution->part_two.result, solution->part_two.expected_result) == 0 ? "✔" : "x",
        solution->part_two.elapsed_seconds,
        solution->elapsed_seconds
    );
}
