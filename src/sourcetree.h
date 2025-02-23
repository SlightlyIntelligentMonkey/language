#pragma once
//#include "utility.h"
#include <stdint.h>
#include <stdbool.h>
#include "parser.h"
#include "utility.h"

typedef struct Symbol
{
    char *name;
    uint64_t hash;
    union
    {
        uint64_t data;
        void *data_ptr;
    };
    uint64_t metadata;
    Symbol *type;
} Symbol;

void symboltable_initialize();

Symbol *symboltable_get_symbol(char *symbol);

typedef struct SRC_Node
{
    //void *material;      //what was used to construct the node
    Symbol *type;
    void *data;
    int data_size;
    DynamicArray children;
    State *state;
    SRC_Node *next;
    SRC_Node *prev;
} SRC_Node;

typedef SRC_Node *(*Transform)(SRC_Node *node);

typedef struct Ruleset
{
    //ParseRule parserules;
    //Pattern patterns;
    LexerRules lrules;
    HashMap parserules;
    DynamicArray transforms;
} Ruleset;

Ruleset *ruleset_create();
Ruleset *ruleset_create_with(Rule *rules);
void ruleset_add(Ruleset *ruleset, Rule *rule);
Ruleset *combine_rulesets(Ruleset *a, Ruleset *b);