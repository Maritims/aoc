#include <stdio.h>
#include "grammar/chomsky.h"
#include "grammar/rule.h"

bool chomsky_validate_grammar(grammar_t *grammar) {
    if(grammar == NULL) {
        fprintf(stderr, "%s:%d: Parameter \"grammar\" cannot be NULL\n", __func__, __LINE__);
        return false;
    }

    size_t rules_length = 0;
    HashTable *rules    = grammar_get_rules(grammar);
    bool valid          = true;

    // A CFG is in CNF if one of these rules are satisfied:
    // 1) A non-terminal generating a terminal.
    // 2) A non-terminal generating two non-terminals.
    // 3) Start symbol generating an empty string.
        
    // Rule 1
    for(size_t i = 0; i < rules_length; i++) {
    }

    return valid;
}
