#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "aoc.h"
#include "file4c.h"
#include "json/parser.h"
#include "test4c.h"

bool is_red(json_object_entry_t *entry) {
    if(strcmp(entry->key, "red") == 0 || (entry->value->type == JSON_NODE_TYPE_STRING && strcmp((char*)entry->value, "red") == 0)) {
        return true;
    }
    return false;
}

int traverse(json_node_t *source, int *sum, bool (*skip)(json_object_entry_t *entry)) {
    json_array_t *array;
    json_object_t *object;
    size_t size;

    switch(source->type) {
        case JSON_NODE_TYPE_ARRAY:
            array = (json_array_t*)source->value;
            size = array->size;

            for(size_t i = 0; i < size; i++) {
                json_node_t *element = array->nodes[i];
                traverse(element, sum, skip);
            }
        break;
        case JSON_NODE_TYPE_OBJECT:
            object = (json_object_t*)source->value;
            size = object->size;

            for(size_t i = 0; i < size; i++) {
                json_object_entry_t *entry = object->entries[i];
                if(skip != NULL && skip(entry)) {
                    return 0;
                }
            }

            for(size_t i = 0; i < size; i++) {
                json_object_entry_t *entry = object->entries[i];
                traverse(entry->value, sum, skip);
            }
        break;
        case JSON_NODE_TYPE_NUMBER:
            *sum += *(int*)source->value;
        break;
        default:
        break;
    }

    return *sum;
}

int main(int argc, char* argv[]) {
    (void)argc;
    Solution solution;
    char *file_content;
    int sum = 0;

    solution_create(&solution, 2015, 12);
    file_read_all_text(&file_content, argv[1]);
    json_node_t *node = json_parse_string(file_content);

    sum = traverse(node, &sum, NULL);
    solution_part_finalize_with_int(&solution, 0, sum, "191164");

    sum = 0;
    sum = traverse(node, &sum, &is_red);
    solution_part_finalize_with_int(&solution, 1, sum, "87842");

    return solution_finalize(&solution);
}
