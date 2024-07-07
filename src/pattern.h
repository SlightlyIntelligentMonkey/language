#pragma once
//#include "utility.h"
#include <stdint.h>
#include <stdbool.h>
#include "parser.h"
#include "utility.h"

typedef AST_Node *(*Transform)(AST_Node *node);

typedef struct Ruleset
{
    //ParseRule parserules;
    //Pattern patterns;
    LexerRules lrules;
    HashMap parserules;
    DynamicArray transforms;
} Ruleset;

AST_Node *branch(AST_Node *node);
AST_Node *assign(AST_Node *node);
AST_Node *operation(AST_Node *node);

AST_Node *apply_pattern(AST_Node *node, Transform transform);

Ruleset *ruleset_create();
void ruleset_add_parserules(ParseRule *rules);
void ruleset_add_patterns(Transform *transform);

void symboltable_initialize();
void symboltable_assign(char *symbol, uint64_t symbol_len, void *value);