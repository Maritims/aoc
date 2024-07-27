#ifndef MATH4C
#define MATH4C

#include <gmp.h>
#include <stdint.h>

int math_min(int a, int b);

int math_max(int a, int b);

void math_factorial(mpz_t *result, int n);

void math_binomial_coefficient(uint64_t *result, int n, int k);

int divide_and_round_up(int dividend, int divisor);

void math_stars_and_bars(uint64_t *result, int n, int k);

#endif
