#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "point.h"

void test_point_to_string(char *display_name, Point *point, char *expected) {
    char *result = point_to_string(point);
    printf("%s: %s (expected: %s, actual: %s)", display_name, strcmp(expected, result) == 0 ? "OK" : "Not OK", expected, result);
}

int main(void) {
    Point point = (Point){1, 1};
    char *expected = "{x=1, y=1}";
    test_point_to_string("Test: Strings should be equal", &point, expected);
    return 0;
}