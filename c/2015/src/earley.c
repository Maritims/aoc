#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "earley.h"

struct Grammar {
    struct GrammarRule **rules;
    size_t rules_capacity;
    size_t rules_length;
};

struct GrammarRule {
    char *left;
    char *right;
};

GrammarRule **grammar_get_rules(Grammar *grammar) {
    return grammar->rules;
}

static GrammarRule *grammar_rule_create(const char *left, const char *right) {
    GrammarRule *rule = malloc(sizeof(GrammarRule));
    
    if(rule == NULL) {
        fprintf(stderr, "%s:%d: Failed to allocate memory for creation of new grammar rule\n", __func__, __LINE__);
        return NULL;
    }

    rule->left = malloc(strlen(left));
    if(rule->left == NULL) {
        fprintf(stderr, "%s:%d: Failed to allocate memory for left hand side of grammar rule\n", __func__, __LINE__);
        free(rule);
        return NULL;
    }

    rule->right = malloc(strlen(right));
    if(rule->right == NULL) {
        fprintf(stderr, "%s:%d: Failed to allocate memory for right hand side of grammar rule\n", __func__, __LINE__);
        free(rule->left);
        free(rule);
        return NULL;
    }

    strcpy(rule->left, left);
    strcpy(rule->right, right);

    return rule;
}

void grammar_add_rule(Grammar *grammar, const char *left, const char *right) {
    GrammarRule *rule = grammar_rule_create(left, right);
    if(rule == NULL) {
        fprintf(stderr, "%s:%d: Failed to create grammar rule\n", __func__, __LINE__);
        return;
    }

    if(grammar->rules_length >= grammar->rules_capacity) {
        size_t new_capacity = grammar->rules_capacity * 2;
        GrammarRule **new_rules = realloc(grammar->rules, new_capacity);

        if(new_rules == NULL) {
            fprintf(stderr, "%s:%d: Failed to allocate additional memory for grammar rules\n", __func__, __LINE__);
            return;
        }

        grammar->rules = new_rules;
    }

    grammar->rules[grammar->rules_length++] = rule;
}

void earley_parse(const char *str, Grammar *grammar) {

}
