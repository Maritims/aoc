#ifndef GRID
#define GRID

#include <stdint.h>
#include <stdio.h>

#define GRID_CELL_UNDEFINED INT_MIN

#define GRID_DIRECTION_NONE 0
#define GRID_DIRECTION_NORTH -1
#define GRID_DIRECTION_SOUTH 1
#define GRID_DIRECTION_WEST -1
#define GRID_DIRECTION_EAST 1

#define GRID_OF_TYPE(T) \
    struct              \
    {                   \
        T **table;      \
        size_t rows;    \
        size_t columns; \
    }

#define CREATE_GRID_FOR(TGrid, TCell) \
    TGrid *grid_create_##TGrid(size_t rows, size_t columns, TCell default_value)

#define CREATE_GRID_IMPL_FOR(TGrid, TCell)                                       \
    TGrid *grid_create_##TGrid(size_t rows, size_t columns, TCell default_value) \
    {                                                                            \
        TGrid *grid = malloc(sizeof(TGrid));                                     \
        grid->rows = rows;                                                       \
        grid->columns = columns;                                                 \
        grid->table = calloc(rows, sizeof(TGrid));                               \
        for (size_t row = 0; row < rows; row++)                                  \
        {                                                                        \
            grid->table[row] = calloc(columns, sizeof(TCell));                   \
            for (size_t column = 0; column < columns; column++)                  \
            {                                                                    \
                grid->table[row][column] = default_value;                        \
            }                                                                    \
        }                                                                        \
        return grid;                                                             \
    }

typedef GRID_OF_TYPE(uint64_t) GridU64;

CREATE_GRID_FOR(GridU64, uint64_t);

typedef struct grid_t grid_t;

typedef struct grid_point_t grid_point_t;

uint32_t grid_get_cell_value(grid_t *grid, size_t row, size_t col);

void grid_set_cell_value(grid_t *grid, size_t row, size_t col, uint32_t value);

void grid_set_all_cell_values(grid_t *grid, uint32_t value);

size_t grid_get_rows(grid_t *grid);

size_t grid_get_cols(grid_t *grid);

grid_point_t *grid_get_top_left_corner(grid_t *grid);

grid_point_t *grid_get_top_right_corner(grid_t *grid);

grid_point_t *grid_get_bottom_left_corner(grid_t *grid);

grid_point_t *grid_get_bottom_right_corner(grid_t *grid);

grid_t *grid_create(size_t number_of_rows, size_t number_of_cols);

void grid_destroy(grid_t *grid);

grid_t *grid_parse(char **lines, size_t number_of_lines, char on);

void grid_print(grid_t *grid, char on);

grid_t *grid_clone(grid_t *grid);

/**
 * Get all cells neighbouring a cell defined by row and column.
 * @param row Cell row.
 * @param col Cell column.
 * @return A pointer to an array of pointers referencing all the available travel directiones which can be applied to a grid to determine the neighbouring ells.
 */
grid_point_t **grid_get_neighbours(grid_t *grid, size_t row, size_t col, size_t *number_of_neighbours);

/**
 * Convert an int between 0 (inclusive) and 8 (exclusive) to it's string representation in terms of direction in the grid.
 * @param n An integer between 0 (inclusive) and 8 (exclusive) representing a direction in the grid.
 * @return Returns either NorthWest (0), West (1), SouthWest (2), North (3), South (4), NorthEast (5), East (6) or SouthEast (7).
 */
const char *grid_neighbour_str(uint32_t n);

int64_t grid_point_get_row(grid_point_t *grid_point);

int64_t grid_point_get_col(grid_point_t *grid_point);

grid_point_t *grid_point_create(size_t row, size_t col);

grid_point_t **grid_point_create_array(size_t grid_points[][2], size_t grid_points_length);

#endif
