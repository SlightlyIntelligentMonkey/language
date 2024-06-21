#include "utility.h"
#include "pattern.h"
#include "parser.h"

/*
    (branch address1)
    (assign testing 1.0)
    address1:
    (assign testing 2.0)

    (assign testing2 testing)

    (assign (operation))
*/

typedef struct Symbol
{
    char *str;
    uint64_t hash;
    uint64_t data;
    uint64_t metadata;
    Symbol *type;
} Symbol;

AST_Node *branch(AST_Node *node)
{
    return node;
}
AST_Node *assign(AST_Node *node)
{
    node->list.node_array.data[1];

    Symbol *symbol;
    HashmapReturn result = hashmap_lookup(&symbol_table, node->tok.val, node->tok.len, &symbol);
    if(result == HASHMAP_NOT_FOUND)
    {
        Symbol new_symbol;
        //create new symbol
        hashmap_insert(&symbol_table, node->tok.val, node->tok.len, &new_symbol);
    }
    return node;
}
AST_Node *operation(AST_Node *node)
{
    node->list.node_array.data[1];
    return node;
}

AST_Node *apply_pattern(AST_Node *tree, Pattern pattern)
{
    AST_Node *result = tree;
    while(result != NULL)
        result = pattern(result);
    switch(result->type)
    {
    case AST_NODE_BINARY:
        apply_pattern(result->inop.left, pattern);
        apply_pattern(result->inop.right, pattern);
        break;
    case AST_NODE_LIST:
        for (int i = 0; i < result->list.node_array.used; i++)
            apply_pattern(result->list.node_array.data[i], pattern);
        break;
    case AST_NODE_UNARY:
        apply_pattern(result->unop.operand, pattern);
        break;
    default:
        break;
    }
    return result;
}

HashMap symbol_table;
DynamicArray symbol_data;

void symboltable_initialize()
{
    hashmap_initialize(&symbol_table);
    dynamicarray_initialize(&symbol_data, sizeof(int *), 8);
}

void symboltable_assign(char *symbol, uint64_t symbol_len, void *value)
{
    hashmap_lookup(&symbol_table, symbol, symbol_len, NULL);
}