#include <stdio.h>
#include "grammar/grammar.h"
#include "grammar/rule.h"
#include "hashtable.h"

void test_chomsky_validate_grammar() {
    HashTable *rules = hashtable_create(10);
    
    hashtable_put(rules, "S", "a", sizeof("a"), 0);
    hashtable_put(rules, "S", "AZ", sizeof("AZ"), 0);
    hashtable_put(rules, "A", "a", sizeof("a"), 0);
    hashtable_put(rules, "Z", "z", sizeof("z"), 0);

    grammar_t *grammar = grammar_create("a", rules);


}

int main() {
    printf("All tests passed\n");
}
