#pragma once
//#include "utility.h"
#include <stdint.h>
#include <stdbool.h>
#include "parser.h"
#include "utility.h"

typedef struct Ruleset
{
    //ParseRule parserules;
    //Pattern patterns;
    LexerRules lrules;
    HashMap parserules;
    DynamicArray transforms;
} Ruleset;

typedef enum NodeType
{
    SRC_DATA,
    SRC_LIST,
    SRC_OPERATOR
} NodeType;
typedef struct SRC_Node
{
    bool fromtoken;
    //void *material;      //what was used to construct the node
    NodeType type;
    void *data;
    DynamicArray children;
} SRC_Node;

typedef SRC_Node *(*Transform)(SRC_Node *node);

Ruleset *ruleset_create();
void ruleset_add_parserule(Ruleset *ruleset, ParseRule *rule);
void ruleset_add_pattern(Ruleset *ruleset, Transform *transform);

void symboltable_initialize();
void symboltable_assign(char *symbol, uint64_t symbol_len, void *value);