#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#include "aoc.h"
#include "file4c.h"
#include "string4c.h"
#include "hamiltonian.h"

void print_mask(int mask, int number_of_nodes) {
    for (int i = number_of_nodes - 1; i >= 0; i--)
    {
        printf("%d", (mask & (1 << i)) ? 1 : 0);
    }
    printf("\n");
}

size_t get_city_index(char cities[][20], size_t *number_of_cities, char *city) {
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

/**
 * Day 9: This problem is a variant of the Hamiltonian path and Hamiltonian cycle problems.
 */
int main(int argc, char *argv[]) {
    (void)argc;

    size_t number_of_cities = 0;
    char cities[100][20];
    int matrix[100][20];

    solution_t *solution = solution_create(2015, 9);
    size_t number_of_lines = 0;
    char **lines = file_read_all_lines(&number_of_lines, argv[1]);

    // Set up an adjancency matrix where each node has its own id.
    // The node ids determine the order of the rows and columns.
    // Each column will contain the distance between the nodes represented by row id (the departure node) and column id (the destination node).
    // Example: London has id 0, Dublin has id 1 and Belfast has id 2.
    //          London Dublin Belfast
    // London        0    464     518
    // Dublin      464      0     141
    // Belfast     518    141       0
    for(size_t i = 0; i < number_of_lines; i++)
    {
        size_t number_of_tokens = 0;
        char **tokens = string_split(&number_of_tokens, lines[i], " ");

        size_t city_index_1 = get_city_index(cities, &number_of_cities, tokens[0]);
        size_t city_index_2 = get_city_index(cities, &number_of_cities, tokens[2]);
        int distance = atoi(tokens[4]);

        matrix[city_index_1][city_index_2] = distance;
        matrix[city_index_2][city_index_1] = distance;

        free(lines[i]);
        for(size_t i = 0; i < number_of_tokens; i++) {
            free(tokens[i]);
        }
        free(tokens);
    }

    solution_part_finalize_with_int(solution, 0, hamiltonian_compute(matrix, number_of_cities, HP_NONE), "251");
    solution_part_finalize_with_int(solution, 1, hamiltonian_compute(matrix, number_of_cities, HP_FIND_MAXIMUM_COST), "898");

    free(lines);
    return solution_finalize_and_destroy(solution);
}
