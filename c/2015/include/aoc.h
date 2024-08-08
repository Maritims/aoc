#ifndef AOC
#define AOC

#include <stdint.h>
#include <time.h>

typedef enum solution_exit_code_t {
    SOLUTION_SUCCESS,
    SOLUTION_PART_ONE_INCORRECT,
    SOLUTION_PART_TWO_INCORRECT
} solution_exit_code_t;

typedef struct solution_part_t
{
    int part_number;
    clock_t start_time;
    clock_t stop_time;
    double elapsed_seconds;
    char result[1024];
    char expected_result[1024];
} solution_part_t;

typedef struct solution_t
{
    clock_t start_time;
    clock_t stop_time;
    double elapsed_seconds;
    uint32_t year;
    uint32_t day;
    solution_part_t parts[2];
} solution_t;

solution_t *solution_create(uint32_t year, uint32_t day);

void solution_part_finalize_with_int(solution_t *solution, int part_number, int result, char *expected_result);

void solution_part_finalize_with_ui(solution_t *solution, int part_number, uint64_t result, char *expected_result);

void solution_part_finalize_with_str(solution_t *solution, int part_number, char *result, char *expected_result);

solution_exit_code_t solution_finalize_and_destroy(solution_t *solution);

#endif
