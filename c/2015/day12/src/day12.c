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
    solution_part_finalize_with_int(&solution, 0, sum, "191164");

    sum = 0;
    sum = traverse(gv, &sum, &is_red);
    solution_part_finalize_with_int(&solution, 1, sum, "87842");

    free(gv);

    return solution_finalize(&solution);
}
