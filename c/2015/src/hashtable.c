#include <stdio.h>
#include <string.h>

#include "fnv.h"
#include "hashtable.h"

#define FNV_OFFSET_BASIS 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

#define DEBUG 1

void hashtable_print_result(HASHTABLE_RESULT hashtable_result)
{
    switch (hashtable_result)
    {
    case UNABLE_TO_ALLOCATE_HASHTABLE:
        fprintf(stderr, "Memory allocation for the hashtable structure was unsuccessful (result code: %d)\n", hashtable_result);
        break;
    case UNABLE_TO_ALLOCATE_ENTRIES:
        fprintf(stderr, "Memory allocation for the hashtable entries was unsuccessful (result code: %d)\n", hashtable_result);
        break;
    case KEY_IS_NULL:
        fprintf(stderr, "The specified key was NULL (result code: %d)\n", hashtable_result);
        break;
    case KEY_IS_EMPTY:
        fprintf(stderr, "The specified key was empty (result code: %d)\n", hashtable_result);
        break;
    default:
        fprintf(stderr, "An unknown result was encountered (result code: %d)\n", hashtable_result);
        break;
    }
}

/*
    Upon success this function returns a newly allocated hashtable with the given initial capacity, otherwise it returns a NULL pointer.
*/
HashTable *hashtable_create(uint32_t initial_capacity)
{
    HashTable *hashtable = malloc(sizeof(HashTable));
    if (hashtable == NULL)
    {
        fprintf(stderr, "Unable to allocate memory for hashtable.\n");
        return NULL;
    }

    hashtable->capacity = initial_capacity;
    hashtable->size = 0;
    hashtable->entries = (HashTableEntry **)calloc(hashtable->capacity, sizeof(HashTableEntry *));
    if (hashtable->entries == NULL)
    {
        fprintf(stderr, "Unable to allocate memory for hashtable entries.\n");
        free(hashtable);
        return NULL;
    }

    for (size_t i = 0; i < hashtable->capacity; i++)
    {
        hashtable->entries[i] = NULL;
    }

    return hashtable;
}

HASHTABLE_RESULT hashtable_destroy(HashTable *hashtable)
{
    if(hashtable == NULL)
    {
        fprintf(stderr, "hashtable is NULL\n");
        return FAILURE;
    }

    if(hashtable->entries == NULL)
    {
        fprintf(stderr, "hashtable entries are NULL\n");
        return FAILURE;
    }

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
    free(hashtable);

    return SUCCESS;
}

/*
    Upon success the hash of the given char array is returned, otherwise a NULL pointer is returned.
    Callers must remember to free() the return value.
*/
uint64_t *hashtable_hash(const char *key)
{
    return fnv1a(key);
}

/*
    Upon success the a pointer to the element with the given key is returned, otherwise a NULL pointer is returned.
*/
HashTableEntry *hashtable_get(const HashTable *hashtable, const char *key)
{
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

HASHTABLE_RESULT hashtable_rehash(HashTable *hashtable)
{
    size_t new_capacity = hashtable->capacity * 2;
    HashTableEntry **new_entries = calloc(new_capacity, sizeof(HashTableEntry*));
    if(new_entries == NULL)
    {
        return UNABLE_TO_ALLOCATE_ENTRIES;
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

    return SUCCESS;
}

/*
    Upon successful insertion the new entry is returned.
    If an entry with the given key already exists, that element's value is updated and the updated entry is returned.
    If insertion fails, a NULL pointer is returned.
*/
HashTableEntry *hashtable_put(HashTable *hashtable, char *key, void *value, size_t value_size, uint32_t flags)
{
    if(value == 0)
    {
        fprintf(stderr, "%s(): Unable to insert element into the hashtable. The value argument cannot be 0.\n", __func__);
        return NULL;
    }

    HashTableEntry *existing_entry = hashtable_get(hashtable, key);
    if (existing_entry != NULL)
    {
        free(existing_entry->value);
        existing_entry->value = malloc(value_size);
        sprintf(existing_entry->value, "%s", value);
        return existing_entry;
    }
    
    if (hashtable->size >= hashtable->capacity)
    {
        HASHTABLE_RESULT hashtable_rehash_result = hashtable_rehash(hashtable);
        if(hashtable_rehash_result != SUCCESS)
        {
            fprintf(stderr, "%s(): Unable to rehash the hashtable. Aborting insertion.\n", __func__);
            return NULL;
        }
    }

    uint64_t *hash = hashtable_hash(key);
    if(hash == NULL)
    {
        fprintf(stderr, "%s(): Unable to hash the given key \"%s\"\n", __func__, key);
        return NULL;
    }

    size_t index = *hash % hashtable->capacity;
    HashTableEntry *new_entry = malloc(sizeof(HashTableEntry));
    if(new_entry == NULL)
    {
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
    sprintf(new_entry->value, "%s", value);
    new_entry->next = hashtable->entries[index];
    hashtable->entries[index] = new_entry;
    hashtable->size++;

    free(hash);

    return new_entry;
}

/*
    Upon encountering an element with the given key, the corresponding element is returned.
    If no element is found, a new element is inserted into the hashtable and returned.
    If insertion fails, a NULL pointer is returned.
*/
HashTableEntry *hashtable_put_if_absent(HashTable *hashtable, char *key, void *value, size_t value_size, uint32_t flags)
{
    HashTableEntry *entry = hashtable_get(hashtable, key);
    return entry == NULL ? hashtable_put(hashtable, key, value, value_size, flags) : entry;
}

HASHTABLE_RESULT hashtable_create_iterator(HashTable *hashtable, HashTableIterator **out_hashtable_iterator)
{
    *out_hashtable_iterator = malloc(sizeof(HashTableIterator));
    (*out_hashtable_iterator)->hashtable = hashtable;
    (*out_hashtable_iterator)->current_index = 0;
    (*out_hashtable_iterator)->current_entry = NULL;

    return SUCCESS;
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
