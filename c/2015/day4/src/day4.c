#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc.h"
#include "get_file.h"
#include "maritims_md5.h"

void solve_part_one(char *file_content, SolutionPart *solution_part)
{
    uint32_t number = 0;
    uint32_t first_three_bytes = 0xFFFFFFFF;
    while((first_three_bytes & 0xFFFFF000) != 0)
    {
        char input[strlen(file_content) + 1];
        sprintf(input, "%s%d", file_content, ++number);
        uint8_t *md5_hash = md5_digest(input);
        first_three_bytes = ((uint32_t)md5_hash[0] << 24) |
                            ((uint32_t)md5_hash[1] << 16) |
                            ((uint32_t)md5_hash[2] << 8);
    }

    sprintf(solution_part->result, "%d", number);
    solution_part_finalize(solution_part);
}

void solve_part_two(char *file_content, SolutionPart *solution_part)
{
    uint32_t number = 0;
    uint32_t first_three_bytes = 0xFFFFFFFF;
    while((first_three_bytes & 0xFFFFFF00) != 0)
    {
        char input[strlen(file_content) + 1];
        sprintf(input, "%s%d", file_content, ++number);
        uint8_t *md5_hash = md5_digest(input);
        first_three_bytes = ((uint32_t)md5_hash[0] << 24) |
                            ((uint32_t)md5_hash[1] << 16) |
                            ((uint32_t)md5_hash[2] << 8);
    }

    sprintf(solution_part->result, "%d", number);
    solution_part_finalize(solution_part);
}

int main(int argc, char *argv[])
{
    Solution *solution = solution_create(2015, 4);

    char *file_content = get_file(argv[1]);

    solve_part_one(file_content, &(solution->part_one));
    solve_part_two(file_content, &(solution->part_two));

    solution_finalize(solution);
    solution_print(solution);

    return 0;
}