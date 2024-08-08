#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "aoc.h"
#include "file4c.h"
#include "hashtable.h"

#define DEBUG 1

typedef struct
{
    int x;
    int y;
} Point2D;

void point2d_move(Point2D *point, char instruction)
{
    switch (instruction)
    {
    case '^':
        point->y++;
        break;
    case 'v':
        point->y--;
        break;
    case '>':
        point->x++;
        break;
    case '<':
        point->x--;
        break;
    }
}

void visit(HashTable *visited, Point2D *visitor)
{
    char key[10];
    sprintf(key, "%d,%d", visitor->x, visitor->y);
    if(hashtable_get(visited, key) == NULL)
    {
        hashtable_put(visited, key, &visitor, sizeof(Point2D));
    }
}

void solve_part_one(char *instructions, solution_t *solution)
{
    HashTable *visited = hashtable_create(10);
    if(visited == NULL)
    {
        return;
    }

    Point2D visitor = {.x = 0, .y = 0};
    char instruction;
    while((instruction = *instructions) != '\0')
    {
        visit(visited, &visitor);
        point2d_move(&visitor, instruction);
        instructions++;
    }

    solution_part_finalize_with_int(solution, 0, hashtable_get_size(visited), "2592");
    hashtable_destroy(visited);
}

void solve_part_two(char *instructions, solution_t *solution)
{
    HashTable *visited = hashtable_create(10);
    if(visited == NULL)
    {
        return;
    }

    Point2D visitors[2] = {
        {.x = 0, .y = 0},
        {.x = 0, .y = 0}
    };
    char instruction;
    uint32_t instruction_number = 0;
    while((instruction = *instructions) != '\0')
    {
        visit(visited, &visitors[instruction_number % 2]);
        point2d_move(&visitors[instruction_number %2], instruction);
        instructions++;
        instruction_number++;
    }

    solution_part_finalize_with_int(solution, 1, hashtable_get_size(visited), "2360");
    hashtable_destroy(visited);
}

int main(int argc, char *argv[]) {
    (void)argc;

    solution_t *solution = solution_create(2015, 3);
    char *instructions = file_read_all_text(argv[1]);
    char *temp = instructions;

    solve_part_one(instructions, solution);
    instructions = temp;
    solve_part_two(instructions, solution);

    free(instructions);
    return solution_finalize_and_destroy(solution);
}
