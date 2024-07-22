#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc.h"
#include "test4c.h"

// Increment password.
// Check for straight of three increasing characters.
// Check for i, l and o.
// Check for at least two different, non-overlapping pairs of letters.

char *increment_password(char *password) {
    if(password == NULL) {
        fprintf(stderr, "Parameter \"password\" cannot be NULL\n");
        return NULL;
    }

    int length = strlen(password);
    if(length == 0) {
        fprintf(stderr, "Parameter \"password\" cannot be empty\n");
        return NULL;
    }

    char *new_password = strdup(password);
    if(new_password == NULL) {
        fprintf(stderr, "Unable to duplicate password\n");
        return NULL;
    }

    for(int i = length - 1; i >= 0; i--) {
        if(new_password[i] == 'z') {
            new_password[i] -= 25;
        } else {
            new_password[i] += 1;
            break;
        }
    }
    return new_password;
}

bool has_straight_of_three(char *password) {
    int length = strlen(password);
    for(int i = 0; i < length - 2; i++) {
        if(password[i] == password[i + 1] - 1 && password[i] == password[i + 2] - 2) {
            return true;
        }
    }
    return false;
}

bool has_illegal_character(char *password) {
    return strchr(password, 'i') || strchr(password, 'l') || strchr(password, 'o');
}

bool has_two_different_non_overlapping_pairs(char *password) {
    int length = strlen(password);
    for(int i = 0; i < length - 1; i++) {
        if(password[i] == password[i + 1]) {
            for(int j = i + 2; j < length - 1; j++) {
                if(password[j] == password[j + 1]) {
                    return true;
                }
            }
        }
    }
    return false;
}

char *create_secure_password(char *password) {
    bool is_secure = false;
    while(!is_secure) {
        char *new_password = increment_password(password);
        //free(password);
        password = new_password;
        is_secure = has_straight_of_three(password) && !has_illegal_character(password) && has_two_different_non_overlapping_pairs(password);
    }
    return password;
}

void test_has_straight_of_three(char *password, bool expected_result) {
    test_string_boolean(__func__, password, expected_result, &has_straight_of_three);
}

void test_has_illegal_character(char *password, bool expected_result) {
    test_string_boolean(__func__, password, expected_result, &has_illegal_character);
}

void test_has_two_different_non_overlapping_pairs(char *password, bool expected_result) {
    test_string_boolean(__func__, password, expected_result, &has_two_different_non_overlapping_pairs);
}

void test_create_secure_password(char *password, char *expected_result) {
    test_string_string(__func__, password, expected_result, &create_secure_password);
}

int main(int argc, char* argv[]) {
    test_has_straight_of_three("hijklmmn", true);
    test_has_straight_of_three("abbceffg", false);
    test_has_straight_of_three("abcdffaa", true);
    test_has_straight_of_three("ghjaabcc", true);
    test_has_illegal_character("hijklmmn", true);
    test_has_two_different_non_overlapping_pairs("abbceffg", true);
    test_has_two_different_non_overlapping_pairs("abbcegjk", false);
    test_has_two_different_non_overlapping_pairs("abcdffaa", true);
    test_has_two_different_non_overlapping_pairs("ghjaabcc", true);
    test_create_secure_password("abcdefgh", "abcdffaa");

    Solution *solution = solution_create(2015, 11);
    char *password = get_file(argv[1]);

    password = create_secure_password(password);
    sprintf(solution->part_one.result, password);
    solution_part_finalize(&solution->part_one);
    
    password = create_secure_password(password);
    sprintf(solution->part_two.result, password);
    solution_part_finalize(&solution->part_two);

    solution_finalize(solution);
    solution_print(solution);

    free(password);
    free(solution);
    return 0;
}