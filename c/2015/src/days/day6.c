#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc.h"
#include "file4c.h"
#include "grid.h"
#include "point.h"
#include "test4c.h"

typedef struct
{
    char operation[10];
    Point starting_point;
    Point stopping_point;
} Action;

typedef struct
{
    uint32_t is_dimmable;
    uint32_t brightness;
} Light;

typedef GRID_OF_TYPE(Light) LightGrid;

CREATE_GRID_IMPL_FOR(LightGrid, Light)

int action_parse(const char *line, Action *action) {
    if (strncmp(line, "turn", 4) == 0) {
        line += 5;
    }

    return sscanf(line, "%s %ld,%ld through %ld,%ld", action->operation, &action->starting_point.x, &action->starting_point.y, &action->stopping_point.x, &action->stopping_point.y) == 5 ? 0 : -1;
}

int action_compare(Action *a, Action *b)
{
    int operation_comparison = strcmp(a->operation, b->operation);
    if (operation_comparison != 0)
    {
        fprintf(stderr, "%s:%d: %s does not equal %s\n", __FILE__, __LINE__, a->operation, b->operation);
        return operation_comparison;
    }

    // Compare the starting points
    if (a->starting_point.x != b->starting_point.x)
    {
        fprintf(stderr, "%s:%d: %ld does not equal %ld\n", __FILE__, __LINE__, a->starting_point.x, b->starting_point.x);
        return (a->starting_point.x > b->starting_point.x) ? 1 : -1;
    }
    if (a->starting_point.y != b->starting_point.y)
    {
        fprintf(stderr, "%s:%d: %ld does not equal %ld\n", __FILE__, __LINE__, a->starting_point.y, b->starting_point.y);
        return (a->starting_point.y > b->starting_point.y) ? 1 : -1;
    }

    // Compare the stopping points
    if (a->stopping_point.x != b->stopping_point.x)
    {
        fprintf(stderr, "%s:%d: %ld does not equal %ld\n", __FILE__, __LINE__, a->starting_point.x, b->starting_point.x);
        return (a->stopping_point.x > b->stopping_point.x) ? 1 : -1;
    }
    if (a->stopping_point.y != b->stopping_point.y)
    {
        fprintf(stderr, "%s:%d: %ld does not equal %ld\n", __FILE__, __LINE__, a->stopping_point.y, b->stopping_point.y);
        return (a->stopping_point.y > b->stopping_point.y) ? 1 : -1;
    }

    // All fields are equal
    return 0;
}

void action_to_string(char *result, Action action)
{
    char starting_point_str[1024];
    char stopping_point_str[1024];

    point_to_string(starting_point_str, action.starting_point);
    point_to_string(stopping_point_str, action.stopping_point);

    sprintf(result, "{operation=%s, ", action.operation);
    sprintf(result + strlen(result), "starting_point=%s, ", starting_point_str);
    sprintf(result + strlen(result), "stopping_point=%s}", stopping_point_str);
}

Action **actions_parse(char **lines, size_t length)
{
    Action **actions = calloc(length, sizeof(Action *));
    size_t number_of_actions = 0;
    for (size_t i = 0; i < length; i++)
    {
        actions[i] = malloc(sizeof(Action));
        int action_parse_result = action_parse(lines[i], actions[i]);
        if (action_parse_result == 0)
        {
            number_of_actions++;
        }
        else
        {
            fprintf(stderr, "Unable to parse action on line %zu: %d\n", i, action_parse_result);
        }
    }

    if (number_of_actions != length)
    {
        fprintf(stderr, "Parsed %zu out of %zu lines. Unable to continue.\n", number_of_actions, length);
        return NULL;
    }
    return actions;
}

void light_adjust_brightness(Light *light, Action *action)
{
    if (light->is_dimmable == 1)
    {
        if (strcmp(action->operation, "on") == 0)
        {
            light->brightness++;
        }
        else if (strcmp(action->operation, "off") == 0 && light->brightness > 0)
        {
            light->brightness--;
        }
        else if (strcmp(action->operation, "toggle") == 0)
        {
            light->brightness += 2;
        }
    }
    else
    {
        if (strcmp(action->operation, "on") == 0)
        {
            light->brightness = 1;
        }
        else if (strcmp(action->operation, "off") == 0)
        {
            light->brightness = 0;
        }
        else if (strcmp(action->operation, "toggle") == 0)
        {
            light->brightness = light->brightness == 0 ? 1 : 0;
        }
    }
}

void light_grid_adjust_brightness(Action **actions, size_t number_of_action, uint32_t is_dimmable, size_t *out_result)
{
    LightGrid *grid = grid_create_LightGrid(1000, 1000, (Light){is_dimmable, 0});
    
    for (uint32_t i = 0; i < number_of_action; i++) {
        Action *action = actions[i];
        for (uint64_t y = action->starting_point.y; y <= action->stopping_point.y; y++) {
            for (uint64_t x = action->starting_point.x; x <= action->stopping_point.x; x++)
            {
                light_adjust_brightness(&grid->table[y][x], action);
            }
        }
    }

    for (uint64_t y = 0; y < grid->rows; y++)
    {
        for (uint64_t x = 0; x < grid->columns; x++)
        {
            (*out_result) += grid->table[y][x].brightness;
        }
        free(grid->table[y]);
    }

    free(grid->table);
    free(grid);
}

void solve_part_one(char **lines, size_t length, solution_t *solution)
{
    Action **actions = actions_parse(lines, length);   
    size_t enabled_lights = 0;
    light_grid_adjust_brightness(actions, length, 0, &enabled_lights);
    solution_part_finalize_with_int(solution, 0, enabled_lights, "377891");
    for(size_t i = 0; i < length; i++) {
        free(actions[i]);
    }
    free(actions);
}

void solve_part_two(char **lines, size_t length, solution_t *solution)
{
    Action **actions = actions_parse(lines, length);
    size_t total_brightness = 0;
    light_grid_adjust_brightness(actions, length, 1, &total_brightness);
    solution_part_finalize_with_int(solution, 1, total_brightness, "14110788");
    for(size_t i = 0; i < length; i++) {
        free(actions[i]);
    }
    free(actions);
}

void test_light_grid_adjust_brightness(TestResults *test_results, Action action, uint32_t is_dimmable, uint64_t expectation)
{
    size_t result       = 0;
    Action **actions    = calloc(1, sizeof(Action *));
    actions[0]          = &action;
    light_grid_adjust_brightness(actions, 1, is_dimmable, &result);
    int success         = result == expectation ? 1 : 0;
    test_results->total++;
    test_results->succeeded += success;

    char action_str[1024];
    action_to_string(action_str, action);

    printf("%s(%s, %d, %lu): %s\n", __func__, action_str, is_dimmable, expectation, success ? "OK" : "Not OK");
    free(actions);
}

void test_action_parse(TestResults *test_results, char *line, Action expectation)
{
    Action action;
    if (action_parse(line, &action) != 0)
    {
        printf("Unable to parse action from line \"%s\"\n", line);
        return;
    }
    int success = action_compare(&action, &expectation) == 0 ? 1 : 0;
    test_results->total++;
    test_results->succeeded += success;

    char action_str[1024];
    action_to_string(action_str, action);

    printf("%s(%s, %s): %s\n", __func__, line, action_str, success ? "OK" : "Not OK");
}

void test_action_compare(TestResults *test_results, Action *a, Action *b, int expectation)
{
    int success = action_compare(a, b) == expectation ? 1 : 0;
    test_results->total++;
    test_results->succeeded += success;

    char action_a_str[1024];
    char action_b_str[1024];
    action_to_string(action_a_str, *a);
    action_to_string(action_b_str, *b);

    printf("%s(%s, %s, %d): %s\n", __func__, action_a_str, action_b_str, expectation, success ? "OK" : "Not OK");
}

void test()
{
    TestResults test_results = {.total = 0, .succeeded = 0};

    // Enable all lights.
    test_light_grid_adjust_brightness(&test_results, (Action){"on", (Point){0, 0}, (Point){999, 999}}, 0, 1000000);
    // Enable the first row of lights.
    test_light_grid_adjust_brightness(&test_results, (Action){"on", (Point){0, 0}, (Point){0, 999}}, 0, 1000);
    // Enable the four middle lights.
    test_light_grid_adjust_brightness(&test_results, (Action){"on", (Point){499, 499}, (Point){500, 500}}, 0, 4);
    // Increase brightness by 1.
    test_light_grid_adjust_brightness(&test_results, (Action){"on", (Point){0, 0}, (Point){0, 0}}, 1, 1);
    // Achieve maximum brightness.
    test_light_grid_adjust_brightness(&test_results, (Action){"toggle", (Point){0, 0}, (Point){999, 999}}, 1, 2000000);
    
    // Actions are equal.
    test_action_compare(&test_results, &(Action){"on", (Point){1, 1}, (Point){2, 2}}, &(Action){"on", (Point){1, 1}, (Point){2, 2}}, 0);
    // Actions are not equal due to different operations.
    test_action_compare(&test_results, &(Action){"off", (Point){1, 1}, (Point){2, 2}}, &(Action){"on", (Point){1, 1}, (Point){2, 2}}, -8);
    // Actions are not equal due to different starting points.
    test_action_compare(&test_results, &(Action){"on", (Point){1, 2}, (Point){2, 2}}, &(Action){"on", (Point){1, 1}, (Point){2, 2}}, 1);
    // Actions are not equal due to different stopping points.
    
    test_action_compare(&test_results, &(Action){"on", (Point){1, 1}, (Point){2, 3}}, &(Action){"on", (Point){1, 1}, (Point){2, 2}}, 1);
    test_action_parse(&test_results, "turn on 944,269 through 975,453", (Action){"on", (Point){944, 269}, (Point){975, 453}});
    test_action_parse(&test_results, "turn off 963,52 through 979,502", (Action){"off", (Point){963, 52}, (Point){979, 502}});
    test_action_parse(&test_results, "toggle 276,956 through 631,964", (Action){"toggle", (Point){276, 956}, (Point){631, 964}});

    printf("%d tests were executed, %d tests succeeded, %d tests failed.\n", test_results.total, test_results.succeeded, test_results.total - test_results.succeeded);
}

int main(int argc, char *argv[]) {
    (void)argc;
    test();

    solution_t *solution = solution_create(2015, 6);
    size_t number_of_lines = 0;
    char **lines = file_read_all_lines(&number_of_lines, argv[1]);

    solve_part_one(lines, number_of_lines, solution);
    solve_part_two(lines, number_of_lines, solution);

    FREE_ARRAY(lines, number_of_lines);
    return solution_finalize_and_destroy(solution);
}
