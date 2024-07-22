#include <stdio.h>
#include <stdlib.h>

#include "point.h"

char *point_to_string(Point *point) {
    char *output = calloc(1000, sizeof(char));
    sprintf(output, "{x=%ld,y=%ld}", point->x, point->y);
    return output;
}