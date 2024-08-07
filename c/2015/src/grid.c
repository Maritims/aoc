#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grid.h"

#define NONE  GRID_DIRECTION_NONE
#define NORTH GRID_DIRECTION_NORTH
#define SOUTH GRID_DIRECTION_SOUTH
#define WEST  GRID_DIRECTION_WEST
#define EAST  GRID_DIRECTION_EAST

CREATE_GRID_IMPL_FOR(GridU64, uint64_t);

uint32_t **grid_create(uint32_t rows, uint32_t cols) {
    uint32_t **grid = calloc(rows, sizeof(uint32_t*));
    if(grid == NULL) {
        fprintf(stderr, "%s:%s:%d: failed to allocate memory for grid\n", __FILE__, __func__, __LINE__);
        return NULL;
    }

    for(uint32_t row = 0; row < rows; row++) {
        grid[row] = calloc(cols, sizeof(uint32_t*));
    
        if(grid[row] == NULL) {
            fprintf(stderr, "%s:%s:%d: Failed to allocate memory for row %d in grid\n", __FILE__, __func__, __LINE__, row);
            for(uint32_t i = 0; i < row; i++) {
                free(grid[i]);
            }
            free(grid);
            return NULL;
        }
    }

    return grid;
}

void grid_destroy(uint32_t **grid, uint32_t rows) {
    for(uint32_t row = 0; row < rows; row++) {
        free(grid[row]);
    }
    free(grid);
}

uint32_t **grid_parse(char **lines, size_t number_of_lines, size_t *number_of_columns, char on) {
    if(lines == NULL) {
        fprintf(stderr, "%s:%d: Parameter \"lines\" cannot be NULL\n", __func__, __LINE__);
        return NULL;
    }

    if(lines[0] == NULL) {
        fprintf(stderr, "%s:%d: Line 1 in parameter \"lines\" is NULL\n", __func__, __LINE__);
        return NULL;
    }

    *number_of_columns = strlen(lines[0]);
    uint32_t **grid = grid_create(number_of_lines, *number_of_columns);

    if(grid == NULL) {
        fprintf(stderr, "%s:%d: Failed to create grid with %zu rows and %zu cols\n", __func__, __LINE__, number_of_lines, *number_of_columns);
        return NULL;
    }

    for(size_t i = 0; i < number_of_lines; i++) {
        for(uint32_t j = 0; j < *number_of_columns; j++) {
            grid[i][j] = lines[i][j] == on ? 1 : 0;
        }
    }

    return grid;
}

void grid_print(uint32_t **grid, uint32_t rows, uint32_t cols, char on) {
    for(uint32_t row = 0; row < rows; row++) {
        for(uint32_t col = 0; col < cols; col++) {
            printf("%c", grid[row][col] == 1 ? on : '.');
        }
        printf("\n");
    }
}

uint32_t **grid_clone(uint32_t **grid, uint32_t rows, uint32_t cols) {
    uint32_t **cloned_grid = calloc(rows, sizeof(uint32_t*));

    if(cloned_grid == NULL) {
        fprintf(stderr, "%s:%d: Failed to allocate memory for cloned grid\n", __func__, __LINE__);
        return NULL;
    }

    for(uint32_t row = 0; row < rows; row++) {
        cloned_grid[row] = calloc(cols, sizeof(int));
    
        if(cloned_grid[row] == NULL) {
            fprintf(stderr, "%s:%s:%d: failed to allocate memory for row %d in grid\n", __FILE__, __func__, __LINE__, row);
            for(uint32_t i = 0; i < row; i++) {
                free(cloned_grid[i]);
            }
            free(cloned_grid);
            return NULL;
        }

        for(uint32_t col = 0; col < cols; col++) {
            cloned_grid[row][col] = grid[row][col];
        }
    }

    return cloned_grid;
}

uint32_t **grid_get_corners(uint32_t rows, uint32_t cols) {
    uint32_t **corners = calloc(4, sizeof(uint32_t*));
    for(uint32_t i = 0; i < 4; i++) {
        corners[i] = calloc(2, sizeof(uint32_t));
    }

    // Top left corner.
    corners[0][0] = 0;
    corners[0][1] = 0;

    // Top right corner.
    corners[1][0] = 0;
    corners[1][1] = cols - 1;

    // Bottom left corner.
    corners[2][0] = rows - 1;
    corners[2][1] = 0;

    // Bottom right corner.
    corners[3][0] = rows - 1;
    corners[3][1] = cols - 1;

    return corners;
}

void grid_destroy_corners(uint32_t **corners) {
    for(uint32_t i = 0; i < 4; i++) {
        free(corners[i]);
    }
    free(corners);
}

int **grid_get_neighbours(int rows, int cols, uint32_t row, uint32_t col, size_t *number_of_neighbours) {
    *number_of_neighbours = 0;

    int dx[]            = { NORTH, NORTH, NORTH, NONE, NONE, SOUTH, SOUTH, SOUTH };
    int dy[]            = {  WEST,  NONE,  EAST, WEST, EAST,  WEST,  NONE,  EAST };
    int **neighbours    = calloc(8, sizeof(int*));

    if(neighbours == NULL) {
        fprintf(stderr, "%s:%d: Failed to allocate memory for grid neighbours\n", __func__, __LINE__);
        return NULL;
    }

    for(uint32_t n = 0; n < 8; n++) {
        int new_row     = row + dx[n];
        int new_col     = col + dy[n];
        neighbours[n]   = calloc(2, sizeof(int));
        
        if(neighbours[n] == NULL) {
            fprintf(stderr, "%s:%d: Failed to allocate memory for grid neighbour (%d,%d) next to grid cell (%d,%d)\n", __func__, __LINE__, new_row, new_col, row, col);
            for(uint32_t i = 0; i < n; i++) {
                free(neighbours[i]);
            }
            free(neighbours);
            return NULL;
        }

        if(new_row >= 0 && new_row < rows && new_col >= 0 && new_col < cols) {
            neighbours[n][0]    = new_row;
            neighbours[n][1]    = new_col;
            (*number_of_neighbours)++;
        }
        else {
            neighbours[n][0]    = GRID_CELL_UNDEFINED;
            neighbours[n][1]    = GRID_CELL_UNDEFINED;
        }
    }

    return neighbours;
}

const char *grid_neighbour_str(uint32_t n) {
    switch(n) {
        case 0:
        return "NorthWest";
        case 1:
        return "North";
        case 2:
        return "NorthEast";
        case 3:
        return "West";
        case 4:
        return "East";
        case 5:
        return "SouthWest";
        case 6:
        return "South";
        case 7:
        return "SouthEast";
        default:
        fprintf(stderr, "%s:%d: %d is an invalid value\n", __func__, __LINE__, n);
        return NULL;
    }
}
