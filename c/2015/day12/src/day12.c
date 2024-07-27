#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "aoc.h"
#include "file4c.h"
#include "json4c.h"
#include "test4c.h"

bool is_red(KeyValuePair *kvp) {
    if(strcmp(kvp->key, "red") == 0 || (kvp->value->type == TYPE_STRING && strcmp(kvp->value->data.string_value, "red") == 0)) {
        return true;
    }
    return false;
}

int traverse(GenericValue *source, int *sum, bool (*skip)(KeyValuePair *kvp)) {
    switch(source->type) {
        case TYPE_ARRAY:
            for(size_t i = 0; i < source->data.array_value->size; i++) {
                GenericValue *element = source->data.array_value->elements[i];
                traverse(element, sum, skip);
            }
        break;
        case TYPE_OBJECT:
            for(size_t i = 0; i < source->data.object_value->size; i++) {
                KeyValuePair *kvp = source->data.object_value->pairs[i];
                if(skip != NULL && skip(kvp)) {
                    return 0;
                }
            }

            for(size_t i = 0; i < source->data.object_value->size; i++) {
                KeyValuePair *kvp = source->data.object_value->pairs[i];
                traverse(kvp->value, sum, skip);
            }
        break;
        case TYPE_INT:
            *sum += source->data.int_value;
        break;
        default:
        break;
    }

    return *sum;
}

int main(int argc, char* argv[]) {
    Solution solution;
    char *file_content;
    GenericValue *gv;
    int sum = 0;

    solution_create(&solution, 2015, 12);
    file_read_all_text(&file_content, argv[1]);
    json_parse_string(&gv, file_content);

    sum = traverse(gv, &sum, NULL);
    sprintf(solution.part_one.result, "%d", sum);
    solution_part_finalize(&solution.part_one, "191164");

    sum = 0;
    sum = traverse(gv, &sum, &is_red);
    sprintf(solution.part_two.result, "%d", sum);
    solution_part_finalize(&solution.part_two, "87842");

    solution_finalize(&solution);
    solution_print(&solution);

    return 0;
}
