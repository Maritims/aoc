#include <stdio.h>

#include "aoc.h"
#include "get_file.h"

void solve_part_one(char *instructions, SolutionPart *solution_part)
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

	sprintf(solution_part->result, "%d", floor);
	solution_part_finalize(solution_part);
}

void solve_part_two(char *instructions, SolutionPart *solution_part)
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

	sprintf(solution_part->result, "%d", basement_entry_position);
	solution_part_finalize(solution_part);
}

int main(int argc, char* argv[]) {
	Solution *solution = solution_create(2015, 1);

	char *instructions = get_file(argv[1]);
	char *tmp = instructions;

	solve_part_one(instructions, &(solution->part_one));
	instructions = tmp;
	solve_part_two(instructions, &(solution->part_two));

	solution_finalize(solution);
	solution_print(solution);
	
	return 0;
}
