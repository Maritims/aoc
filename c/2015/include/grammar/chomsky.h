#ifndef CHOMSKY_H
#define CHOMSKY_H

#include <stdbool.h>
#include "grammar/grammar.h"

/*
 * chomsky_validate_grammar: Check whether the grammar is in Chomsky Normal Form (CNF).
 * @param grammar The grammar instance.
 * @return True if the grammar is in CNF, false otherwise.
 */
bool chomsky_validate_grammar(grammar_t *grammar);

#endif
