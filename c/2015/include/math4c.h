#ifndef MATH4C
#define MATH4C

#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>

int math_number_places(int n);

uint64_t *math_binomial_coefficient(int n, int k);

int math_divide_and_round_up(int dividend, int divisor);

mpz_t *math_factorial(int n);

int **math_sets_compute_subsets(int *original_array, size_t original_array_size, size_t *results_size, size_t **results_column_sizes);

bool math_sets_is_subset_sum(int *result, int *set, size_t length, int n, int sum, int index_in_result);

int math_max(int a, int b);

int math_min(int a, int b);

int **math_permutations_compute_int(int n, size_t *out_permutations_length);

uint64_t *math_stars_and_bars(int n, int k);

#endif
