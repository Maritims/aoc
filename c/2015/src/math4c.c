#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "math4c.h"

int math_min(int a, int b)
{
    return a >= b ? b : a;
}

int math_max(int a, int b)
{
    return a >= b ? a : b;
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