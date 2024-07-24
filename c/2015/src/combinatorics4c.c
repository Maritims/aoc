#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "combinatorics4c.h"

uint64_t *combinatorics_factorial_compute(int n) {
    if(n < 0) {
        fprintf(stderr, "Parameter \"n\" cannot be %d. Factorial of a negative number doesn't exist.\n", n);
        return NULL;
    }

    uint64_t *factorial = malloc(sizeof(uint64_t));
    *factorial = 1;
    for(int i = 2; i <= n; i++) {
        *factorial *= i;
    }
    return factorial;
}

static void permute_ints_swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

static COMBINATORICS_RESULT permute_ints(int *elements, int left, int right, int **permutations, int *permutations_index) {
    if(left == right) {
        // We've generated a set of permutations.
        // Add the generated permutations to the permutations array.
        for(int i = 0; i <= right; i++) {
            permutations[*permutations_index][i] = elements[i];
        }
        (*permutations_index)++;
        return COMBINATORICS_SUCCESS;
    }

    // Generate all permutations from left to right.
    for(int i = left; i <= right; i++) {
        permute_ints_swap((elements + left), (elements + i));
        permute_ints(elements, left + 1, right, permutations, permutations_index);
        permute_ints_swap((elements + left), (elements + i)); // Backtrack.
    }
    return COMBINATORICS_SUCCESS;
}

int **combinatorics_permutations_compute_int(int n, uint64_t **number_of_permutations) {
    *number_of_permutations = combinatorics_factorial_compute(n);
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
    printf("%s():%d: Generated %d permutations..\n", __func__, __LINE__, permutations_index);

    free(elements);
    return permutations;
}
