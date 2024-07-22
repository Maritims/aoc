#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fnv.h"

/*
    Upon success the FNV-1a hash of the given char array is returned, otherwise a NULL pointer is returned.
    Callers must remember to free() the return value.
*/
uint64_t *fnv1a(const char *bytes)
{
    uint64_t *hash = malloc(sizeof(uint64_t));
    if(hash == NULL)
    {
        fprintf(stderr, "Unable to allocate memory for hash\n");
        return NULL;
    }

    *hash = FNV_OFFSET_BASIS;
    size_t length = strlen(bytes);
    for (size_t i = 0; i < length; i++)
    {
        uint8_t byte_of_data = bytes[i];
        *hash ^= byte_of_data;
        *hash *= FNV_PRIME;
    }
    return hash;
}