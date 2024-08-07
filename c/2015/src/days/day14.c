#include <stdio.h>

#include "aoc.h"
#include "file4c.h"
#include "string4c.h"

typedef struct Reindeer {
    char *name;
    int velocity_in_kps;
    int stamina_in_seconds;
    int rest_time_in_seconds;

    // Race status
    int remaining_stamina;
    int remaining_rest_time;
    int travelled_distance;
    int points;
} Reindeer;

int compare_by_travelled_distance_desc(const void *a, const void *b) {
    Reindeer reindeer_a = *((Reindeer*)a);
    Reindeer reindeer_b = *((Reindeer*)b);

    if(reindeer_a.travelled_distance == reindeer_b.travelled_distance) {
        return 0;
    }

    if(reindeer_a.travelled_distance < reindeer_b.travelled_distance) {
        return 1;
    }

    return -1;
}

int compare_by_points_desc(const void *a, const void *b) {
    Reindeer reindeer_a = *((Reindeer*)a);
    Reindeer reindeer_b = *((Reindeer*)b);

    if(reindeer_a.points == reindeer_b.points) {
        return 0;
    }

    if(reindeer_a.points < reindeer_b.points) {
        return 1;
    }

    return -1;
}

int main(int argc, char *argv[]) {
    (void)argc;
    Solution solution;
    char **lines;
    size_t number_of_lines = 0;

    solution_create(&solution, 2015, 14);
    file_read_all_lines(&lines, &number_of_lines, argv[1]);

    Reindeer reindeer[number_of_lines];
    for(size_t i = 0; i < number_of_lines; i++) {
        char *line = lines[i];
        char **tokens;
        size_t number_of_tokens = 0;

        string_split(&tokens, &number_of_tokens, line, " ");

        reindeer[i] = (Reindeer){
            .name = tokens[0],
            .velocity_in_kps = atoi(tokens[3]),
            .stamina_in_seconds = atoi(tokens[6]),
            .rest_time_in_seconds = atoi(tokens[13]),
            
            // Race status
            .remaining_stamina = atoi(tokens[6]),
            .remaining_rest_time = 0,
            .travelled_distance = 0,
            .points = 0
        };
    }

    // Loop for 1000 "seconds".
    for(int i = 0; i < 2503; i++) {
        for(size_t j = 0; j < number_of_lines; j++) {
            if(reindeer[j].remaining_stamina > 0) {
                reindeer[j].travelled_distance += reindeer[j].velocity_in_kps;
                reindeer[j].remaining_stamina--;
                continue;
            }

            if(reindeer[j].remaining_rest_time == 0) {
                reindeer[j].remaining_rest_time = reindeer[j].rest_time_in_seconds;
            }

            reindeer[j].remaining_rest_time--;
            if(reindeer[j].remaining_rest_time == 0) {
                reindeer[j].remaining_stamina = reindeer[j].stamina_in_seconds;
            }
        }
        qsort(reindeer, number_of_lines, sizeof(Reindeer), compare_by_travelled_distance_desc);
        int top_distance = reindeer[0].travelled_distance;
        for(size_t j = 0; j < number_of_lines; j++) {
            if(reindeer[j].travelled_distance == top_distance) {
                reindeer[j].points++;
            }
            else {
                break;
            }
        }
    }

    qsort(reindeer, number_of_lines, sizeof(Reindeer), compare_by_travelled_distance_desc);
    solution_part_finalize_with_int(&solution, 0, reindeer[0].travelled_distance, "2640");

    qsort(reindeer, number_of_lines, sizeof(Reindeer), compare_by_points_desc);
    solution_part_finalize_with_int(&solution, 1, reindeer[0].points, "1102");

    free(lines);

    return solution_finalize(&solution);
}
