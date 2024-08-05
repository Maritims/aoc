#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "aoc.h"
#include "file4c.h"
#include "generics/array.h"
#include "generics/object.h"
#include "generics/value.h"
#include "json/parser.h"
#include "test4c.h"

bool is_red(generic_object_key_value_pair_t *kvp) {
    char *kvp_key                       = generic_object_kvp_get_key(kvp);
    generic_value_t *kvp_value          = generic_object_kvp_get_value(kvp);
    generic_value_type_t kvp_value_type = generic_value_get_type(kvp_value);
    char *kvp_string_value              = generic_value_get_string(kvp_value);

    if(strcmp(kvp_key, "red") == 0 || (kvp_value_type == GENERIC_VALUE_TYPE_STRING && strcmp(kvp_string_value, "red") == 0)) {
        return true;
    }
    return false;
}

int traverse(generic_value_t *source, int *sum, bool (*skip)(generic_object_key_value_pair_t *kvp)) {
    generic_value_type_t source_type = generic_value_get_type(source);

    generic_array_t *array;
    generic_object_t *object;
    size_t size;

    switch(source_type) {
        case GENERIC_VALUE_TYPE_ARRAY:
            array   = generic_value_get_array(source);
            size    = generic_array_get_size(array);

            for(size_t i = 0; i < size; i++) {
                generic_value_t *element = generic_array_get(array, i);
                traverse(element, sum, skip);
            }
        break;
        case GENERIC_VALUE_TYPE_OBJECT:
            object  = generic_value_get_object(source);
            size    = generic_object_get_size(object);

            for(size_t i = 0; i < size; i++) {
                generic_object_key_value_pair_t *kvp = generic_object_kvp_get(object, i);
                if(skip != NULL && skip(kvp)) {
                    return 0;
                }
            }

            for(size_t i = 0; i < size; i++) {
                generic_object_key_value_pair_t *kvp    = generic_object_kvp_get(object, i);
                generic_value_t *kvp_value              = generic_object_kvp_get_value(kvp);
                traverse(kvp_value, sum, skip);
            }
        break;
        case GENERIC_VALUE_TYPE_INT:
            *sum += generic_value_get_int(source);
        break;
        default:
        break;
    }

    return *sum;
}

int main(int argc, char* argv[]) {
    Solution solution;
    char *file_content;
    generic_value_t *gv;
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
