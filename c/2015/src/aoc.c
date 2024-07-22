#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "aoc.h"

Solution *solution_create(uint32_t year, uint32_t day)
{
    Solution *solution = malloc(sizeof(Solution));
    solution->start_time = clock();
    solution->year = year;
    solution->day = day;
    solution->part_one = (SolutionPart){.result = malloc(100)};
    solution->part_two = (SolutionPart){.result = malloc(100)};
    return solution;
}

void solution_part_finalize(SolutionPart *solution_part)
{
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
    printf("%d-%02d, part 1: %s (%f s), part 2: %s (%f s), total duration: %f s\n",
        solution->year,
        solution->day,
        solution->part_one.result,
        solution->part_one.elapsed_seconds,
        solution->part_two.result,
        solution->part_two.elapsed_seconds,
        solution->elapsed_seconds
    );
}