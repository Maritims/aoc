#ifndef POINT
#define POINT


typedef struct
{
    long x;
    long y;
} Point;

void point_to_string(char *result, Point point);
#endif
