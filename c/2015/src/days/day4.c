#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "aoc.h"
#include "file4c.h"
#include "maritims_md5.h"

void solve_part_one(char *file_content, solution_t *solution)
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
        free(md5_hash);
    }

    solution_part_finalize_with_int(solution, 0, number, "282749");
}

void solve_part_two(char *file_content, solution_t *solution)
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
        free(md5_hash);
    }

    solution_part_finalize_with_int(solution, 1, number, "9962624");
}

int main(int argc, char *argv[]) {
    (void)argc;

    solution_t *solution = solution_create(2015, 4);
    char *file_content = file_read_all_text(argv[1]);

    solve_part_one(file_content, solution);
    solve_part_two(file_content, solution);

    free(file_content);
    return solution_finalize_and_destroy(solution);
}
