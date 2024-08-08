#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "math4c.h"
#include "testing/assertions.h"

void test_math_number_places(int n, int expected_result) {
    int result = math_number_places(n);
    assert_primitive_equality(expected_result, result, "math_number_places(%d) != %d (%d)\n", n, expected_result, result);
    printf("%s(%d, %d) passed\n", __func__, n, expected_result);
}

void test_math_sets_compute_subsets() {
    int *original_array     = (int[]){ 2, 4 };
    size_t return_results_size;
    size_t *return_results_column_sizes;
    int **results = math_sets_compute_subsets(original_array, 2, &return_results_size, &return_results_column_sizes);

    for(size_t i = 0; i < return_results_size; i++) {
        printf("[");
        for(size_t j = 0; j < return_results_column_sizes[i]; j++) {
            printf("%d", results[i][j]);
            if(j < return_results_column_sizes[i] - 1) {
                printf(", ");
            }
        }
        printf("]\n");

        free(results[i]);
    }

    free(return_results_column_sizes);
    free(results);

    printf("%s passed\n", __func__);
}

void test_math_sets_is_subset_sum(int *set, size_t length, int sum) {
    char set_as_str[1024];
    strcpy(set_as_str, "[");
    for(size_t i = 0; i < length; i++) {
        sprintf(set_as_str + strlen(set_as_str), "%d", set[i]);
        if(i < length - 1) {
            sprintf(set_as_str + strlen(set_as_str), ", ");
        }
    }
    sprintf(set_as_str + strlen(set_as_str), "]");

    int result[length];
    bool is_subset_sum = math_sets_is_subset_sum(result, set, length, length, sum, 0);
    assert_true(is_subset_sum, "math_sets_is_subset_sum(%s, %zu) != %d\n", set_as_str, length, sum);
    printf("%s passed: math_sets_is_subset_sum(%s, %zu) == %d\n", __func__, set_as_str, length, sum);
}

void test_math_stars_and_bars(int n, int k, uint64_t expected_result) {
    uint64_t *combinations = math_stars_and_bars(n, k);

    assert_primitive_equality(*combinations, expected_result, "math_stars_and_bars(%d, %d) != %lu\n", n, k, expected_result);

    free(combinations);
    printf("%s passed\n", __func__);
}

int main() {
    test_math_number_places(0, 1);
    test_math_number_places(1, 1);
    test_math_number_places(10, 2);
    test_math_number_places(100, 3);
    test_math_sets_compute_subsets();
    test_math_sets_is_subset_sum((int[]){ 1, 2, 3 }, 3, 5);
    test_math_sets_is_subset_sum((int[]){ 20, 15, 10, 5, 5 }, 5, 25);
    test_math_stars_and_bars(5, 3, 21);
    
    printf("All tests passed\n");
    return EXIT_SUCCESS;
}
