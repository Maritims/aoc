#include <limits.h>
#include <signal.h>
#include <stdbool.h>
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

struct grid_point_t {
    int64_t row;
    int64_t col;
};

struct grid_t {
    uint32_t **cells;
    size_t rows;
    size_t cols;
    struct {
        grid_point_t *top_left;
        grid_point_t *top_right;
        grid_point_t *bottom_left;
        grid_point_t *bottom_right;
    } corners;
};

static bool is_out_of_bounds(grid_t *grid, size_t row, size_t col) {
    return row >= grid->rows || col >= grid->cols;
}

uint32_t grid_get_cell_value(grid_t *grid, size_t row, size_t col) {
    if(is_out_of_bounds(grid, row, col)) {
        fprintf(stderr, "%s:%s:%d: (%zu,%zu) is out of bounds\n", __FILE__, __func__, __LINE__, row, col);
        raise(SIGSEGV);
    }

    return grid->cells[row][col];
}

void grid_set_cell_value(grid_t *grid, size_t row, size_t col, uint32_t value) {
    if(is_out_of_bounds(grid, row, col)) {
        fprintf(stderr, "%s:%s:%d: (%zu,%zu) is out of bounds\n", __FILE__, __func__, __LINE__, row, col);
        raise(SIGSEGV);
    }

    grid->cells[row][col] = value;
}

void grid_set_all_cell_values(grid_t *grid, uint32_t value) {
    for(size_t row = 0; row < grid->rows; row++) {
        for(size_t col = 0; col < grid->cols; col++) {
            grid_set_cell_value(grid, row, col, value);
        }
    }
}

size_t grid_get_rows(grid_t *grid) {
    return grid->rows;
}

size_t grid_get_cols(grid_t *grid) {
    return grid->cols;
}

grid_point_t *grid_get_top_left_corner(grid_t *grid) {
    return grid->corners.top_left;
}

grid_point_t *grid_get_top_right_corner(grid_t *grid) {
    return grid->corners.top_right;
}

grid_point_t *grid_get_bottom_left_corner(grid_t *grid) {
    return grid->corners.bottom_left;
}

grid_point_t *grid_get_bottom_right_corner(grid_t *grid) {
    return grid->corners.bottom_right;
}

grid_t *grid_create(size_t number_of_rows, size_t number_of_cols) {
    grid_t *grid    = malloc(sizeof(grid_t));
    grid->cells     = calloc(number_of_rows, sizeof(uint32_t*));
    grid->rows      = number_of_rows;
    grid->cols      = number_of_cols;

    for(size_t row = 0; row < number_of_rows; row++) {
        grid->cells[row] = calloc(number_of_cols, sizeof(uint32_t));
    }

    grid->corners.top_left = grid_point_create(0, 0);
    grid->corners.top_right = grid_point_create(0, grid->cols - 1);
    grid->corners.bottom_left = grid_point_create(grid->rows - 1, 0);
    grid->corners.bottom_right = grid_point_create(grid->rows - 1, grid->cols - 1);

    return grid;
}

void grid_destroy(grid_t *grid) {
    for(size_t row = 0; row < grid->rows; row++) {
        free(grid->cells[row]);
    }
    free(grid->cells);
    free(grid->corners.top_left);
    free(grid->corners.top_right);
    free(grid->corners.bottom_left);
    free(grid->corners.bottom_right);
    free(grid);
}

grid_t *grid_parse(char **lines, size_t number_of_lines, char on) {
    size_t number_of_columns = strlen(lines[0]);
    grid_t *grid = grid_create(number_of_lines, number_of_columns);

    for(size_t row = 0; row < number_of_lines; row++) {
        for(size_t col = 0; col < number_of_columns; col++) {
            grid->cells[row][col] = lines[row][col] == on ? 1 : 0;
        }
    }

    return grid;
}

void grid_print(grid_t *grid, char on) {
    for(size_t row = 0; row < grid->rows; row++) {
        for(size_t col = 0; col < grid->cols; col++) {
            printf("%c", grid->cells[row][col] == 1 ? on : '.');
        }
        printf("\n");
    }
}

grid_t *grid_clone(grid_t *grid) {
    grid_t *cloned_grid = grid_create(grid->rows, grid->cols);

    for(size_t row = 0; row < cloned_grid->rows; row++) {
        for(size_t col = 0; col < cloned_grid->cols; col++) {
            cloned_grid->cells[row][col] = grid->cells[row][col];
        }
    }

    return cloned_grid;
}

grid_point_t **grid_get_neighbours(grid_t *grid, size_t row, size_t col, size_t *number_of_neighbours) {
    int dx[] = { NORTH, NORTH, NORTH, NONE, NONE, SOUTH, SOUTH, SOUTH };
    int dy[] = {  WEST,  NONE,  EAST, WEST, EAST,  WEST,  NONE,  EAST };
    grid_point_t **neighbours = calloc(8, sizeof(grid_point_t*));

    for(uint32_t n = 0; n < 8; n++) {
        size_t new_row  = row + dx[n];
        size_t new_col  = col + dy[n];

        if(new_row < grid->rows && new_col < grid->cols) {
            neighbours[n] = grid_point_create(new_row, new_col);
            (*number_of_neighbours)++;
        }
        else {
            neighbours[n] = grid_point_create(GRID_CELL_UNDEFINED, GRID_CELL_UNDEFINED);
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

int64_t grid_point_get_row(grid_point_t *grid_point) {
    if(grid_point == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter grid_point was null\n", __FILE__, __func__, __LINE__);
        exit(EXIT_FAILURE);
    }

    return grid_point->row;
}

int64_t grid_point_get_col(grid_point_t *grid_point) {
    if(grid_point == NULL) {
        fprintf(stderr, "%s:%s:%d: parameter grid_point was null\n", __FILE__, __func__, __LINE__);
        exit(EXIT_FAILURE);
    }

    return grid_point->col;
}

grid_point_t *grid_point_create(size_t row, size_t col) {
    grid_point_t *grid_point = malloc(sizeof(grid_point_t));

    grid_point->row = row;
    grid_point->col = col;

    return grid_point;
}

grid_point_t **grid_point_create_array(size_t points[][2], size_t points_length) {
    grid_point_t **grid_points = calloc(points_length, sizeof(grid_point_t*));

    for(size_t i = 0; i < points_length; i++) {
        grid_point_t *grid_point = grid_point_create(points[i][0], points[i][1]);
        grid_points[i] = grid_point;
    }

    return grid_points;
}
