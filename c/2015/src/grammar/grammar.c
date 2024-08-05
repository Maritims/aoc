#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grammar/grammar.h"
#include "hashtable.h"

/**
 * grammar_t: A grammar can be written as a 4-tuple (N, T, S, P) where:
 * - N is a set of varibles also known as non-terminal symbols or non-terminals.
 * - T is a set of constants also known as terminal symbols or terminals.
 * - S is a special variable called the Start symbol where S ∈ N.
 * - P is the production rules for N union T.
 */
struct grammar_t {
    HashSet *N;
    HashSet *T;
    HashSet *V_star; // N union T.
    const char *S;
    HashTable *P;
};

grammar_t *grammar_create(const char *S, HashTable *P) {
    if(S == NULL) {
        fprintf(stderr, "%s: Parameter \"S\" cannot be null\n", __func__);
        return NULL;
    }

    if(P == NULL) {
        fprintf(stderr, "%s: Parameter \"P\" cannot be NULL\n", __func__);
        return NULL;
    }

    grammar_t *grammar = malloc(sizeof(grammar_t));
    if(grammar == NULL) {
        fprintf(stderr, "%s:%d: Failed to allocate memory for grammar\n", __func__, __LINE__);
        return NULL;
    }

    grammar->S = S;
    grammar->P = P;

    // Determine terminals and non-terminals.
    HashSet *N                          = hashset_create(10);
    HashSet *T                          = hashset_create(10);
    HashSet *V_star                     = hashset_create(20);
    HashTableIterator *rules_iterator   = hashtable_create_iterator(P);
    HashTableEntry *rule;
    char **keys                         = hashtable_get_keys(P);
    size_t keys_length                  = hashtable_get_size(P);

    for(size_t i = 0; i < keys_length; i++) {
        hashset_add(N, keys[i], TYPE_STRING);
        hashset_add(V_star, keys[i], TYPE_STRING);
    }
    
    while((rule = hashtable_iterator_next(rules_iterator)) != NULL) {
        char *rhs = hashtable_entry_get_value(rule);

        if(strlen(rhs) > 1) {
            // Is this two or more non-terminals in conjunction, or a terminal in conjunction with one or more non-terminals?
        }

        printf("Checking %s... ", rhs);
        if(hashset_contains(N, rhs, TYPE_STRING)) {
            printf("it's a non-terminal\n");
        }
        else if(hashset_contains(T, rhs, TYPE_STRING)) {
            printf("it's a terminal\n");
        }
        else {
            printf("adding it as a terminal\n");
            hashset_add(T, rhs, TYPE_STRING);
        }
    }

    grammar->N = N;
    grammar->T = T;

    return grammar;
}

HashTable *grammar_get_rules(grammar_t *grammar) {
    if(grammar == NULL) {
        fprintf(stderr, "%s:%d: Parameter \"grammar\" cannot be NULL\n", __func__, __LINE__);
        return NULL;
    }

    return grammar->P;
}

HashSet *grammar_get_non_terminals(grammar_t *grammar) {
    return grammar->N;
}

HashSet *grammar_get_terminals(grammar_t *grammar) {
    return grammar->T;
}

void grammar_derive_with_leftmost_derivation(grammar_t *grammar, char *target_string){
}

void grammar_derive_with_rightmost_derivation(grammar_t *grammar, char *target_string) {
}

void grammar_derive_with_parse_tree(grammar_t *grammar, char *target_string) {
}
