#ifndef AOC
#define AOC

#include <stdint.h>
#include <time.h>

#include "get_file.h"

typedef struct SolutionPart
{
    clock_t start_time;
    clock_t stop_time;
    double elapsed_seconds;
    char *result;
} SolutionPart;

typedef struct Solution
{
    clock_t start_time;
    clock_t stop_time;
    double elapsed_seconds;
    uint32_t year;
    uint32_t day;
    SolutionPart part_one;
    SolutionPart part_two;
} Solution;

Solution *solution_create(uint32_t year, uint32_t day);

void solution_part_finalize(SolutionPart *solution_part);

void solution_finalize(Solution *solution);

void solution_print(Solution *solution);

#endif