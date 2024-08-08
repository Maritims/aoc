#include <limits.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "aoc.h"
#include "file4c.h"
#include "math4c.h"
#include "string4c.h"

void calculate_happiness(int *result, char **lines, size_t number_of_lines, bool include_yourself) {
    int map[26][26]; // There are 26 letters in the alphabet.
    int id_map[26] = {0}; // Map for converting iteration variables to actual map ids;
    char current_main_id = ' ';
    size_t number_of_main_ids = 0;
    for(size_t i = 0; i < number_of_lines; i++) {
        size_t number_of_tokens = 0;
        char **tokens = string_split(&number_of_tokens, lines[i], " ");
        
        char main_id = tokens[0][0]; // First letter of the person we're currently looking at.
        char minor_id = tokens[10][0]; // First letter of the person they could be sitting next to.
        int happiness = atoi(tokens[3]) * (strcmp(tokens[2], "lose") == 0 ? -1 : 1); // Change in happiness if seated next to the person identified by minor_id.
        map[(int)main_id % 65][(int)minor_id % 65] = happiness;

        if(current_main_id != main_id) {
            id_map[number_of_main_ids] = (int)main_id % 65;
            number_of_main_ids++;
        }
        current_main_id = main_id;


        free(tokens);
    }

    if(include_yourself) {
        for(size_t i = 0; i < number_of_main_ids; i++) {
            int actual_i = id_map[i];
            map[actual_i][24] = 0;
            map[24][actual_i] = 0;
        }

        id_map[number_of_main_ids] = 24; // 24+65 is the ASCII code for Y, the first letter in "You".
        number_of_main_ids++;
    }

    uint64_t *number_of_permutations = malloc(sizeof(uint64_t));
    int **permutations = math_permutations_compute_int(number_of_main_ids, &number_of_permutations);
    if(permutations == NULL) {
        fprintf(stderr, "%s:%d: Failed to generate permutations\n", __func__, __LINE__);
        return;
    }

    *result = INT_MIN;
    for(uint64_t i = 0; i < *number_of_permutations; i++) {
        int *ids = permutations[i];
        int total_happiness = 0;
        for(size_t j = 0; j < number_of_main_ids; j++) {
            int main_id_key = ids[j] - 1;
            int left_id_key = (ids[(j - 1 + number_of_main_ids) % number_of_main_ids] - 1); // Correct handling of negative index
            int right_id_key = ids[(j + 1) % number_of_main_ids] - 1;

            int main_id = id_map[main_id_key];
            int left_id = id_map[left_id_key];
            int right_id = id_map[right_id_key];

            int left_happiness = map[main_id][left_id];
            int right_happiness = map[main_id][right_id];

            total_happiness += left_happiness + right_happiness;
        }

        if(total_happiness >= (*result)) {
            *result = total_happiness;
        }
    }

    free(number_of_permutations);
    free(permutations);
}

int main(int argc, char *argv[]) {
    (void)argc;

    solution_t *solution = solution_create(2015, 13);
    size_t number_of_lines = 0;
    char **lines = file_read_all_lines(&number_of_lines, argv[1]);

    int part_one = 0;
    calculate_happiness(&part_one, lines, number_of_lines, false);
    solution_part_finalize_with_int(solution, 0, part_one, "664");

    int part_two = 0;
    calculate_happiness(&part_two, lines, number_of_lines, true);
    solution_part_finalize_with_int(solution, 1, part_two, "640");


    file_destroy_all_lines(lines, number_of_lines); 
    return solution_finalize_and_destroy(solution);
}
