#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "conway.h"
#include "grid.h"

struct conway_t {
    bool        verbose;
    uint32_t**  grid;
    uint32_t    grid_rows;
    uint32_t    grid_cols;
    uint32_t    grid_enabled_cells;
    uint32_t**  grid_always_enabled_cells;
    uint32_t    grid_always_enabled_cells_size;
    uint32_t    total_rounds;
    uint32_t    current_round;
};

uint32_t conway_get_grid_cols(conway_t *conway) {
    return conway->grid_cols;
}

uint32_t conway_get_total_rounds(conway_t *conway) {
    return conway->total_rounds;
}

void conway_set_verbose(conway_t *conway, bool verbose) {
    conway->verbose = verbose;
}

conway_t *conway_create(char **lines, uint32_t number_of_lines, char on, uint32_t total_rounds, uint32_t **always_enabled, uint32_t always_enabled_size) {
    conway_t *conway = malloc(sizeof(conway_t));
    size_t number_of_columns;

    conway->verbose = false;
    conway->grid    = grid_parse(lines, number_of_lines, &number_of_columns, on);
    
    if(conway->grid == NULL) {
        fprintf(stderr, "%s:%d: Failed to create grid for Conway's Game of Life\n", __func__, __LINE__);
        free(conway);
        return NULL;
    }

    conway->grid_rows                       = number_of_lines;
    conway->grid_cols                       = number_of_columns;
    conway->grid_enabled_cells              = 0;
    conway->grid_always_enabled_cells_size  = always_enabled_size;
    conway->total_rounds                    = total_rounds;
    conway->current_round                   = 0;
    
    if(always_enabled_size > 0) {
        conway->grid_always_enabled_cells = calloc(always_enabled_size, sizeof(int*));
        for(uint32_t i = 0; i < always_enabled_size; i++) {
            uint32_t grid_row = always_enabled[i][0];
            uint32_t grid_col = always_enabled[i][1];

            if(conway->verbose) {
                printf("%s:%d: (%d,%d) is always enabled\n", __func__, __LINE__, grid_row, grid_col);
            }

            conway->grid_always_enabled_cells[i] = calloc(2, sizeof(int));
            conway->grid_always_enabled_cells[i][0] = always_enabled[i][0];
            conway->grid_always_enabled_cells[i][1] = always_enabled[i][1];

            conway->grid[grid_row][grid_col] = 1;
        }
    }
    else {
        conway->grid_always_enabled_cells = NULL;
    }

    for(uint32_t row = 0; row < conway->grid_rows; row++) {
        for(uint32_t col = 0; col < conway->grid_cols; col++) {
            if(conway->grid[row][col] == 1) {
                conway->grid_enabled_cells++;
            }
        }
    }

    return conway;
}

void conway_destroy(conway_t *conway) {
    grid_destroy(conway->grid, conway->grid_rows);
    
    if(conway->grid_always_enabled_cells_size > 0) {
        for(uint32_t i = 0; i < conway->grid_always_enabled_cells_size; i++) {
            free(conway->grid_always_enabled_cells[i]);
        }
        free(conway->grid_always_enabled_cells);
    }

    free(conway);
}

static bool conway_grid_cell_is_always_enabled(conway_t *conway, uint32_t target_row, uint32_t target_col) {
    if(conway->grid_always_enabled_cells_size == 0) {
        if(conway->verbose) {
            printf("%s:%s:%d: %d == 0\n", __FILE__, __func__, __LINE__, conway->grid_always_enabled_cells_size);
        }
        return false;
    }

    for(uint32_t row = 0; row < conway->grid_always_enabled_cells_size; row++) {
        uint32_t grid_row = conway->grid_always_enabled_cells[row][0];
        uint32_t grid_col = conway->grid_always_enabled_cells[row][1];
        if(target_row == grid_row && target_col == grid_col) {
            return true;
        }
    }

    return false;
}

void conway_play_round(conway_t *conway) {
    uint32_t **cloned_grid = grid_clone(conway->grid, conway->grid_rows, conway->grid_cols);

    for(uint32_t row = 0; row < conway->grid_rows; row++) {
        for(uint32_t col = 0; col < conway->grid_cols; col++) {
            size_t number_of_neighbours;
            int **neighbours   = grid_get_neighbours(conway->grid_rows, conway->grid_cols, row, col, &number_of_neighbours);
            uint32_t enabled_neighbours  = 0;
            uint32_t is_enabled          = cloned_grid[row][col];

            for(uint32_t i = 0; i < 8; i++) {
                int neighbour_row = neighbours[i][0];
                int neighbour_col = neighbours[i][1];

                if(neighbour_row == GRID_CELL_UNDEFINED || neighbour_col == GRID_CELL_UNDEFINED) {
                    continue;
                }

                if(cloned_grid[neighbour_row][neighbour_col] == 1) {
                    enabled_neighbours++;
                }
            }

            if(is_enabled == 1) {
                if(enabled_neighbours == 2 || enabled_neighbours == 3) {
                    // The cell stays enabled.
                }
                else if(!conway_grid_cell_is_always_enabled(conway, row, col)) {
                    conway->grid[row][col] = 0;
                    conway->grid_enabled_cells--;
                }
            }
            else {
                if(enabled_neighbours == 3) {
                    conway->grid[row][col] = 1;
                    conway->grid_enabled_cells++;
                }
                else {
                    // The cell stays disabled.
                }
            }

            free(neighbours[0]);
            free(neighbours[1]);
            free(neighbours);
        }
    }

    if(conway->verbose) {
        printf("After step %d:\n", conway->current_round + 1);
        grid_print(conway->grid, conway->grid_rows, conway->grid_cols, '#');
        printf("\n");
    }

    grid_destroy(cloned_grid, conway->grid_rows);
    conway->current_round++;
}

uint32_t conway_play_all_rounds(conway_t *conway) {
    uint32_t total_rounds = conway_get_total_rounds(conway);
    
    if(conway->verbose) {
        printf("Initial setup:\n");
        grid_print(conway->grid, conway->grid_rows, conway->grid_cols, '#');
        printf("\n");
    }

    for(uint32_t i = 0; i < total_rounds; i++) {
        conway_play_round(conway);
    }
    return conway->grid_enabled_cells;
}
