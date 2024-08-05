#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "grammar/rule.h"
#include "hashset.h"
#include "grammar/grammar.h"

void test_grammar_create() {
    char **lines = calloc(4, sizeof(char*));
    for(size_t i = 0; i < 4; i++) {
        lines[i] = malloc(10);
    }
    lines[0] = "S => a";
    lines[1] = "S => AZ";
    lines[2] = "A => a";
    lines[3] = "Z => z";

    HashTable *rules        = grammar_rule_parse(lines, 4);
    grammar_t *grammar      = grammar_create("", rules);
    HashSet *non_terminals  = grammar_get_non_terminals(grammar);
    HashSet *terminals      = grammar_get_terminals(grammar);

    printf("non_terminals: %zu\n", hashset_get_size(non_terminals));
    printf("terminals: %zu\n", hashset_get_size(terminals));
}


int main() {
    test_grammar_create();

    printf("All tests passed\n");

    return EXIT_SUCCESS;
}
