typedef struct Grammar Grammar;
typedef struct GrammarRule GrammarRule;

GrammarRule **grammar_get_rules(Grammar *grammar);

void grammar_add_rule(Grammar *grammar, const char *left, const char *right);

void earley_parse(const char *str, Grammar *grammar);
