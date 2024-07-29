#ifndef GRID
#define GRID

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

/**
 * grid_create: Creates 2D array of integers.
 * @param rows: Size of first array dimension.
 * @param cols: Size of second array dimension.
 * @return Returns a 2D array of integers, or NULL if array creation fails.
 */
int **grid_create(size_t rows, size_t cols);

/**
 * grid_destroy: Free the memory allocated for a 2D array and all its rows.
 * @param grid: The 2D array.
 * @param rows: Size of first array dimension.
 */
void grid_destroy(int **grid, size_t rows);

/**
 * grid_parse: Creates a 2D array from a list of lines.
 * @param lines: Lines consisting of multiple instances of two different characters, one representing an enabled state and one representing a disabled state of cells in the grid.
 * @param number_of_lines: The number of lines to parse which is also the number of rows the grid will consist of.
 * @param number_of_columns: The number of columns on each line.
 * @param on The character representing the enabled state of a cell in the grid.
 * @return Returns a 2D array of integers, or NULL if grid creation fails.
 */
int **grid_parse(char **lines, size_t number_of_lines, size_t *number_of_columns, char on);

/**
 * grid_print: Prints a 2D array to the standard output.
 * @param grid: The 2D array.
 * @param rows: Size of first array dimension.
 * @param cols: Size of second array dimension.
 * @param The character representing the enabled state of a cell in the grid.
 */
void grid_print(int **grid, size_t rows, size_t cols, char on);

/**
 * grid_clone: Clones a 2D array.
 * @param grid: The 2D Array.
 * @param rows: Size of first array dimension.
 * @param cols: Size of second array dimension.
 * @return Returns the cloned array.
 */
int **grid_clone(int **grid, size_t rows, size_t cols);

/**
 * grid_get_corners: Get the coordinates of the grid corners.
 * @param rows: Size of first array dimension.
 * @param cols: Size of second array dimension.
 * @return Returns a 2D array of integers containing references to each corner's coordinates in the grid.
 */
int **grid_get_corners(size_t rows, size_t cols);

/**
 * grid_destroy_corners: Destroy a corners array.
 * @param corners: The 2D array to destroy.
 */
void grid_destroy_corners(int **corners);

/**
 * grid_get_neighbours: Returns all cells neighbouring the cell defined by the row and col parameters.
 * @param grid: The 2D array to search.
 * @param rows: Size of first array dimension.
 * @param cols: Size of second array dimension.
 * @param row: Value in the first array dimension identifying the target cell.
 * @param col: Value in the second array dimension identifying the target cell.
 * @return Returns a 2D array of integers containing references to each neighbour's coordinates in the grid.
 */
int **grid_get_neighbours(int **grid, size_t rows, size_t cols, size_t row, size_t col, size_t *number_of_neighbours);

/**
 * grid_neighbour_str: Converts an int between 0 (inclusive) and 8 (exclusive) to it's string representation in terms of direction in the grid.
 * @param n An integer between 0 (inclusive) and 8 (exclusive) representing a direction in the grid.
 * @return Returns either NorthWest (0), West (1), SouthWest (2), North (3), South (4), NorthEast (5), East (6) or SouthEast (7).
 */
const char *grid_neighbour_str(int n);

#endif
