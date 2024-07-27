#include <gmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "math4c.h"

int math_min(int a, int b)
{
    return a >= b ? b : a;
}

int math_max(int a, int b)
{
    return a >= b ? a : b;
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

/**
 * math_stars_and_bars: Compute the number of ways to split the positive number n into k parts.
 * @param n: The number to split.
 * @param k: The number of parts to split n into.
 * @return The number of ways to split n into k parts.
*/
void math_stars_and_bars(uint64_t *result, int n, int k) {
    math_binomial_coefficient(result, n + k - 1, k - 1);
}

int divide_and_round_up(int dividend, int divisor)
{
    if (divisor == 0)
    {
        // Handle division by zero if necessary
        fprintf(stderr, "Error: Division by zero\n");
        return -1; // Or some other error code
    }
    return (dividend + (divisor - 1)) / divisor;
}
