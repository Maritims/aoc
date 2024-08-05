#include <stdbool.h>
#include <stdio.h>
#include "hashset.h"

void test_hashset_hash_collision(char *value1, char *value2) {
    uint64_t *hash1 = hashset_hash(value1, TYPE_STRING);
    uint64_t *hash2 = hashset_hash(value2, TYPE_STRING);

    if(*hash1 == *hash2) {
        printf("%s(\"%s\", \"%s\") failed. Hashes collided\n", __func__, value1, value2);
        free(hash1);
        free(hash2);
        exit(EXIT_FAILURE);
    }

    printf("%s(\"%s\", \"%s\") passed\n", __func__, value1, value2);
    free(hash1);
    free(hash2);
}

void test_hashset_contains(HashSet *hashset, char *needle, bool expected) {
    bool contains = hashset_contains(hashset, needle, TYPE_STRING);
    if(contains != expected) {
        printf("%s failed: hashset_contains(hashset, \"%s\") != %d\n", __func__, needle, expected);
        exit(EXIT_FAILURE);
    }

    printf("%s(\"%s\") passed\n", __func__, needle);
}

int main() {
    test_hashset_hash_collision("a", "A");

    HashSet *hashset = hashset_create(10);
    hashset_add(hashset, "A", TYPE_STRING);
    test_hashset_contains(hashset, "a", false);
}
