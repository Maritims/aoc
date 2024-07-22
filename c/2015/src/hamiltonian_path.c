#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hamiltonian_path.h"

#define MAX_NODES 10

static int cache[1 << MAX_NODES][MAX_NODES];

static int compute_hamiltonian_path(int mask, int position, int number_of_nodes, int nodes[][20], HAMILTONIAN_PATH_FLAGS flags)
{
    // Has every node been visited?
    if (mask == (1 << number_of_nodes) - 1)
    {
        // Return the distance from the current node to the first node if the HP_RETURN_TO_START flag is set.
        // If not, just return zero because we're not interested in adding the cost of going home.
        return flags & HP_RETURN_TO_START ? nodes[position][0] : 0;
    }

    if(cache[mask][position] != -1)
    {
        return cache[mask][position];
    }

    int final_answer = flags & HP_FIND_MAXIMUM_COST ? INT_MIN : INT_MAX;

    // Visit an unvisited node.
    for (int n = 0; n < number_of_nodes; n++)
    {
        // The node is visited if the nth bit in the mask variable is equal to 1.
        // Check this by AND-ing the mask with nth bit.
        if ((mask & (1 << n)) == 0)
        {
            // Get the distance from the current node to the new node, plus the remaining distance.
            // The remaining distance is given by calling the function recursively with the nth bit of the mask variable equal to 1, indicating that the nth node has been visited.
            int answer = nodes[position][n] + compute_hamiltonian_path(mask | (1 << n), n, number_of_nodes, nodes, flags);
            final_answer = flags & HP_FIND_MAXIMUM_COST ? (answer > final_answer ? answer : final_answer) : (answer < final_answer ? answer : final_answer);

            if((flags & HP_DEBUG) != 0)
            {
                printf("Going to %d from %d. Current answer: %d, final answer: %d\n", n, position, answer, final_answer);
            }
        }
    }

    return cache[mask][position] = final_answer;
}

int hamiltonian_path(int nodes[][20], int number_of_nodes, HAMILTONIAN_PATH_FLAGS flags)
{
    memset(cache, -1, sizeof(cache));

    int final_answer = flags & HP_FIND_MAXIMUM_COST ? INT_MIN : INT_MAX;

    for(int i = 0; i < number_of_nodes; i++)
    {
        int answer = compute_hamiltonian_path(1 << i, i, number_of_nodes, nodes, flags);
        final_answer = flags & HP_FIND_MAXIMUM_COST ? (answer > final_answer ? answer : final_answer) : (answer < final_answer ? answer : final_answer);
    }

    memset(cache, -1, sizeof(cache));

    return final_answer;
}