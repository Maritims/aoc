#include <stdio.h>
#include <string.h>
#include "testing/assertions.h"
#include "hashtable.h"
#include "string4c.h"

void test_hashtable_put(char **keys, char **values, size_t number_of_entries) {
    HashTable *hashtable = hashtable_create(10);
    assert_not_null(hashtable, "%s\n", "hashtable was null");

    for(size_t i = 0; i < number_of_entries; i++) {
        assert_not_null(keys[i], "no value at index %zu in keys array\n", i);
        assert_not_null(values[i], "no value at index %zu in values array\n", i);
        
        HashTableEntry *entry = hashtable_put(hashtable, keys[i], values[i], strlen(values[i]));
        assert_not_null(entry, "failed to insert entry with key %s and value %s in hashtable", keys[i], values[i]);

        char *entry_key = hashtable_entry_get_key(entry);
        assert_string_equality(keys[i], entry_key, "inserted key %s does not match original key %s at index %zu in the keys array\n", entry_key, keys[i], i);

        char *entry_value = hashtable_entry_get_value(entry);
        assert_string_equality(values[i], entry_value, "inserted value %s does not match original value %s at index %zu in the values array\n", entry_value, values[i], i);
    }
   
    hashtable_destroy(hashtable);
    printf("%s passed\n", __func__);
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
    test_hashtable_put(
        (char *[]){"hello","world","lorem","ipsum","foo","bar","baz"},
        (char *[]){"baz","bar","foo","ipsum","lorem","world","hello"},
        7
    );
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
