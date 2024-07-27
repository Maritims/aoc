#include <stdio.h>
#include "hashtable.h"

void test_hashtable_create() {
    HashTable *hashtable = hashtable_create(10);
    if(hashtable == NULL) {
        printf("%s failed: hashtable was NULL\n", __func__);
        exit(EXIT_FAILURE);
    }

    printf("%s passed\n", __func__);
}

void test_hashtable_put_and_hashtable_get_size() {
    HashTable *hashtable = hashtable_create(10);
    for(size_t i = 0; i < 10; i++) {
        char key[5];
        sprintf(key, "%s%zu", "foo", i);
        char value[4] = { 'b', 'a', 'r', '\0' };
        HashTableEntry *entry = hashtable_entry_create(key, value);
        hashtable_put(hashtable, key, value, sizeof(value), 0);
    }
   
    if(hashtable->size != 10) {
        printf("%s failed: expected hashtable->size to be 10, but it was %zu\n", __func__, hashtable->size);
        exit(EXIT_FAILURE);
    }

    printf("%s passed\n", __func__);
}

int main(void) {
    test_hashtable_create();
    test_hashtable_put_and_hashtable_get_size();
    printf("All tests passed\n");
}
