#ifndef HASHSET_H
#define HASHSET_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum
{
    TYPE_STRING
} Type;

typedef struct HashSetEntry
{
    uint64_t hash;
    Type type;
    void *value;
    struct HashSetEntry *next;
} HashSetEntry;

typedef struct
{
    size_t capacity;
    size_t size;
    HashSetEntry **entries;
} HashSet;

typedef struct
{
    size_t index;
    HashSetEntry *entry;
    HashSet *hashset;
} HashSetIterator;

typedef enum
{
    HASHSET_RESULT_SUCCESS,
    HASHSET_RESULT_FAILURE,
    HASHSET_RESULT_NOOP
} HASHSET_RESULT;

size_t hashset_get_size(HashSet *hashset);

HashSet *hashset_create(size_t capacity);

uint64_t *hashset_hash(void *value, Type type);

int hashset_index_of(HashSet *hashset, void *value, Type type);

bool hashset_contains(HashSet *hashset, void *data, Type type);

int hashset_add(HashSet *hashset, void *data, Type type);

HashSetIterator *hashset_iterator_create(HashSet *hashset);

HashSetEntry *hashset_iterator_next(HashSetIterator *hashset_iterator);

#endif
