#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "conway.h"
#include "aoc.h"
#include "grid.h"

struct conway_t {
    bool            verbose;
    grid_t          *grid;
    uint32_t        grid_enabled_cells;
    grid_point_t    **grid_always_enabled_cells;
    size_t          grid_always_enabled_cells_length;
    uint32_t        total_rounds;
    uint32_t        current_round;
};

static void set_always_enabled_cells(conway_t *conway, grid_point_t **always_enabled_cells, size_t always_enabled_cells_length) {
    if(always_enabled_cells != NULL && always_enabled_cells_length > 0) {
        for(size_t i = 0; i < always_enabled_cells_length; i++) {
            int64_t row = grid_point_get_row(always_enabled_cells[i]);
            int64_t col = grid_point_get_col(always_enabled_cells[i]);
            grid_set_cell_value(conway->grid, row, col, 1);
        }

        conway->grid_always_enabled_cells = always_enabled_cells;
        conway->grid_always_enabled_cells_length = always_enabled_cells_length;
    }
    else {
        conway->grid_always_enabled_cells = NULL;
        conway->grid_always_enabled_cells_length = 0;
    }
}

static void set_enabled_cells(conway_t *conway) {
    conway->grid_enabled_cells = 0;

    size_t rows = grid_get_rows(conway->grid);
    size_t cols = grid_get_cols(conway->grid);

    for(size_t row = 0; row < rows; row++) {
        for(size_t col = 0; col < cols; col++) {
            uint32_t grid_cell_value = grid_get_cell_value(conway->grid, row, col);
            if(grid_cell_value == 1) {
                conway->grid_enabled_cells++;
            }
        }
    }
}

size_t conway_get_grid_cols(conway_t *conway) {
    return grid_get_cols(conway->grid);
}

uint32_t conway_get_total_rounds(conway_t *conway) {
    return conway->total_rounds;
}

void conway_set_verbose(conway_t *conway, bool verbose) {
    conway->verbose = verbose;
}

conway_t *conway_create(
    grid_t *grid,
    uint32_t total_rounds,
    grid_point_t **always_enabled_cells,
    size_t always_enabled_cells_length
) {
    conway_t *conway = malloc(sizeof(conway_t));
    conway->verbose = false;
    conway->grid = grid;
    conway->current_round = 0;
    conway->total_rounds = total_rounds;

    set_always_enabled_cells(conway, always_enabled_cells, always_enabled_cells_length);
    set_enabled_cells(conway);

    return conway;
}

void conway_destroy(conway_t *conway) {
    grid_destroy(conway->grid);
    if(conway->grid_always_enabled_cells != NULL) {
        free((void*)conway->grid_always_enabled_cells);
    }
    free(conway);
}

static bool conway_grid_cell_is_always_enabled(conway_t *conway, uint32_t target_row, uint32_t target_col) {
    if(conway->grid_always_enabled_cells_length == 0) {
        if(conway->verbose) {
            printf("%s:%s:%d: %zu == 0\n", __FILE__, __func__, __LINE__, conway->grid_always_enabled_cells_length);
        }
        return false;
    }

    for(size_t row = 0; row < conway->grid_always_enabled_cells_length; row++) {
        grid_point_t *grid_point = conway->grid_always_enabled_cells[row];
        size_t row = grid_point_get_row(grid_point);
        size_t col = grid_point_get_col(grid_point);

        if(target_row == row && target_col == col) {
            return true;
        }
    }

    return false;
}

void conway_play_round(conway_t *conway) {
    grid_t *cloned_grid = grid_clone(conway->grid);
    size_t rows = grid_get_rows(conway->grid);
    size_t cols = grid_get_cols(conway->grid);

    for(size_t row = 0; row < rows; row++) {
        for(size_t col = 0; col < cols; col++) {
            size_t number_of_neighbours = 0;
            grid_point_t **neighbours   = grid_get_neighbours(conway->grid, row, col, &number_of_neighbours);
            uint32_t enabled_neighbours = 0;
            uint32_t is_enabled         = grid_get_cell_value(cloned_grid, row, col);

            for(size_t i = 0; i < 8; i++) {
                grid_point_t *neighbour = neighbours[i];
                int64_t neighbour_row    = grid_point_get_row(neighbour);
                int64_t neighbour_col    = grid_point_get_col(neighbour);

                if(neighbour_row == GRID_CELL_UNDEFINED || neighbour_col == GRID_CELL_UNDEFINED) {
                    continue;
                }

                size_t cloned_grid_cell_value = grid_get_cell_value(cloned_grid, neighbour_row, neighbour_col);
                if(cloned_grid_cell_value == 1) {
                    enabled_neighbours++;
                }
            }

            if(is_enabled == 1) {
                if(enabled_neighbours == 2 || enabled_neighbours == 3) {
                    // The cell stays enabled.
                }
                else if(!conway_grid_cell_is_always_enabled(conway, row, col)) {
                    grid_set_cell_value(conway->grid, row, col, 0);
                    conway->grid_enabled_cells--;
                }
            }
            else {
                if(enabled_neighbours == 3) {
                    grid_set_cell_value(conway->grid, row, col, 1);
                    conway->grid_enabled_cells++;
                }
                else {
                    // The cell stays disabled.
                }
            }

            FREE_ARRAY(neighbours, 8);
        }
    }

    if(conway->verbose) {
        printf("After step %d:\n", conway->current_round + 1);
        grid_print(conway->grid, '#');
        printf("\n");
    }

    grid_destroy(cloned_grid);
    conway->current_round++;
}

uint32_t conway_play_all_rounds(conway_t *conway) {
    uint32_t total_rounds = conway_get_total_rounds(conway);
    
    if(conway->verbose) {
        printf("Initial setup:\n");
        grid_print(conway->grid, '#');
        printf("\n");
    }

    for(uint32_t i = 0; i < total_rounds; i++) {
        conway_play_round(conway);
    }
    return conway->grid_enabled_cells;
}
