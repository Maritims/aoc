#ifndef POINT_H
#define POINT_H

#include <stdint.h>

typedef struct
{
    uint64_t x;
    uint64_t y;
} Point;

void point_to_string(char *result, Point point);

#endif
