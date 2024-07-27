#ifndef AOC
#define AOC

#include <stdint.h>
#include <time.h>

typedef enum SolutionExitCode {
    SOLUTION_SUCCESS,
    SOLUTION_PART_ONE_INCORRECT,
    SOLUTION_PART_TWO_INCORRECT
} SolutionExitCode;

typedef struct SolutionPart
{
    int part_number;
    clock_t start_time;
    clock_t stop_time;
    double elapsed_seconds;
    char result[1024];
    char expected_result[1024];
} SolutionPart;

typedef struct Solution
{
    clock_t start_time;
    clock_t stop_time;
    double elapsed_seconds;
    uint32_t year;
    uint32_t day;
    SolutionPart parts[2];
} Solution;

void solution_create(Solution *solution, uint32_t year, uint32_t day);

void solution_part_finalize_with_int(Solution *solution, int part_number, int result, char *expected_result);

void solution_part_finalize_with_ui(Solution *solution, int part_number, uint64_t result, char *expected_result);

void solution_part_finalize_with_str(Solution *solution, int part_number, char *result, char *expected_result);

SolutionExitCode solution_finalize(Solution *solution);

#endif
