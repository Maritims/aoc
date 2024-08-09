#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "aoc.h"
#include "file4c.h"
#include "string4c.h"

typedef struct AuntSue {
    int id;
    int children;
    int cats;
    int samoyeds;
    int pomeranians;
    int akitas;
    int vizslas;
    int goldfish;
    int trees;
    int cars;
    int perfumes;
} AuntSue;

void auntsue_init(AuntSue *result) {
    result->children = INT_MIN;
    result->cats = INT_MIN;
    result->samoyeds = INT_MIN;
    result->pomeranians = INT_MIN;
    result->akitas = INT_MIN;
    result->vizslas = INT_MIN;
    result->goldfish = INT_MIN;
    result->trees = INT_MIN;
    result->cars = INT_MIN;
    result->perfumes = INT_MIN;
}

void auntsue_print(AuntSue aunt) {
    printf("{id=%d, children=%d, cats=%d, samoyeds=%d, pomeranians=%d, akitas=%d, vizslas=%d, goldfish=%d, trees=%d, cars=%d, perfumes=%d}\n", aunt.id, aunt.children, aunt.cats, aunt.samoyeds, aunt.pomeranians, aunt.akitas, aunt.vizslas, aunt.goldfish, aunt.trees, aunt.cars, aunt.perfumes);
}

int main(int argc, char *argv[]) {
    (void)argc;

    solution_t *solution = solution_create(2015, 16);
    size_t number_of_lines = 0;
    char **lines = file_read_all_lines(&number_of_lines, argv[1]);

    int children = 3;
    int cats = 7;
    int samoyeds = 2;
    int pomeranians = 3;
    int akitas = 0;
    int vizslas = 0;
    int goldfish = 5;
    int trees = 3;
    int cars = 2;
    int perfumes = 1;

    int part_one = 0;
    int part_two = 0;

    for(size_t i = 0; i < number_of_lines; i++) {
        size_t number_of_tokens;
        char **tokens = string_split(&number_of_tokens, lines[i], " ");
        char *id = string_substring(tokens[1], 0, (size_t)(strchr(tokens[1], ':') - tokens[1]));

        bool is_match_in_part_one = true;
        bool is_match_in_part_two = true;

        for(size_t j = 2; j < number_of_tokens - 1; j += 2) {
            char *token = string_substring(tokens[j], 0, (size_t)(strchr(tokens[j], ':') - tokens[j]));
            char *next_token = tokens[j + 1];
            int value = atoi(next_token);

            if(
                (strcmp(token, "children") == 0 && value != children) ||
                (strcmp(token, "samoyeds") == 0 && value != samoyeds) ||
                (strcmp(token, "akitas") == 0 && value != akitas) ||
                (strcmp(token, "vizslas") == 0 && value != vizslas) ||
                (strcmp(token, "cars") == 0 && value != cars) ||
                (strcmp(token, "perfumes") == 0 && value != perfumes)
            ) {
                is_match_in_part_one = false;
                is_match_in_part_two = false;

                free(token);
                break;
            }

            if(
                (strcmp(token, "cats") == 0 && value != cats) ||
                (strcmp(token, "trees") == 0 && value != trees) ||
                (strcmp(token, "pomeranians") == 0 && value != pomeranians) ||
                (strcmp(token, "goldfish") == 0 && value != goldfish)
            ) {
                is_match_in_part_one = false;
            }

            if(
                (strcmp(token, "cats") == 0 && cats > value) ||
                (strcmp(token, "trees") == 0 && trees > value) ||
                (strcmp(token, "pomeranians") == 0 && pomeranians < value) ||
                (strcmp(token, "goldfish") == 0 && goldfish < value)
            ) {
                is_match_in_part_two = false;
            }

            free(token);
        }

        if(is_match_in_part_one) {
            part_one = atoi(id);
        }

        if(is_match_in_part_two) {
            part_two = atoi(id);
        }

        free(id);
        FREE_ARRAY(tokens, number_of_tokens);
    }

    solution_part_finalize_with_int(solution, 0, part_one, "213");
    solution_part_finalize_with_int(solution, 1, part_two, "323");

    FREE_ARRAY(lines, number_of_lines);
    return solution_finalize_and_destroy(solution);
}
