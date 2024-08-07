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

/**
 * Create a grid of rows x cols.
 * @param rows Number of rows in grid.
 * @param cols Number of columns in grid.
 * @return The newly created grid if successful, otherwise NULL.
 */
uint32_t **grid_create(uint32_t rows, uint32_t cols);

/**
 * grid_destroy: Free the memory allocated for a 2D array and all its rows.
 * @param grid: The 2D array.
 * @param rows: Size of first array dimension.
 */
void grid_destroy(uint32_t **grid, uint32_t rows);

/**
 * Create a grid based on lines of text.
 * @param lines Lines consisting of multiple instances of two different characters, one representing an enabled state and one representing a disabled state of cells in the grid.
 * @param number_of_lines: The number of lines to parse which is also the number of rows the grid will consist of.
 * @param number_of_columns The number of columns on each line.
 * @param on The character representing the enabled state of a cell in the grid.
 * @return The newly created grid if successful, otherwise NULL.
 */
uint32_t **grid_parse(char **lines, size_t number_of_lines, size_t *number_of_columns, char on);

/**
 * grid_print: Prints a 2D array to the standard output.
 * @param grid: The 2D array.
 * @param rows: Size of first array dimension.
 * @param cols: Size of second array dimension.
 * @param The character representing the enabled state of a cell in the grid.
 */
void grid_print(uint32_t **grid, uint32_t rows, uint32_t cols, char on);

/**
 * Clone a grid
 * @param grid A grid.
 * @param rows Number of rows in grid.
 * @param cols Number of columns in grid.
 * @return The cloned grid if successful, otherwise NULL.
 */
uint32_t **grid_clone(uint32_t **grid, uint32_t rows, uint32_t cols);

/**
 * Get the coordinates of the corners in a grid.
 * @param rows Number of rows in grid.
 * @param cols Number of columns in grid.
 * @return A pointer to an array containing pointers referencing all the corners in the grid.
 */
uint32_t **grid_get_corners(uint32_t rows, uint32_t cols);

/**
 * Destroy an array of corner pointers.
 * @param corners Array.
 */
void grid_destroy_corners(uint32_t **corners);

/**
 * Get all cells neighbouring a cell defined by row and column.
 * @param rows Number of rows in grid.
 * @param cols Number of columns in grid.
 * @param row Cell row.
 * @param col Cell column.
 * @return A pointer to an array of pointers referencing all the available travel directiones which can be applied to a grid to determine the neighbouring ells.
 */
int **grid_get_neighbours(int rows, int cols, uint32_t row, uint32_t col, size_t *number_of_neighbours);

/**
 * Convert an int between 0 (inclusive) and 8 (exclusive) to it's string representation in terms of direction in the grid.
 * @param n An integer between 0 (inclusive) and 8 (exclusive) representing a direction in the grid.
 * @return Returns either NorthWest (0), West (1), SouthWest (2), North (3), South (4), NorthEast (5), East (6) or SouthEast (7).
 */
const char *grid_neighbour_str(uint32_t n);

#endif
