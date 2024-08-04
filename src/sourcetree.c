#include "utility.h"
#include "sourcetree.h"
#include "parser.h"

/*
    (branch address1)
    (assign testing 1.0)
    address1:
    (assign testing 2.0)

    (assign testing2 testing)

    (assign (operation))
*/

Ruleset *ruleset_create()
{
    Ruleset *ruleset = malloc(sizeof(Ruleset));
    hashmap_initialize(&ruleset->parserules, 16, sizeof(ParseRule), 0.75);
}
void ruleset_add_parserule(Ruleset *ruleset, ParseRule *rule)
{
    hashmap_insert(&ruleset->parserules, rule->identifier, strlen(rule->identifier), rule);
}
void ruleset_add_pattern(Ruleset *ruleset, Transform *transform)
{
    //
}

typedef struct Symbol
{
    char *str;
    uint64_t hash;
    uint64_t data;
    uint64_t metadata;
    Symbol *type;
} Symbol;

HashMap symbol_table;
DynamicArray symbol_data;

void symboltable_initialize()
{
    hashmap_initialize(&symbol_table, 64, sizeof(Symbol), 0.75);
    dynamicarray_initialize(&symbol_data, sizeof(int *), 8);
}

void symboltable_assign(char *symbol, uint64_t symbol_len, void *value)
{
    hashmap_lookup(&symbol_table, symbol, symbol_len, NULL);
}