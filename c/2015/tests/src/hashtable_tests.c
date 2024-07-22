#include <stdio.h>
#include "hashtable.h"

int main(void) {
    HashTable *hashtable = hashtable_create(10);
    for(uint32_t i = 0; i < 10; i++) {
        char *key = calloc(4, sizeof(char));
        sprintf(key, "%s%d", "foo", i);
        char *value = "bar";
        HashTableEntry *entry = hashtable_entry_create(key, value);
        hashtable_put(hashtable, key, value);
    }
   
    size_t hashtable_size = hashtable_get_size(hashtable);
    printf("hashtable_create: %s. The expected result was %zu and the actual result was %zu\n", hashtable_size == 10 ? "OK" : "Not OK", 10, hashtable_size);
}
