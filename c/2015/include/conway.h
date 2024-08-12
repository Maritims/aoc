#ifndef CONWAY_H
#define CONWAY_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "grid.h"

/**
 * conway: A type representing an instance of Conway's Game of Life.
 */
typedef struct conway_t conway_t;

size_t conway_get_grid_cols(conway_t *conway);

uint32_t conway_get_total_rounds(conway_t *conway);

void conway_set_verbose(conway_t *conway, bool verbose);

conway_t *conway_create(
        grid_t *grid,
        const uint32_t total_rounds,
        grid_point_t **always_enabled_cells,
        size_t always_enabled_cells_length);

void conway_reset(conway_t *conway);

void conway_destroy(conway_t *conway);

void conway_play_round(conway_t *conway);

uint32_t conway_play_all_rounds(conway_t *conway);

#endif
