#ifndef GRID
#define GRID

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

#include <stdint.h>

typedef GRID_OF_TYPE(uint64_t) GridU64;

CREATE_GRID_FOR(GridU64, uint64_t);

#endif