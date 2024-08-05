#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grammar/rule.h"
#include "hashtable.h"
#include "string4c.h"

struct grammar_rule_t {
    char *lhs;
    char *rhs;
    bool lhs_is_terminal;
    bool rhs_is_terminal;
};

char *grammar_rule_get_lhs(grammar_rule_t *rule) {
    if(rule == NULL) {
        fprintf(stderr, "%s:%d: Parameter \"rule\" cannot be NULL\n", __func__, __LINE__);
        return NULL;
    }

    return rule->lhs;
}

char *grammar_rule_get_rhs(grammar_rule_t *rule) {
    if(rule == NULL) {
        fprintf(stderr, "%s:%d: Parameter \"rule\" cannot be NULL\n", __func__, __LINE__);
        return NULL;
    }
    
    return rule->rhs;
}

HashTable *grammar_rule_parse(char **lines, size_t lines_length) {
    if(lines == NULL) {
        fprintf(stderr, "%s:%d: Parameter \"lines\" cannot be NULL\n", __func__, __LINE__);
        return NULL;
    }

    HashTable *rules = hashtable_create(10);

    for(size_t i = 0; i < lines_length; i++) {
        char **tokens;
        size_t number_of_tokens;
        string_split(&tokens, &number_of_tokens, lines[i], " => ");

        if(number_of_tokens != 2) {
            fprintf(stderr, "%s:%d: Line %zu is invalid: %s", __func__, __LINE__, i + 1, lines[i]);
            continue;
        }

        hashtable_put(rules, tokens[0], tokens[1], sizeof(tokens[1]), 0);
        free(tokens);
    }

    return rules;
}

void grammar_rule_destroy(grammar_rule_t *rule) {
    free(rule->lhs);
    free(rule->rhs);
    free(rule);
}
