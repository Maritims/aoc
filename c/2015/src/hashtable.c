#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "fnv.h"
#include "hashtable.h"

#define FNV_OFFSET_BASIS 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

struct HashTable {
    size_t capacity;
    size_t size;
    HashTableEntry **entries;
};

struct HashTableEntry {
    char *key;
    uint64_t key_hash;
    void *value;
    struct HashTableEntry *next;
};

struct HashTableIterator {
    size_t current_index;
    HashTable *hashtable;
    HashTableEntry *current_entry;
};

char **hashtable_get_keys(HashTable *hashtable) {
    HashTableIterator *iterator = hashtable_create_iterator(hashtable);
    char **keys                 = calloc(hashtable->size, sizeof(char*));
    size_t i                    = 0;
    HashTableEntry *entry;

    while((entry = hashtable_iterator_next(iterator)) != NULL) {
        keys[i++] = hashtable_entry_get_key(entry);
    }

    return keys;
}

size_t hashtable_get_capacity(HashTable *hashtable) {
    return hashtable->capacity;
}

size_t hashtable_get_size(HashTable *hashtable) {
    return hashtable->size;
}

HashTable *hashtable_create(size_t initial_capacity) {
    HashTable *hashtable = malloc(sizeof(HashTable));
    if(hashtable == NULL) {
        fprintf(stderr, "%s:%d: Failed to allocate memory for hashtable\n", __func__, __LINE__);
        return NULL;
    }

    hashtable->capacity = initial_capacity;
    hashtable->size     = 0;
    hashtable->entries  = calloc(hashtable->capacity, sizeof(HashTableEntry*));

    if (hashtable->entries == NULL)
    {
        fprintf(stderr, "%s:%d: Failed to allocate memory for hashtable entries\n", __func__, __LINE__);
        free(hashtable);
        return NULL;
    }

    for (size_t i = 0; i < hashtable->capacity; i++)
    {
        hashtable->entries[i] = NULL;
    }

    return hashtable;
}

void hashtable_destroy(HashTable *hashtable)
{
    for (size_t i = 0; i < hashtable->capacity; i++)
    {
        HashTableEntry *entry = hashtable->entries[i];
        while (entry)
        {
            HashTableEntry *next = entry->next;
            free(entry->key);
            free(entry->value);
            free(entry);
            entry = next;
        }
    }

    free(hashtable->entries);
}

uint64_t *hashtable_hash(const char *key)
{
    return fnv1a(key);
}

HashTableEntry *hashtable_get(const HashTable *hashtable, const char *key) {
    uint64_t *hash = hashtable_hash(key);
    size_t index = *hash % hashtable->capacity;
    free(hash);

    HashTableEntry *entry = hashtable->entries[index];
    while (entry != NULL)
    {
        if (strcmp(entry->key, key) == 0)
        {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

static bool hashtable_rehash(HashTable *hashtable)
{
    size_t new_capacity = hashtable->capacity * 2;
    HashTableEntry **new_entries = calloc(new_capacity, sizeof(HashTableEntry*));
    if(new_entries == NULL)
    {
        fprintf(stderr, "%s:%d: Failed to allocate memory for hashtable entries\n", __func__, __LINE__);
        return false;
    }

    for(size_t i = 0; i < hashtable->capacity; i++)
    {
        HashTableEntry *entry = hashtable->entries[i];
        while(entry)
        {
            HashTableEntry *next = entry->next;
            size_t new_index = entry->key_hash % new_capacity;
            entry->next = new_entries[new_index];
            new_entries[new_index] = entry;
            entry = next;
        }
    }

    free(hashtable->entries);
    hashtable->entries = new_entries;
    hashtable->capacity = new_capacity;

    return true;
}

HashTableEntry *hashtable_put(HashTable *hashtable, char *key, void *value, size_t value_size) {
    if(value == 0) {
        fprintf(stderr, "%s(): Unable to insert element into the hashtable. The value argument cannot be 0.\n", __func__);
        return NULL;
    }

    HashTableEntry *existing_entry = hashtable_get(hashtable, key);
    if (existing_entry != NULL) {
        free(existing_entry->value);
        existing_entry->value = malloc(value_size);
        sprintf(existing_entry->value, "%s", (char*)value);
        return existing_entry;
    }
    
    if (hashtable->size >= hashtable->capacity && !hashtable_rehash(hashtable)) {
        fprintf(stderr, "%s:%d: Failed to rehash the hashtable. Aborting insertion of entry with key \"%s\"\n", __func__, __LINE__, key);
        return NULL;
    }

    uint64_t *hash = hashtable_hash(key);
    if(hash == NULL) {
        fprintf(stderr, "%s:%d: Failed to hash the key \"%s\". Aborting insertion of entry with key \"%s\"\n", __func__, __LINE__, key, key);
        return NULL;
    }

    size_t index = *hash % hashtable->capacity;
    HashTableEntry *new_entry = malloc(sizeof(HashTableEntry));
    if(new_entry == NULL) {
        fprintf(stderr, "%s(): Unable to allocate memory for new entry.\n", __func__);
        return NULL;
    }

    new_entry->key = strdup(key);
    if(new_entry->key == NULL)
    {
        fprintf(stderr, "%s(): Unable to duplicate key for new entry.\n", __func__);
        free(new_entry);
        return NULL;
    }

    new_entry->key_hash = *hash;
    new_entry->value = malloc(value_size);
    sprintf(new_entry->value, "%s", (char*)value);
    new_entry->next = hashtable->entries[index];
    hashtable->entries[index] = new_entry;
    hashtable->size++;

    free(hash);

    return new_entry;
}

HashTableEntry *hashtable_put_if_absent(HashTable *hashtable, char *key, void *value, size_t value_size)
{
    HashTableEntry *entry = hashtable_get(hashtable, key);
    return entry == NULL ? hashtable_put(hashtable, key, value, value_size) : entry;
}

HashTableIterator *hashtable_create_iterator(HashTable *hashtable) {
    HashTableIterator *iterator = malloc(sizeof(HashTableIterator));
    
    if(iterator == NULL) {
        fprintf(stderr, "%s:%d: Failed to allocate memory for hashtable iterator\n", __func__, __LINE__);
        return NULL;
    }

    iterator->hashtable = hashtable;
    iterator->current_index = 0;
    iterator->current_entry = NULL;

    return iterator;
}

void hashtable_iterator_destroy(HashTableIterator *iterator) {
    free(iterator);
}

HashTableEntry *hashtable_iterator_next(HashTableIterator *hashtable_iterator)
{
    HashTable *hashtable = hashtable_iterator->hashtable;

    while (hashtable_iterator->current_index < hashtable->capacity)
    {
        if (hashtable_iterator->current_entry == NULL)
        {
            hashtable_iterator->current_entry = hashtable->entries[hashtable_iterator->current_index];
        }
        else
        {
            hashtable_iterator->current_entry = hashtable_iterator->current_entry->next;
        }

        if (hashtable_iterator->current_entry != NULL)
        {
            return hashtable_iterator->current_entry;
        }

        hashtable_iterator->current_index++;
    }

    return NULL;
}

HashTableEntry *hashtable_entry_create(char *key, void *value)
{
    HashTableEntry *entry = malloc(sizeof(HashTableEntry));
    entry->key = key;
    entry->value = value;
    return entry;
}

char *hashtable_entry_get_key(HashTableEntry *entry) {
    return entry->key;
}

void *hashtable_entry_get_value(HashTableEntry *entry) {
    return entry->value;
}
