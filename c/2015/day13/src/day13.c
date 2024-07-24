#include <limits.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "aoc.h"
#include "combinatorics4c.h"
#include "get_file.h"
#include "string4c.h"

int *calculate_happiness(char **lines, size_t number_of_lines, bool include_yourself) {
    int map[25][25]; // There are 25 letters in the alphabet.
    int id_map[25] = {0}; // Map for converting iteration variables to actual map ids;
    char current_main_id = ' ';
    size_t number_of_main_ids = 0;
    for(size_t i = 0; i < number_of_lines; i++) {
        char *line = lines[i];
        size_t number_of_tokens = 0;
        char **tokens = string_split(line, " ", &number_of_tokens);
        
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
        for(int i = 0; i < number_of_main_ids; i++) {
            int actual_i = id_map[i];
            printf("%d = %d\n", i, actual_i);
            map[actual_i][24] = 0;
            map[24][actual_i] = 0;
        }

        id_map[number_of_main_ids] = 24; // 24+65 is the ASCII code for Y, the first letter in "You".
        number_of_main_ids++;
    }

    uint64_t *number_of_permutations = malloc(sizeof(uint64_t));
    int **permutations = combinatorics_permutations_compute_int(number_of_main_ids, &number_of_permutations);
    if(permutations == NULL) {
        fprintf(stderr, "Failed to generate permutations\n");
        return NULL;
    }

    int *highest_total_happiness = malloc(sizeof(int));
    *highest_total_happiness = INT_MIN;
    char id_string[number_of_main_ids];
    for(uint64_t i = 0; i < *number_of_permutations; i++) {
        int *ids = permutations[i];
        int total_happiness = 0;
        for(int j = 0; j < number_of_main_ids; j++) {
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

        if(total_happiness >= *highest_total_happiness) {
            *highest_total_happiness = total_happiness;
        }
    }

    free(number_of_permutations);
    free(permutations);

    return highest_total_happiness;
}

int main(int argc, char *argv[]) {
    Solution *solution = solution_create(2015, 13);
    size_t number_of_lines = 0;
    char **lines = get_file_as_lines(argv[1], &number_of_lines);

    int *part_one = calculate_happiness(lines, number_of_lines, false);
    sprintf(solution->part_one.result, "%d", *part_one);
    solution_part_finalize(&solution->part_one);

    int *part_two = calculate_happiness(lines, number_of_lines, true);
    sprintf(solution->part_two.result, "%d", *part_two);

    solution_finalize(solution);
    solution_print(solution);
    free(solution);
    free(lines);
    free(part_one);
    free(part_two);

    return 1;
}
