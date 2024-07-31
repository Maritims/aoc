#ifndef MSET
#define MSET

#include <stdint.h>
#include <stdlib.h>

typedef struct HashTable HashTable;
typedef struct HashTableEntry HashTableEntry;
typedef struct HashTableIterator HashTableIterator; 

/**
 * hashtable_get_keys: Get all keys in the hashtable.
 * @param hashtable The hashtable instance.
 * @return Returns a copy of all keys in the hashtable.
 */
char **hashtable_get_keys(HashTable *hashtable);

/**
 * hashtable_get_capacity: Get the capacity of the hashtable.
 * @param hashtable The hashtable instance.
 * @return Returns the capacity of the hashtable.
 */
size_t hashtable_get_capacity(HashTable *hashtable);

/**
 * hashtable_get_size: Get the size of the hashtable.
 * @param hashtable The hashtable instance.
 * @return Returns the size of the hashtable.
 */
size_t hashtable_get_size(HashTable *hashtable);

/**
 * hashtable_create: Create a new hashtable.
 * @param initial_capacity The initial capacity of the hashtable.
 * @return Returns a pointer to the new hashtable.
 */
HashTable *hashtable_create(size_t initial_capacity);

/**
 * hashtable_destroy: Destroy the hashtable and clean up all memory allocations.
 * @param hashtable The hashtable instance.
 */
void hashtable_destroy(HashTable *hashtable);

/**
 * hashtable_hash: Hash the key.
 * key The key to hash.
 * @return Returns an FNV-1 hash of the key.
 */
uint64_t *hashtable_hash(const char *key);

/**
 * hashtable_get: Get an entry from the hashtable.
 * @param hashtable The hashtable instance.
 * @param key The key of the element to retrieve.
 * @return Returns a pointer to the entry.
 */
HashTableEntry *hashtable_get(const HashTable *hashtable, const char *key);

/**
 * hashtable_put: Put an entry into the hashtable.
 * @param hashtable The hashtable instance.
 * @param key The entry key.
 * @param value The entry value.
 * @param value_size Size of the entry value.
 * @return Returns a pointer to the entry.
 */
HashTableEntry *hashtable_put(HashTable *hashtable, char *key, void *value, size_t value_size, uint32_t flags);

/**
 * hashtable_put_if_absent: Put an entry into the hashtable if it isn't there already.
 * @param hashtable The hashtable instance.
 * @param key The entry key.
 * @param value The entry value.
 * @param value_size Size of the entry value.
 * @return Returns a pointer to the entry.
 */
HashTableEntry *hashtable_put_if_absent(HashTable *hashtable, char *key, void *value, size_t value_size, uint32_t flags);

HashTableIterator *hashtable_create_iterator(HashTable *hashtable);

void hashtable_iterator_destroy(HashTableIterator *iterator);

HashTableEntry *hashtable_iterator_next(HashTableIterator *hashtable_iterator);

HashTableEntry *hashtable_entry_create(char *key, void *value);

char *hashtable_entry_get_key(HashTableEntry *key);

void *hashtable_entry_get_value(HashTableEntry *entry);

#endif
