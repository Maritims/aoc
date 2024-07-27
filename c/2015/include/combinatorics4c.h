#ifndef COMBINATORICS4C
#define COMBINATORICS4C

#include <stdint.h>

typedef enum COMBINATORICS_RESULT {
    COMBINATORICS_SUCCESS = 1,
    COMBINATORICS_FAILURE
} COMBINATORICS_RESULT;

int **combinatorics_permutations_compute_int(int n, uint64_t **number_of_permutations);

#endif
