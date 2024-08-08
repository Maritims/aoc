#include "aoc.h"
#include "file4c.h"

void solve_part_one(char *instructions, solution_t *solution)
{
	int floor = 0;
	char instruction = '0';
	while((instruction = *instructions) != '\0')
	{
		switch(instruction)
		{
			case '(':
			floor++;
			break;
			case ')':
			floor--;
			break;
		}
		instructions++;
	}

	solution_part_finalize_with_int(solution, 0, floor, "280");
}

void solve_part_two(char *instructions, solution_t *solution)
{
	int floor = 0;
	int basement_entry_position = 1;
	char instruction;
	while((instruction = *instructions) != '\0')
	{
		switch(instruction)
		{
			case '(':
			floor++;
			break;
			case ')':
			floor--;
			break;
		}

		if(floor == -1)
		{
			break;
		}

		basement_entry_position++;
		instructions++;
	}

	solution_part_finalize_with_int(solution, 1, basement_entry_position, "1797");
}

int main(int argc, char* argv[]) {
    (void)argc;

	solution_t *solution = solution_create(2015, 1);
	char *instructions = file_read_all_text(argv[1]);
    char *temp = instructions;

	solve_part_one(instructions, solution);
	instructions = temp;
	solve_part_two(instructions, solution);

    free(instructions);
	return solution_finalize_and_destroy(solution);	
}
