#ifndef MSET
#define MSET

#include <stdint.h>
#include <stdlib.h>

typedef struct HashTable HashTable;
typedef struct HashTableEntry HashTableEntry;
typedef struct HashTableIterator HashTableIterator; 

HashTable *hashtable_create(size_t initial_capacity);

void hashtable_destroy(HashTable *hashtable);

size_t hashtable_get_capacity(HashTable *hashtable);

size_t hashtable_get_size(HashTable *hashtable);

char **hashtable_get_keys(HashTable *hashtable);

/**
 * hashtable_hash: Hash the key.
 * key The key to hash.
 * @return Returns an FNV-1 hash of the key.
 */
uint64_t *hashtable_hash(const char *key);

HashTableEntry *hashtable_get(const HashTable *hashtable, const char *key);

HashTableEntry *hashtable_put(HashTable *hashtable, char *key, void *value, size_t length);

HashTableEntry *hashtable_put_if_absent(HashTable *hashtable, char *key, void *value, size_t length);

HashTableIterator *hashtable_create_iterator(HashTable *hashtable);

void hashtable_iterator_destroy(HashTableIterator *iterator);

HashTableEntry *hashtable_iterator_next(HashTableIterator *hashtable_iterator);

HashTableEntry *hashtable_entry_create(char *key, void *value);

char *hashtable_entry_get_key(HashTableEntry *key);

void *hashtable_entry_get_value(HashTableEntry *entry);

#endif
