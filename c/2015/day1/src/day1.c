#include "aoc.h"
#include "file4c.h"

void solve_part_one(char *instructions, Solution *solution)
{
	int floor = 0;
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
		instructions++;
	}

	solution_part_finalize_with_int(solution, 0, floor, "280");
}

void solve_part_two(char *instructions, Solution *solution)
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

    Solution solution;
    char *instructions;
    char *tmp;

	solution_create(&solution, 2015, 1);
	file_read_all_text(&instructions, argv[1]);
	
    tmp = instructions;

	solve_part_one(instructions, &solution);
	instructions = tmp;
	solve_part_two(instructions, &solution);

	return solution_finalize(&solution);	
}
