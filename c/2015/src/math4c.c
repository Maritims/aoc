#include <gmp.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "math4c.h"

int math_number_places(int n) {
    if(n < 0) {
        return math_number_places(n == INT_MIN ? INT_MAX : -n);
    }
    if(n < 10) {
        return 1;
    }
    return 1 + math_number_places(n / 10);
}

uint64_t *math_binomial_coefficient(int n, int k) {
    mpz_t *n_factorial = math_factorial(n);
    mpz_t *k_factorial = math_factorial(k);
    mpz_t *n_minus_k_factorial = math_factorial(n - k);
    mpz_t binomial_coefficient;
    mpz_t denominator;

    mpz_init_set_ui(binomial_coefficient, 0);
    mpz_init_set_ui(denominator, 0);
    mpz_mul(denominator, *k_factorial, *n_minus_k_factorial);
    mpz_div(binomial_coefficient, *n_factorial, denominator);

    uint64_t *result = malloc(sizeof(uint64_t));
    *result = mpz_get_ui(binomial_coefficient);

    mpz_clear(*n_factorial);
    mpz_clear(*k_factorial);
    mpz_clear(*n_minus_k_factorial);
    mpz_clear(denominator);
    mpz_clear(binomial_coefficient);

    free(n_factorial);
    free(k_factorial);
    free(n_minus_k_factorial);

    return result;
}

int math_divide_and_round_up(int dividend, int divisor) {
    if (divisor == 0)
    {
        // Handle division by zero if necessary
        fprintf(stderr, "Error: Division by zero\n");
        return -1; // Or some other error code
    }
    return (dividend + (divisor - 1)) / divisor;
}

mpz_t *math_factorial(int n) {
    if(n < 0) {
        fprintf(stderr, "%s:%s:%d: parameter n cannot be %d. Factorial of a negative number doesn't exist.\n", __FILE__, __func__, __LINE__, n);
        return NULL;
    }

    mpz_t *result = malloc(sizeof(mpz_t));
    mpz_init(*result);
    mpz_set_ui(*result, 1);

    for(int i = 2; i <= n; i++) {
        mpz_t multiplier;
        mpz_init_set_ui(multiplier, i);
        mpz_mul_ui(*result, *result, i);
        mpz_clear(multiplier);
    }

    return result;
}

int *clone_subset(int *subset, size_t subset_length) {
    int *result = calloc(subset_length, sizeof(int));
    for(size_t i = 0; i < subset_length; i++) {
        result[i] = subset[i];
    }
    return result;
}

/**
 * compute_subsets: Helper function for computing subsets of an original array.
 * - Recursive.
 *
 * @param results: An array of int arrays to hold all the subsets.
 * @param results_size: The size of the results array thus far.
 * @param original_array: The original array of which to compute subsets.
 * @param original_array_size: The size of the original array.
 * @param subset: The current subset.
 * @param size_of_subset: The size of the current subset.
 * @param index: The index of the element in the original array to add to the subset. Also the lowest index of the original array we're interested in to achieve exclusion of elements.
 */
void sets_compute_subsets(int **results, size_t *results_size, size_t *results_column_sizes, int *original_array, size_t original_array_size, int *subset, size_t subset_size, size_t index) {
    int *cloned_subset                  = clone_subset(subset, subset_size);
    results[*results_size]              = cloned_subset;
    results_column_sizes[*results_size] = subset_size;
    (*results_size)++;

    for(size_t i = index; i < original_array_size; i++) {
        subset[subset_size] = original_array[i]; // Append to the end of the subset.
        sets_compute_subsets(results, results_size, results_column_sizes, original_array, original_array_size, subset, subset_size + 1, i + 1); // Step ahead in the subset and start at the next element in the original array.
    }
}

int **math_sets_compute_subsets(int *original_array, size_t original_array_size, size_t *return_results_size, size_t **return_results_column_sizes) {
    int number_of_subsets           = pow(2, original_array_size);
    int **subsets                   = calloc(number_of_subsets, sizeof(int*));
    size_t *results_column_sizes    = calloc(number_of_subsets, sizeof(size_t));
    int *subset                     = calloc(original_array_size, sizeof(int));
    size_t results_size             = 0;

    sets_compute_subsets(subsets, &results_size, results_column_sizes, original_array, original_array_size, subset, 0, 0);

    *return_results_size            = results_size;
    *return_results_column_sizes    = results_column_sizes;
    free(subset);

    return subsets;
}

bool math_sets_is_subset_sum(int *result, int *set, size_t length, int n, int sum, int index_in_result) {
    result[index_in_result] = set[n];

    // Base cases.
    if(sum < 0 || n < 0) {
        // We've gone too far. Get out!
        //memset(result, 0, length * sizeof(int));
        return false;
    }
    if(sum > 0 && n == 0) {
        // We've exhausted the set and the sum has not reached zero.
        //memset(result, 0, length * sizeof(int));
        return false;
    }
    if(sum == 0) {
        // The sum has reached zero. This is the way!
        return true;
    }

    // The first part follows the path of exclusion while the second part follows the path of inclusion.
    return math_sets_is_subset_sum(result, set, length, n - 1, sum, index_in_result + 1) || math_sets_is_subset_sum(result, set, length, n - 1, sum - set[n - 1], index_in_result + 1);
}

int math_max(int a, int b) {
    return a >= b ? a : b;
}

int math_min(int a, int b) {
    return a >= b ? b : a;
}

static void permute_ints_swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

static void permute_ints(int *elements, int left, int right, int **permutations, int *permutations_index) {
    if(left == right) {
        // We've generated a set of permutations.
        // Add the generated permutations to the permutations array.
        for(int i = 0; i <= right; i++) {
            permutations[*permutations_index][i] = elements[i];
        }
        (*permutations_index)++;
        return;
    }

    // Generate all permutations from left to right.
    for(int i = left; i <= right; i++) {
        permute_ints_swap((elements + left), (elements + i));
        permute_ints(elements, left + 1, right, permutations, permutations_index);
        permute_ints_swap((elements + left), (elements + i)); // Backtrack.
    }
}

int **math_permutations_compute_int(int n, size_t *out_permutations_length) {
    if(n < 0) {
        fprintf(stderr, "%s:%s:%d: unable to compute permutations for a negative number\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    mpz_t *factorial            = math_factorial(n);
    size_t permutations_length  = mpz_get_ui(*factorial);
    mpz_clear(*factorial);
    free(factorial);
    
    int **permutations = calloc(permutations_length, sizeof(int*));
    if(permutations == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to allocate memory for permutations\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    for(uint64_t i = 0; i < permutations_length; i++) {
        permutations[i] = calloc(n, sizeof(int));
    }

    int *elements = calloc(n, sizeof(int));
    if(elements == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to allocate memory for elements\n", __FILE__, __func__, __LINE__);
        for(uint64_t i = 0; i < permutations_length; i++) {
            free(permutations[i]);
        }
        free(permutations);
        return NULL;
    }

    for(int i = 0; i < n; i++) {
        elements[i] = i + 1;
    }

    int permutations_index = 0;
    permute_ints(elements, 0, n - 1, permutations, &permutations_index);

    free(elements);
    *out_permutations_length = permutations_length;
    return permutations;
}
/**
 * math_stars_and_bars: Compute the number of ways to split the positive number n into k parts.
 * @param n: The number to split.
 * @param k: The number of parts to split n into.
 * @return The number of ways to split n into k parts.
*/
uint64_t *math_stars_and_bars(int n, int k) {
    return math_binomial_coefficient(n + k - 1, k - 1);
}

