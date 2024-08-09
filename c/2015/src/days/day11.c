#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc.h"
#include "file4c.h"
#include "string4c.h"
#include "test4c.h"
#include "testing/assertions.h"

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
    size_t len                  = strlen(password);
    char illegal_characters[4]  = { 'i', 'l', 'o', '\0' };
    bool is_secure              = false;
    char *new_password          = malloc(len + 1);
    strcpy(new_password, password);
    new_password[len] = '\0';

    while(!is_secure) {
        char *temp = increment_password(new_password);
        new_password[0] = '\0';
        strcpy(new_password, temp);
        new_password[len] = '\0';
        free(temp);
        //free(password);
        is_secure = string_has_straight(new_password) && !string_has_any_needle(new_password, illegal_characters, 3) && has_two_different_non_overlapping_pairs(new_password);
    }
    return new_password;
}

void test_has_two_different_non_overlapping_pairs(char *password, bool expected_result) {
    test_string_boolean(__func__, password, expected_result, &has_two_different_non_overlapping_pairs);
}

void test_create_secure_password(char *password, char *expected_result) {
    // act, assign
    char *result = create_secure_password(password);

    // assert
    assert_string_equality(expected_result, result, "create_secure_password(\"%s\") != %s (%s)\n", password, expected_result, result);
    printf("%s(\"%s\", \"%s\") passed\n", __func__, password, expected_result);
    free(result);
}

int main(int argc, char* argv[]) {
    (void)argc;

    test_has_two_different_non_overlapping_pairs("abbceffg", true);
    test_has_two_different_non_overlapping_pairs("abbcegjk", false);
    test_has_two_different_non_overlapping_pairs("abcdffaa", true);
    test_has_two_different_non_overlapping_pairs("ghjaabcc", true);
    test_create_secure_password("abcdefgh", "abcdffaa");

    solution_t *solution = solution_create(2015, 11);
    char *file_content = file_read_all_text(argv[1]);

    char *part_one = create_secure_password(file_content);
    solution_part_finalize_with_str(solution, 0, part_one, "hepxxyzz");
    
    char *part_two = create_secure_password(part_one);
    free(part_one);
    solution_part_finalize_with_str(solution, 1, part_two, "heqaabcc");
    free(part_two);

    free(file_content);
    return solution_finalize_and_destroy(solution);
}
