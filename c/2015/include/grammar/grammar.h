#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "hashset.h"
#include "hashtable.h"

typedef struct grammar_t grammar_t;

grammar_t *grammar_create(const char *S, HashTable *P);

HashTable *grammar_get_rules(grammar_t *grammar);

HashSet *grammar_get_non_terminals(grammar_t *grammar);

HashSet *grammar_get_terminals(grammar_t *grammar);

HashSet *grammar_get_v_star(grammar_t *grammar);

void grammar_derive_with_leftmost_derivation(grammar_t *grammar, char *target_string);

void grammar_derive_with_rightmost_derivation(grammar_t *grammar, char *target_string);

void grammar_derive_with_parse_tree(grammar_t *grammar, char *target_string);

#endif
