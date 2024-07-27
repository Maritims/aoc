#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "math4c.h"

void math_binomial_coefficient(uint64_t *result, int n, int k) {
    mpz_t n_factorial;
    mpz_t k_factorial;
    mpz_t n_minus_k_factorial;
    mpz_t binomial_coefficient;
    mpz_t denominator;

    math_factorial(&n_factorial, n);
    math_factorial(&k_factorial, k);
    math_factorial(&n_minus_k_factorial, n - k);

    mpz_init_set_ui(binomial_coefficient, 0);
    mpz_init_set_ui(denominator, 0);
    mpz_mul(denominator, k_factorial, n_minus_k_factorial);
    mpz_div(binomial_coefficient, n_factorial, denominator);

    *result = mpz_get_ui(binomial_coefficient);

    mpz_clear(n_factorial);
    mpz_clear(k_factorial);
    mpz_clear(n_minus_k_factorial);
    mpz_clear(denominator);
    mpz_clear(binomial_coefficient);
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

void math_factorial(mpz_t *result, int n) {
    if(n < 0) {
        fprintf(stderr, "Parameter \"n\" cannot be %d. Factorial of a negative number doesn't exist.\n", n);
        return;
    }

    mpz_init(*result);
    mpz_set_ui(*result, 1);
    for(int i = 2; i <= n; i++) {
        mpz_t multiplier;
        mpz_init_set_ui(multiplier, i);
        mpz_mul_ui(*result, *result, i);
        mpz_clear(multiplier);
    }
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

int **math_permutations_compute_int(int n, uint64_t **number_of_permutations) {
    mpz_t mpz_number_of_permutations;
    mpz_init_set_ui(mpz_number_of_permutations, **number_of_permutations);
    math_factorial(&mpz_number_of_permutations, n);
    **number_of_permutations = mpz_get_ui(mpz_number_of_permutations);
    mpz_clear(mpz_number_of_permutations);

    if(number_of_permutations == NULL) {
        fprintf(stderr, "%s():%d: Unable to compute the factorial of n = %d\n", __func__, __LINE__, n);
        return NULL;
    }
    
    int **permutations = calloc(**number_of_permutations, sizeof(int*));
    if(permutations == NULL) {
        fprintf(stderr, "%s():%d: Failed to allocate memory for permutations\n", __func__, __LINE__);
        free(number_of_permutations);
        return NULL;
    }
    for(uint64_t i = 0; i < **number_of_permutations; i++) {
        permutations[i] = calloc(n, sizeof(int));
    }

    int *elements = calloc(n, sizeof(int));
    if(elements == NULL) {
        fprintf(stderr, "%s():%d: Failed to allocate memory for elements\n", __func__, __LINE__);
        for(uint64_t i = 0; i < **number_of_permutations; i++) {
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
    return permutations;
}
/**
 * math_stars_and_bars: Compute the number of ways to split the positive number n into k parts.
 * @param n: The number to split.
 * @param k: The number of parts to split n into.
 * @return The number of ways to split n into k parts.
*/
void math_stars_and_bars(uint64_t *result, int n, int k) {
    math_binomial_coefficient(result, n + k - 1, k - 1);
}

