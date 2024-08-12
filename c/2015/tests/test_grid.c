#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grid.h"
#include "testing/assertions.h"

void test_grid_parse(char **lines, size_t number_of_lines) {
    grid_t *grid = grid_parse(lines, number_of_lines, '#');
    if(grid == NULL) {
        printf("%s failed: unable to create grid\n", __func__);
        exit(EXIT_FAILURE);
    }

    printf("%s passed\n", __func__);
}

void test_grid_get_neighbours(size_t rows, size_t cols, int row, int col, int expected_neighbours[][2], size_t expected_number_of_neighbours) {
    size_t number_of_neighbours = 0;
    grid_t *grid                = grid_create(rows, cols);
    grid_point_t **neighbours   = grid_get_neighbours(grid, row, col, &number_of_neighbours);
   
    assert_not_null(neighbours, "%s\n", "neighbours was null");
    assert_primitive_equality(expected_number_of_neighbours, number_of_neighbours, "expected %zu neighbours, but got %zu\n", expected_number_of_neighbours, number_of_neighbours);

    for(size_t i = 0; i < 8; i++) {
        grid_point_t *neighbour = neighbours[i];
        int64_t neighbour_row = grid_point_get_row(neighbour);
        int64_t neighbour_col = grid_point_get_col(neighbour);

        if(neighbour_row != expected_neighbours[i][0] || neighbour_col != expected_neighbours[i][1]) {
            printf(
                "%s failed: expected neighbour[%zu] (%s) to be (%d,%d), but was (%lu,%lu)\n",
                __func__,
                i,
                grid_neighbour_str(i),
                expected_neighbours[i][0],
                expected_neighbours[i][1],
                neighbour_row,
                neighbour_col
            );
            exit(EXIT_FAILURE);
        }
    }

    printf("%s passed\n", __func__);
}

void test_grid_neighbour_str(int n, const char *expected) {
    const char *result = grid_neighbour_str(n);
    if(strcmp(expected, result) != 0) {
        printf("%s failed: grid_neighbour_str(%d, \"%s\") != %s\n", __func__, n, expected, result);
        exit(EXIT_FAILURE);
    }

    printf("%s(%d, \"%s\") passed\n", __func__, n, expected);
}

int main() {
    test_grid_parse((char *[6]){
        ".#.#.#",
        "...##.",
        "#....#",
        "..#...",
        "#.#..#",
        "####.."
    }, 6);
    test_grid_get_neighbours(6, 6, 0, 1, (int[8][2]){
        { GRID_CELL_UNDEFINED, GRID_CELL_UNDEFINED },   // NW
        { GRID_CELL_UNDEFINED, GRID_CELL_UNDEFINED },   // N
        { GRID_CELL_UNDEFINED, GRID_CELL_UNDEFINED },   // NE
        { 0, 0 },                                       // W
        { 0, 2 },                                       // E
        { 1, 0 },                                       // SW
        { 1, 1 },                                       // S
        { 1, 2 }                                        // SE
    }, 5);
    test_grid_neighbour_str(0, "NorthWest");
    test_grid_neighbour_str(1, "North");
    test_grid_neighbour_str(2, "NorthEast");
    test_grid_neighbour_str(3, "West");
    test_grid_neighbour_str(4, "East");
    test_grid_neighbour_str(5, "SouthWest");
    test_grid_neighbour_str(6, "South");
    test_grid_neighbour_str(7, "SouthEast");

    printf("All tests passed\n");
    return EXIT_SUCCESS;
}
