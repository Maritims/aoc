#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "aoc.h"
#include "generics4c.h"
#include "get_file.h"
#include "string4c.h"

typedef struct SeatingArrangement {
    char *name_one;
    char *name_two;
    int happiness;
} Person;

int main(int argc, char *argv[]) {
    Solution *solution = solution_create(2015, 13);
    size_t number_of_lines = 0;
    char **lines = get_file_as_lines(argv[1], &number_of_lines);
    GenericObject *happiness_map = generic_create_object(10);

    for(size_t i = 0; i < number_of_lines; i++) {
        char *line = lines[i];
        size_t number_of_tokens = 0;
        char **tokens = string_split(line, " ", &number_of_tokens);
        
        char *name = tokens[0];
        int num = atoi(tokens[3]);
        if(strcmp(tokens[2], "lose") == 0) {
            num *= -1;
        }
        char *neighbour = string_substring(tokens[10], 0, strlen(tokens[10]) - 1);

        GenericValue *happiness = generic_create_int(num);
        KeyValuePair *kvp = generic_get_pair_from_object(happiness_map, name);
        GenericObject *map = NULL;
        if(kvp == NULL) {
            map = generic_create_object(10);
            GenericValue *gv = generic_create_object_value(map);
            generic_add_to_object(happiness_map, name, gv);
        } else {
            map = kvp->value->data.object_value;
        }
        generic_add_to_object(map, neighbour, happiness);

        free(neighbour);
        free(tokens);
    }

    GenericValue *gv = generic_create_object_value(happiness_map);
    char *gv_string = generic_value_to_string(gv);
    printf("%s\n", gv_string);
    free(gv_string);

    solution_finalize(solution);
    solution_print(solution);
    free(solution);

    return 0;
}
