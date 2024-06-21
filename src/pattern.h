#pragma once
//#include "utility.h"
#include <stdint.h>
#include <stdbool.h>
#include "parser.h"
#include "utility.h"

typedef AST_Node *(*Pattern)(AST_Node *node);

typedef struct Ruleset
{
    //ParseRule parserules;
    //Pattern patterns;
    DynamicArray parserules;
    DynamicArray patterns;
} Ruleset;

Pattern *pattern_arr;

AST_Node *branch(AST_Node *node);
AST_Node *assign(AST_Node *node);
AST_Node *operation(AST_Node *node);

AST_Node *apply_pattern(AST_Node *node, Pattern pattern);

Ruleset *ruleset_create();
void ruleset_add_parserules(ParseRule *rules);
void ruleset_add_patterns(Pattern *patterns);

void symboltable_initialize();
void symboltable_assign(char *symbol, uint64_t symbol_len, void *value);