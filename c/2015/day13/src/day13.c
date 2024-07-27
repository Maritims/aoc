#include <limits.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "aoc.h"
#include "combinatorics4c.h"
#include "file4c.h"
#include "string4c.h"

void calculate_happiness(int *result, char **lines, size_t number_of_lines, bool include_yourself) {
    int map[26][26]; // There are 26 letters in the alphabet.
    int id_map[26] = {0}; // Map for converting iteration variables to actual map ids;
    char current_main_id = ' ';
    size_t number_of_main_ids = 0;
    for(size_t i = 0; i < number_of_lines; i++) {
        char *line = lines[i];
        char **tokens;
        size_t number_of_tokens = 0;

        string_split(&tokens, &number_of_tokens, line, " ");
        
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
        fprintf(stderr, "%s:%d: Failed to generate permutations\n", __func__, __LINE__);
        return;
    }

    *result = INT_MIN;
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

        if(total_happiness >= (*result)) {
            *result = total_happiness;
        }
    }

    free(number_of_permutations);
    free(permutations);
}

int main(int argc, char *argv[]) {
    Solution solution;
    char **lines;
    size_t number_of_lines;
    int part_one;
    int part_two;

    
    solution_create(&solution, 2015, 13);
    file_read_all_lines(&lines, &number_of_lines, argv[1]);

    calculate_happiness(&part_one, lines, number_of_lines, false);
    sprintf(solution.part_one.result, "%d", part_one);
    solution_part_finalize(&solution.part_one, "664");

    calculate_happiness(&part_two, lines, number_of_lines, true);
    sprintf(solution.part_two.result, "%d", part_two);
    solution_part_finalize(&solution.part_two, "640");

    solution_finalize(&solution);
    solution_print(&solution);

    free(lines);

    return 0;
}
