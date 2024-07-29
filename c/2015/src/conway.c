#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

#include "conway.h"
#include "grid.h"

struct conway_t {
    bool verbose;
    int** grid;
    size_t grid_rows;
    size_t grid_cols;
    size_t grid_enabled_cells;
    int **grid_always_enabled_cells;
    size_t grid_always_enabled_cells_size;
    int total_rounds;
    int current_round;
};

size_t conway_get_grid_cols(conway_t *conway) {
    return conway->grid_cols;
}

int conway_get_total_rounds(conway_t *conway) {
    return conway->total_rounds;
}

void conway_set_verbose(conway_t *conway, bool verbose) {
    conway->verbose = verbose;
}

conway_t *conway_create(char **lines, size_t number_of_lines, char on, int total_rounds, int **always_enabled, size_t always_enabled_size) {
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
        for(size_t i = 0; i < always_enabled_size; i++) {
            int grid_row = always_enabled[i][0];
            int grid_col = always_enabled[i][1];

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

    for(size_t row = 0; row < conway->grid_rows; row++) {
        for(size_t col = 0; col < conway->grid_cols; col++) {
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
        for(size_t i = 0; i < conway->grid_always_enabled_cells_size; i++) {
            free(conway->grid_always_enabled_cells[i]);
        }
        free(conway->grid_always_enabled_cells);
    }

    free(conway);
}

static bool conway_grid_cell_is_always_enabled(conway_t *conway, size_t target_row, size_t target_col) {
    if(conway->grid_always_enabled_cells_size == 0) {
        if(conway->verbose) {
            printf("%s:%d: %zu == 0\n", __func__, __LINE__, conway->grid_always_enabled_cells_size);
        }
        return false;
    }

    for(size_t row = 0; row < conway->grid_always_enabled_cells_size; row++) {
        int grid_row = conway->grid_always_enabled_cells[row][0];
        int grid_col = conway->grid_always_enabled_cells[row][1];
        if(target_row == grid_row && target_col == grid_col) {
            return true;
        }
    }

    return false;
}

void conway_play_round(conway_t *conway) {
    int **cloned_grid = grid_clone(conway->grid, conway->grid_rows, conway->grid_cols);

    for(size_t row = 0; row < conway->grid_rows; row++) {
        for(size_t col = 0; col < conway->grid_cols; col++) {
            size_t number_of_neighbours;
            int **neighbours        = grid_get_neighbours(cloned_grid, conway->grid_rows, conway->grid_cols, row, col, &number_of_neighbours);
            int enabled_neighbours  = 0;
            int is_enabled          = cloned_grid[row][col];

            for(size_t i = 0; i < 8; i++) {
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

int conway_play_all_rounds(conway_t *conway) {
    int total_rounds = conway_get_total_rounds(conway);
    
    if(conway->verbose) {
        printf("Initial setup:\n");
        grid_print(conway->grid, conway->grid_rows, conway->grid_cols, '#');
        printf("\n");
    }

    for(int i = 0; i < total_rounds; i++) {
        conway_play_round(conway);
    }
    return conway->grid_enabled_cells;
}
