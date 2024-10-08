#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc.h"
#include "file4c.h"
#include "math4c.h"

typedef struct RightRectangularPrism
{
	uint32_t width;
	uint32_t height;
	uint32_t length;
	uint32_t area;
	uint32_t volume;
	uint32_t width_by_height;
	uint32_t width_by_length;
	uint32_t height_by_length;
	uint32_t smallest_side;
} RightRectangularPrism;

RightRectangularPrism *create_right_rectangular_prism(char *line)
{
	RightRectangularPrism *prism = malloc(sizeof(RightRectangularPrism));
	char *copy = strdup(line);

	prism->height = atoi(strtok(copy, "x"));
	prism->length = atoi(strtok(NULL, "x"));
	prism->width = atoi(strtok(NULL, "x"));

	free(copy);

	prism->volume = prism->height * prism->length * prism->width;
	prism->width_by_height = prism->width * prism->height;
	prism->width_by_length = prism->width * prism->length;
	prism->height_by_length = prism->height * prism->length;

	if (prism->width_by_height <= prism->width_by_length && prism->width_by_height <= prism->height_by_length)
	{
		prism->smallest_side = prism->width_by_height;
	}
	else if (prism->width_by_length <= prism->width_by_height && prism->width_by_length <= prism->height_by_length)
	{
		prism->smallest_side = prism->width_by_length;
	}
	else
	{
		prism->smallest_side = prism->height_by_length;
	}

	prism->area = (2 * prism->length * prism->width) + (2 * prism->width * prism->height) + (2 * prism->height * prism->length) + prism->smallest_side;

	return prism;
}

void solve_part_one(char **lines, size_t length, solution_t *solution) {
	uint32_t total_area = 0;
	for(size_t i = 0; i < length; i++)
	{
		RightRectangularPrism *prism = create_right_rectangular_prism(lines[i]);
		total_area += prism->area;
        free(prism);
	}

	solution_part_finalize_with_int(solution, 0, total_area, "1588178");
}

void solve_part_two(char **lines, size_t length, solution_t *solution) {
	uint32_t total_ribbon_length = 0;
	for(size_t i = 0; i < length; i++)
	{
		RightRectangularPrism *prism = create_right_rectangular_prism(lines[i]);

		uint32_t min_value = math_min(prism->width, math_min(prism->height, prism->length));
		uint32_t max_value = math_max(prism->width, math_max(prism->height, prism->length));
		uint32_t remaining_value = prism->width + prism->height + prism->length - min_value - max_value;

		total_ribbon_length += min_value + min_value + remaining_value + remaining_value + prism->volume;
        free(prism);
	}

	solution_part_finalize_with_int(solution, 1, total_ribbon_length, "3783758");
}

int main(int argc, char *argv[]) {
    (void)argc;

	solution_t *solution = solution_create(2015, 2);
    size_t number_of_lines = 0;
    char **lines = file_read_all_lines(&number_of_lines, argv[1]);

	solve_part_one(lines, number_of_lines, solution);
	solve_part_two(lines, number_of_lines, solution);

    FREE_ARRAY(lines, number_of_lines);
	return solution_finalize_and_destroy(solution);
}
