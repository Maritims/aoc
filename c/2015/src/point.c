#include <stdio.h>

#include "point.h"

void point_to_string(char *result, Point point) {
    sprintf(result, "{x=%ld,y=%ld}", point.x, point.y);
}
