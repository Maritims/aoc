#include <stdio.h>

#include "aoc.h"
#include "get_file.h"
#include "string4c.h"

typedef struct Ingredient {
    char *name;
    int capacity;
    int durability;
    int flavor;
    int texture;
    int calories;
} Ingredient;

char *ingredient_to_string(Ingredient ingredient) {
    char *buf = calloc(1024, sizeof(char));
    if(buf == NULL) {
        fprintf(stderr, "Failed to allocate memory for string representation of ingredient with name \"%s\"\n", ingredient.name);
        return NULL;
    }

    sprintf(buf, "{name: \"%s\", capacity: %d, durability: %d, flavor: %d, texture: %d, calories: %d}", ingredient.name, ingredient.capacity, ingredient.durability, ingredient.flavor, ingredient.texture, ingredient.calories);
    return buf;
}

int main(int argc, char *argv[]) {
    Solution *solution = solution_create(2015, 15);
    size_t number_of_lines = 0;
    char **lines = get_file_as_lines(argv[1], &number_of_lines);

    Ingredient ingredients[number_of_lines];
    for(size_t i = 0; i < number_of_lines; i++) {
        char *line = lines[i];
        size_t number_of_tokens = 0;
        char **tokens = string_split(line, " ", &number_of_tokens);
        char *name = string_substring(tokens[0], 0, string_index_of(tokens[0], ':'));
        ingredients[i] = (Ingredient){
            .name = name,
            .capacity = atoi(tokens[2]),
            .durability = atoi(tokens[4]),
            .flavor = atoi(tokens[6]),
            .texture = atoi(tokens[8]),
            .calories = atoi(tokens[10])
        };
        char *ingredient_string = ingredient_to_string(ingredients[i]);
        printf("%s\n", ingredient_string);
        free(ingredient_string);
    }

    /*
     * Solve part 1
     * char *part_one = ...;
     * sprintf(solution->part_one.result, "%s", part_one);
     */
    solution_part_finalize(&solution->part_one);

    /* 
     * Solve part 2
     * char *part_two = ...;
     * sprintf(solution->part_two.result, "%s", part_two);
     */
    solution_part_finalize(&solution->part_two);

    solution_finalize(solution);
    solution_print(solution);

    return 0;
}
