#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "point.h"

void test_point_to_string(Point point, char *expected) {
    char result[1024];
    point_to_string(result, point);

    if(strcmp(result, expected) == 0) {
        printf("%s passed\n", __func__);
        return;
    }

    printf("%s failed: expected the result to be \"%s\" but it was \"%s\"", __func__, expected, result);
    exit(EXIT_FAILURE);
}

int main(void) {
    test_point_to_string((Point){.x = 1, .y = 1}, "{x=1,y=1}");
    printf("All tests passed\n");
    return EXIT_SUCCESS;
}
