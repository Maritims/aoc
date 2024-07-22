#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fnv.h"
#include "hashset.h"

HashSet *hashset_create(size_t capacity)
{
    HashSet *hashset = malloc(sizeof(HashSet));
    if (hashset == NULL)
    {
        fprintf(stderr, "Unable to allocate memory for hashset.\n");
        return NULL;
    }

    hashset->capacity = capacity;
    hashset->size = 0;
    hashset->entries = calloc(hashset->capacity, sizeof(HashSetEntry *));
    if (hashset->entries == NULL)
    {
        fprintf(stderr, "Unable to allocate memory for hashset entries.\n");
        free(hashset);
        return NULL;
    }

    for (size_t i = 0; i < hashset->capacity; i++)
    {
        hashset->entries[i] = NULL;
    }

    return hashset;
}

uint64_t *hashset_hash(void *value, Type type)
{
    switch (type)
    {
    case TYPE_STRING:
        char *key = (char *)value;
        return fnv1a(key);
    default:
        fprintf(stderr, "Unsupported entry type: %d\n", type);
        return NULL;
    }
}

size_t hashset_index_of(HashSet *hashset, void *value, Type type)
{
    uint64_t *hash = hashset_hash(value, type);
    if (hash == NULL)
    {
        fprintf(stderr, "Unable to compute the hash of the given entry.\n");
        return -1;
    }

    return *hash % hashset->capacity;
}
bool hashset_contains(HashSet *hashset, void *value, Type type)
{
    uint64_t *hash = hashset_hash(value, type);
    if (hash == NULL)
    {
        fprintf(stderr, "Unable to compute the hash of the given entry.\n");
        return NULL;
    }

    size_t index = hashset_index_of(hashset, value, type);
    if(index == -1)
    {
        fprintf(stderr, "Unable to get index of given value.\n");
        return false;
    }

    HashSetEntry *entry = hashset->entries[index];
    return entry == NULL ? false : true;
}

void *duplicate_value(void *value, Type type)
{
    switch (type)
    {
    case TYPE_STRING:
        char *str = (char *)value;
        char *copy = calloc(strlen(str), sizeof(char));
        sprintf(copy, "%s", str);
        return copy;
    default:
        fprintf(stderr, "Unsupported type: %d\n", type);
        return NULL;
    }
}

HASHSET_RESULT hashset_rehash(HashSet *hashset)
{
    size_t new_capacity = hashset->capacity * 2;
    HashSetEntry **new_entries = calloc(new_capacity, sizeof(HashSetEntry*));
    if(new_entries == NULL)
    {
        fprintf(stderr, "Unable to allocate memory for additional entries.\n");
        return HASHSET_RESULT_FAILURE;
    }

    for(size_t i = 0; i < hashset->capacity; i++)
    {
        HashSetEntry *entry = hashset->entries[i];
        while(entry)
        {
            HashSetEntry *next = entry->next;
            size_t new_index = entry->hash % new_capacity;
            entry->next = new_entries[new_index];
            new_entries[new_index] = entry;
            entry = next;
        }
    }

    free(hashset->entries);
    hashset->entries = new_entries;
    hashset->capacity = new_capacity;

    return HASHSET_RESULT_SUCCESS;
}

size_t hashset_add(HashSet *hashset, void *value, Type type)
{
    size_t index_of = hashset_index_of(hashset, value, type);
    if (index_of > -1)
    {
        return index_of;
    }

    if (hashset->size >= hashset->capacity)
    {
        HASHSET_RESULT hashset_rehash_result = hashset_rehash(hashset);
        if (hashset_rehash_result != HASHSET_RESULT_SUCCESS)
        {
            fprintf(stderr, "Unable to rehash hashset. Aborting insertion.\n");
            return HASHSET_RESULT_FAILURE;
        }
    }

    uint64_t *hash = hashset_hash(value, type);
    if (hash == NULL)
    {
        fprintf(stderr, "Unable to hash given value.\n");
        return HASHSET_RESULT_FAILURE;
    }

    size_t index = *hash % hashset->capacity;
    HashSetEntry *new_entry = malloc(sizeof(HashSetEntry));
    if (new_entry == NULL)
    {
        fprintf(stderr, "Unable to allocate memory for new entry.\n");
        free(hash);
        return HASHSET_RESULT_FAILURE;
    }

    new_entry->hash = *hash;
    new_entry->value = duplicate_value(value, type);
    if(new_entry->value == NULL)
    {
        fprintf(stderr, "Unable to duplicate value.\n");
        return HASHSET_RESULT_FAILURE;
    }
    new_entry->next = hashset->entries[index];
    hashset->entries[index] = new_entry;
    hashset->size++;
    
    free(hash);

    return index;
}

HashSetIterator *hashset_iterator_create(HashSet *hashset)
{
    HashSetIterator *hashset_iterator = malloc(sizeof(HashSetIterator));
    if(hashset_iterator == NULL)
    {
        fprintf(stderr, "Unable to allocate memory for iterator.\n");
        return NULL;
    }

    hashset_iterator->entry = NULL;
    hashset_iterator->index = 0;
    hashset_iterator->hashset = hashset;

    return hashset_iterator;
}

HashSetEntry *hashset_iterator_next(HashSetIterator *hashset_iterator)
{
    if(hashset_iterator == NULL)
    {
        fprintf(stderr, "Parameter \"hashset_iterator\" cannot be NULL\n");
        return NULL;
    }

    while(hashset_iterator->index < hashset_iterator->hashset->capacity)
    {
        if(hashset_iterator->entry == NULL)
        {
            hashset_iterator->entry = hashset_iterator->hashset->entries[hashset_iterator->index];
        }
        else
        {
            hashset_iterator->entry = hashset_iterator->entry->next;
        }

        if(hashset_iterator->entry != NULL)
        {
            return hashset_iterator->entry;
        }

        hashset_iterator->index++;
    }

    return NULL;
}