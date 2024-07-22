#ifndef POINT
#define POINT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    long x;
    long y;
} Point;

char *point_to_string(Point *point);
#endif