#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "aoc.h"
#include "file4c.h"
#include "math4c.h"
#include "string4c.h"

typedef struct Ingredient {
    char *name;
    int capacity;
    int durability;
    int flavor;
    int texture;
    int calories;
} Ingredient;

typedef struct CookieIngredient {
    int amount;
    Ingredient ingredient;
} CookieIngredient;

void ingredient_parse(Ingredient *result, char *str) {
    size_t number_of_tokens;
    char *name;
    char **tokens;
    
    string_split(&tokens, &number_of_tokens, str, " ");
    string_substring(&name, tokens[0], 0, string_index_of(tokens[0], ':'));

    *result = (Ingredient){
        .name = name,
        .capacity = atoi(tokens[2]),
        .durability = atoi(tokens[4]),
        .flavor = atoi(tokens[6]),
        .texture = atoi(tokens[8]),
        .calories = atoi(tokens[10])
    };

    free(tokens);
}

typedef struct CookieScore {
    int total_capacity;
    int total_durability;
    int total_flavor;
    int total_texture;
    int total_calories;
    uint64_t total_score;
} CookieScore;

void cookie_compute_score(CookieScore *result, CookieIngredient cookie_ingredients[], size_t number_of_cookie_ingredients) {
    int total_capacity = 0;
    int total_durability = 0;
    int total_flavor = 0;
    int total_texture = 0;
    int total_calories = 0;

    for(size_t i = 0; i < number_of_cookie_ingredients; i++) {
        int amount = cookie_ingredients[i].amount;
        Ingredient ingredient = cookie_ingredients[i].ingredient;

        total_capacity += amount * ingredient.capacity;
        total_durability += amount * ingredient.durability;
        total_flavor += amount * ingredient.flavor;
        total_texture += amount * ingredient.texture;
        total_calories += amount * ingredient.calories;
    }

    if(total_capacity < 0) {
        total_capacity = 0;
    }

    if(total_durability < 0) {
        total_durability = 0;
    }

    if(total_flavor < 0) {
        total_flavor = 0;
    }

    if(total_texture < 0) {
        total_texture = 0;
    }

    uint64_t total_score = total_capacity * total_durability * total_flavor * total_texture;
    *result = (CookieScore){
        .total_capacity = total_capacity,
        .total_durability = total_durability,
        .total_flavor = total_flavor,
        .total_texture = total_texture,
        .total_calories = total_calories,
        .total_score = total_score
    };
}

char *ingredient_to_string(Ingredient ingredient) {
    char *buf = calloc(1024, sizeof(char));
    if(buf == NULL) {
        fprintf(stderr, "Failed to allocate memory for string representation of ingredient with name \"%s\"\n", ingredient.name);
        return NULL;
    }

    sprintf(buf, "{name: \"%s\", capacity: %d, durability: %d, flavor: %d, texture: %d, calories: %d}", ingredient.name, ingredient.capacity, ingredient.durability, ingredient.flavor, ingredient.texture, ingredient.calories);
    return buf;
}

/**
 * compute_sets: Compute every possible partition when partitioning n into k partitions.
 * @param n: The number to partition.
 * @param k: The number of partitions.
 * @param current_k: Represents the current index in the "set" array.
 * @param set: Temporary array for storing the current set. Its size must be equal to k. The size of the array is the responsibility of the caller.
 * @param sets: Two dimensional array for storing every computed set. Must be allocated to a size equal to the number of possible combinations. Allocation is the responsibility of the caller.
 */
void compute_sets(int n, int k, int current_k, int current_sum, int *set, int **sets, int *number_of_set) {
    // Base case. We've chosen enough items or exhausted the range. Store the result.
    if(current_k == k - 1) {
        set[current_k] = n - current_sum;
        for(int i = 0; i < k; i++) {
            sets[*number_of_set][i] = set[i];
        }
        (*number_of_set)++;
        return;
    }

    for(int i = 0; i <= n - current_sum; i++) {
        set[current_k] = i;
        compute_sets(n, k, current_k + 1, current_sum + i, set, sets, number_of_set);
    }
}

/**
 * Day 15: This problem is a variation of the "Knapsack problem".
 */
int main(int argc, char *argv[]) {
    Solution solution;
    char **lines;
    size_t number_of_lines = 0;
    uint64_t combinations;
    
    solution_create(&solution, 2015, 15);
    file_read_all_lines(&lines, &number_of_lines, argv[1]);
    math_stars_and_bars(&combinations, 100, number_of_lines);
    
    int *set = calloc(number_of_lines, sizeof(int));
    if(set == NULL) {
        fprintf(stderr, "%s:%d: Failed allocating memory for set: %s\n", __func__, __LINE__, strerror(errno));
        return 1;
    }

    int **sets = calloc(combinations, sizeof(int*));
    for(int i = 0; i < combinations; i++) {
        sets[i] = calloc(number_of_lines, sizeof(int));
    }
    
    int number_of_set = 0;
    compute_sets(100, number_of_lines, 0, 0, set, sets, &number_of_set);

    Ingredient ingredients[number_of_lines];
    for(size_t i = 0; i < number_of_lines; i++) {
        ingredient_parse(&ingredients[i], lines[i]);
    }
   
    uint64_t highest_cookie_score = 0;
    uint64_t highest_cookie_score_in_part_two = 0;
    for(int i = 0; i < combinations; i++) {
        set = sets[i];
        CookieIngredient cookie_ingredients[number_of_lines];
        for(int j = 0; j < number_of_lines; j++) {
            cookie_ingredients[j] = (CookieIngredient){
                .amount = set[j],
                .ingredient = ingredients[j]
            };
        }
        
        CookieScore cookie_score;
        cookie_compute_score(&cookie_score, cookie_ingredients, number_of_lines);
        
        if(cookie_score.total_score > highest_cookie_score) {
            highest_cookie_score = cookie_score.total_score;
        }

        if(cookie_score.total_calories == 500 && cookie_score.total_score > highest_cookie_score_in_part_two) {
            highest_cookie_score_in_part_two = cookie_score.total_score;
        }
    }

    solution_part_finalize_with_ui(&solution, 0, highest_cookie_score, "222870");
    solution_part_finalize_with_ui(&solution, 1, highest_cookie_score_in_part_two, "117936");


    free(set);
    free(sets);

    return solution_finalize(&solution);
}
