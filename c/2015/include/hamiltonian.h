#ifndef HAMILTONIAN
#define HAMILTONIAN

typedef enum
{
    HP_NONE = 1 << 0,
    HP_DEBUG = 1 << 1,
    HP_RETURN_TO_START = 1 << 2,
    HP_FIND_MAXIMUM_COST = 1 << 3
} HAMILTONIAN_PATH_FLAGS;

int hamiltonian_compute(int nodes[][20], int number_of_nodes, HAMILTONIAN_PATH_FLAGS flags);

#endif
