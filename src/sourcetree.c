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
    dynamicarray_initialize(&ruleset->transforms, sizeof(void *), 16);
}
Ruleset *ruleset_create_with(Rule *rules)
{
    //
}
void ruleset_add(Ruleset *ruleset, Rule *rule)
{
    //
}
Ruleset *combine_rulesets(Ruleset *a, Ruleset *b)
{
    //
}

HashMap symbol_table;
DynamicArray symbol_data;

void symboltable_initialize()
{
    hashmap_initialize(&symbol_table, 64, sizeof(Symbol), 0.75);
    dynamicarray_initialize(&symbol_data, sizeof(int *), 8);
}

Symbol *symboltable_create_symbol(char *symbol)
{
    Symbol sym;
    sym.name = symbol;
    sym.hash = 0;   //this needs to be set
    sym.data = 0;
    sym.metadata = 0;
    sym.type = NULL;
    hashmap_insert(&symbol_table, symbol, strlen(symbol), &sym);
}

Symbol *symboltable_get_symbol(char *symbol)
{
    Symbol *sym;
    if(hashmap_lookup(&symbol_table, symbol, strlen(symbol), &sym) == HASHMAP_NOT_FOUND)
        return symboltable_create_symbol(symbol);
    return sym;
}

void construct_src_tree(Ruleset *ruleset, char *raw)
{
    //
}