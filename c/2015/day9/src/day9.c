#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#include "aoc.h"
#include "hashset.h"
#include "math4c.h"
#include "string4c.h"
#include "hamiltonian_path.h"

void print_mask(int mask, int number_of_nodes)
{
    for (int i = number_of_nodes - 1; i >= 0; i--)
    {
        printf("%d", (mask & (1 << i)) ? 1 : 0);
    }
    printf("\n");
}

size_t get_city_index(char cities[][20], size_t *number_of_cities, char *city)
{
    for(size_t i = 0; i < *number_of_cities; i++)
    {
        if(strcmp(cities[i], city) == 0)
        {
            return i;
        }
    }

    strcpy(cities[*number_of_cities], city);
    (*number_of_cities)++;
    return *number_of_cities - 1;
}

int main(int argc, char *argv[])
{
    Solution *solution = solution_create(2015, 9);
    size_t length = 0;
    size_t number_of_cities = 0;
    char **lines = get_file_as_lines(argv[1], &length);
    char cities[100][20];
    int matrix[100][20];

    // Set up an adjancency matrix where each node has its own id.
    // The node ids determine the order of the rows and columns.
    // Each column will contain the distance between the nodes represented by row id (the departure node) and column id (the destination node).
    // Example: London has id 0, Dublin has id 1 and Belfast has id 2.
    //          London Dublin Belfast
    // London        0    464     518
    // Dublin      464      0     141
    // Belfast     518    141       0
    for(size_t i = 0; i < length; i++)
    {
        char *line = lines[i];
        size_t number_of_elements = 0;
        char **elements = string_split(line, " ", &number_of_elements);
        size_t city_index_1 = get_city_index(cities, &number_of_cities, elements[0]);
        size_t city_index_2 = get_city_index(cities, &number_of_cities, elements[2]);
        int distance = atoi(elements[4]);

        matrix[city_index_1][city_index_2] = distance;
        matrix[city_index_2][city_index_1] = distance;
    }

    sprintf(solution->part_one.result, "%d", hamiltonian_path(matrix, number_of_cities, HP_NONE));
    solution_part_finalize(&solution->part_one);
    sprintf(solution->part_two.result, "%d", hamiltonian_path(matrix, number_of_cities, HP_FIND_MAXIMUM_COST));
    solution_part_finalize(&solution->part_two);
    solution_finalize(solution);
    solution_print(solution);

    return 0;
}