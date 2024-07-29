#include <stdbool.h>
#include <stdlib.h>

/**
 * conway: A type representing an instance of Conway's Game of Life.
 */
typedef struct conway_t conway_t;

/**
 * conway_get_grid_cols: Get the number of columns on each row in the grid.
 * @param conway: The game instance.
 * @return Returns the number of columns on each row in the grid.
 */
size_t conway_get_grid_cols(conway_t *conway);

/**
 * conway_get_total_rounds: Get the total rounds to play.
 * @return Returns an integer with the number of total rounds to play.
 */
int conway_get_total_rounds(conway_t *conway);

/**
 * @company_set_verbose: Sets the verbose property to the provided value.
 * @param conway: The game instance.
 * @param verbose: The new value.
 */
void conway_set_verbose(conway_t *conway, bool verbose);

/**
 * conway_create: Create an instance of Conway's Game of Life.
 * @param lines: Lines consisting of multiple instances of two different characters, one representing an enabled state and one representing a disabled of cells in the grid.
 * @param number_of_lines: The number of lines to parse.
 * @param on The character representing the enabled state of a cell in the grid.
 * @param total_rounds The number of rounds to play in total.
 * @param always_enabled A 2D array representing cells which are always enabled. These cells will never be turned off.
 * @param always_enabled_size Size of first array dimension in the always_enabled 2D array.
 */
conway_t *conway_create(char **lines, size_t number_of_lines, char on, int total_rounds, int **always_enabled, size_t always_enabled_size);

/**
 * conway_destroy: Destroy an instance of Conway's Game of Life.
 * @param conway: The instance to destroy.
 */
void conway_destroy(conway_t *conway);

/**
 * conway_play_round: Plays one round of Conway's Game of Life.
 * @param conway: The game instance.
 */
void conway_play_round(conway_t *conway);

/**
 * conway_play_all_rounds: Plays all the rounds of Conway's Game of Life.
 * @param conway: The game instance.
 * @return Returns the number of enabled cells after having played all the rounds of the game.
 */
int conway_play_all_rounds(conway_t *conway);
