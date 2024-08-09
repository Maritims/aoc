#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "aoc.h"
#include "file4c.h"
#include "math4c.h"
#include "string4c.h"

typedef struct ingredient_t {
    char name[256];
    int capacity;
    int durability;
    int flavor;
    int texture;
    int calories;
} ingredient_t;

typedef struct cookie_ingredient_t {
    int amount;
    ingredient_t ingredient;
} cookie_ingredient_t;

ingredient_t *ingredient_parse(char *str) {
    size_t number_of_tokens = 0;
    char **tokens           = string_split(&number_of_tokens, str, " ");
    char *name              = string_substring(tokens[0], 0, (size_t)(strchr(tokens[0], ':') - tokens[0]));
    size_t name_length      = strlen(name);
    ingredient_t *result    = malloc(sizeof(ingredient_t));

    strcpy(result->name, name);
    result->name[name_length]   = '\0';
    result->capacity            = atoi(tokens[2]);
    result->durability          = atoi(tokens[4]);
    result->flavor              = atoi(tokens[6]);
    result->texture             = atoi(tokens[8]);
    result->calories            = atoi(tokens[10]);

    free(name);
    FREE_ARRAY(tokens, number_of_tokens);
    return result;
}

typedef struct cookie_score_t {
    int total_capacity;
    int total_durability;
    int total_flavor;
    int total_texture;
    int total_calories;
    uint64_t total_score;
} cookie_score_t;

cookie_score_t *cookie_compute_score(cookie_ingredient_t cookie_ingredients[], size_t number_of_cookie_ingredients) {
    int total_capacity = 0;
    int total_durability = 0;
    int total_flavor = 0;
    int total_texture = 0;
    int total_calories = 0;

    for(size_t i = 0; i < number_of_cookie_ingredients; i++) {
        int amount = cookie_ingredients[i].amount;
        ingredient_t ingredient = cookie_ingredients[i].ingredient;

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
    cookie_score_t *result = malloc(sizeof(cookie_score_t));
    result->total_capacity = total_capacity;
    result->total_durability = total_durability;
    result->total_flavor = total_flavor;
    result->total_texture = total_texture;
    result->total_calories = total_calories;
    result->total_score = total_score;

    return result;
}

char *ingredient_to_string(ingredient_t ingredient) {
    char *buf = calloc(1024, sizeof(char));
    if(buf == NULL) {
        fprintf(stderr, "Failed to allocate memory for string representation of ingredient with name \"%s\"\n", ingredient.name);
        return NULL;
    }

    sprintf(buf, "{name: \"%s\", capacity: %d, durability: %d, flavor: %d, texture: %d, calories: %d}", ingredient.name, ingredient.capacity, ingredient.durability, ingredient.flavor, ingredient.texture, ingredient.calories);
    return buf;
}

/**
 * Compute every possible partition when partitioning n into k partitions.
 * @param n The number to partition.
 * @param k The number of partitions.
 * @param current_k Represents the current index in the "set" array.
 * @param set Temporary array for storing the current set. Its size must be equal to k. The size of the array is the responsibility of the caller.
 * @param sets Two dimensional array for storing every computed set. Must be allocated to a size equal to the number of possible combinations. Allocation is the responsibility of the caller.
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
    (void)argc;
    
    solution_t *solution    = solution_create(2015, 15);
    size_t number_of_lines  = 0;
    char **lines            = file_read_all_lines(&number_of_lines, argv[1]);
    uint64_t *combinations  = math_stars_and_bars(100, number_of_lines);
    
    int *set            = calloc(number_of_lines, sizeof(int));
    int *original_set   = set;
    int **sets          = calloc(*combinations, sizeof(int*));

    for(uint64_t i = 0; i < *combinations; i++) {
        sets[i] = calloc(number_of_lines, sizeof(int));
    }
    
    int number_of_set = 0;
    compute_sets(100, number_of_lines, 0, 0, set, sets, &number_of_set);

    ingredient_t **ingredients = calloc(number_of_lines, sizeof(ingredient_t*));
    for(size_t i = 0; i < number_of_lines; i++) {
        ingredients[i] = ingredient_parse(lines[i]);
    }
   
    uint64_t highest_cookie_score = 0;
    uint64_t highest_cookie_score_in_part_two = 0;
    for(uint64_t i = 0; i < *combinations; i++) {
        set = sets[i];
        cookie_ingredient_t cookie_ingredients[number_of_lines];
        for(size_t j = 0; j < number_of_lines; j++) {
            cookie_ingredients[j] = (cookie_ingredient_t){
                .amount = set[j],
                .ingredient = *ingredients[j]
            };
        }
        
        cookie_score_t *cookie_score = cookie_compute_score(cookie_ingredients, number_of_lines);
        
        if(cookie_score->total_score > highest_cookie_score) {
            highest_cookie_score = cookie_score->total_score;
        }

        if(cookie_score->total_calories == 500 && cookie_score->total_score > highest_cookie_score_in_part_two) {
            highest_cookie_score_in_part_two = cookie_score->total_score;
        }

        free(cookie_score);
    }

    solution_part_finalize_with_ui(solution, 0, highest_cookie_score, "222870");
    solution_part_finalize_with_ui(solution, 1, highest_cookie_score_in_part_two, "117936");

    free(original_set);
    FREE_ARRAY(ingredients, number_of_lines);
    FREE_ARRAY(sets, *combinations);
    FREE_ARRAY(lines, number_of_lines);
    free(combinations);
    return solution_finalize_and_destroy(solution);
}
