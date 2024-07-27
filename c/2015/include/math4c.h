#ifndef MATH4C
#define MATH4C

#include <gmp.h>
#include <stdint.h>

void math_binomial_coefficient(uint64_t *result, int n, int k);

int math_divide_and_round_up(int dividend, int divisor);

void math_factorial(mpz_t *result, int n);

int math_max(int a, int b);

int math_min(int a, int b);

int **math_permutations_compute_int(int n, uint64_t **number_of_permutations);

void math_stars_and_bars(uint64_t *result, int n, int k);

#endif
