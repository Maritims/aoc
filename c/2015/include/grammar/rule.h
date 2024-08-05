#ifndef GRAMMAR_RULE_H
#define GRAMMAR_RULE_H

#include "hashtable.h"
#include <stdbool.h>

typedef struct grammar_rule_t grammar_rule_t;

char *grammar_rule_get_lhs(grammar_rule_t *rule);

char *grammar_rule_get_rhs(grammar_rule_t *rule);

HashTable *grammar_rule_parse(char **lines, size_t lines_length);

void grammar_rule_destroy(grammar_rule_t *rule);

#endif
