#ifndef MSET
#define MSET

#include <stdint.h>
#include <stdlib.h>

typedef struct HashTableEntry
{
    char *key;
    uint64_t key_hash;
    void *value;
    struct HashTableEntry *next;
} HashTableEntry;

typedef struct
{
    size_t capacity;
    size_t size;
    HashTableEntry **entries;
} HashTable;

typedef struct 
{
    size_t current_index;
    HashTable *hashtable;
    HashTableEntry *current_entry;
} HashTableIterator;

typedef enum HASHTABLE_RESULT
{
    SUCCESS,
    FAILURE,
    UNABLE_TO_ALLOCATE_HASHTABLE,
    UNABLE_TO_ALLOCATE_ENTRIES,
    KEY_IS_NULL,
    KEY_IS_EMPTY,
    VALUE_POINTER_IS_ZERO
} HASHTABLE_RESULT;

void hashtable_print_result(HASHTABLE_RESULT hashtable_result);

HashTable *hashtable_create(uint32_t initial_capacity);

HASHTABLE_RESULT hashtable_destroy(HashTable *hashtable);

uint64_t *hashtable_hash(const char *key);

HashTableEntry *hashtable_get(const HashTable *hashtable, const char *key);

HashTableEntry *hashtable_put(HashTable *hashtable, char *key, void *value, size_t value_size, uint32_t flags);

HashTableEntry *hashtable_put_if_absent(HashTable *hashtable, char *key, void *value, size_t value_size, uint32_t flags);

HASHTABLE_RESULT hashtable_create_iterator(HashTable *hashtable, HashTableIterator **out_hashtable_iterator);

HashTableEntry *hashtable_iterator_next(HashTableIterator *hashtable_iterator);

HashTableEntry *hashtable_entry_create(char *key, void *value);
#endif