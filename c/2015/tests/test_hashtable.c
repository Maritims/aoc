#include <stdio.h>
#include <string.h>

#include "hashtable.h"
#include "string4c.h"

void test_hashtable_create() {
    HashTable *hashtable = hashtable_create(10);
    if(hashtable == NULL) {
        printf("%s failed: hashtable was NULL\n", __func__);
        exit(EXIT_FAILURE);
    }

    printf("%s passed\n", __func__);
}

void test_hashtable_put_and_hashtable_get_size(size_t expected) {
    HashTable *hashtable = hashtable_create(expected);
    for(size_t i = 0; i < expected; i++) {
        char key[100];
        sprintf(key, "%s%zu", "foo", i);
        char value[4] = { 'b', 'a', 'r', '\0' };
        hashtable_put(hashtable, key, value, sizeof(value));
    }
   
    size_t hashtable_size = hashtable_get_size(hashtable);
    if(hashtable_get_size(hashtable) != expected) {
        printf("%s failed: hashtable_get_size(hashtable) != 10 (%zu)\n", __func__, hashtable_size);
        exit(EXIT_FAILURE);
    }

    printf("%s(%zu) passed\n", __func__, expected);
}

void test_hashtable_get_keys(char **keys, size_t keys_length) {
    HashTable *hashtable = hashtable_create(keys_length);
    for(size_t i = 0; i < keys_length; i++) {
        hashtable_put(hashtable, keys[i], keys[i], sizeof(char*));
    }

    char **actual_keys = hashtable_get_keys(hashtable);
    qsort(actual_keys, keys_length, sizeof(char*), string_compare_asc);

    for(size_t i = 0; i < keys_length; i++) {
        if(actual_keys[i] == NULL) {
            printf("%s failed: key at index %zu is NULL\n", __func__, i);
            exit(EXIT_FAILURE);
        }

        if(strcmp(keys[i], actual_keys[i]) != 0) {
            printf("%s failed: actual_keys[%zu] (%s) != keys[%zu] (%s)\n", __func__, i, actual_keys[i], i, keys[i]);
            exit(EXIT_FAILURE);
        }
    }

    printf("%s passed\n", __func__);
}

int main(void) {
    test_hashtable_create();
    test_hashtable_put_and_hashtable_get_size(10);
    test_hashtable_get_keys((char*[]){
        "amet",
        "bar",
        "baz",
        "dolor",
        "foo",
        "hello",
        "ipsum",
        "lorem",
        "sit",
        "world"
    }, 10);
    printf("All tests passed\n");
}
