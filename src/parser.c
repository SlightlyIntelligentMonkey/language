#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "utility.h"
#include "parser.h"

//const char *types[] = {
//    "void",
//    "bool",
//    "sint",
//    "uint",
//    "float",
//    "char",
//    "string",
//    "func"
//};

ParseRule create_parse_rule(char *identifier, enum ParseType type)
{
    ParseRule rule;
    rule.identifier = strdup(identifier);
    rule.hash = FNV_hash(identifier, strlen(identifier));
    rule.type = type;
    return rule;
}

int handle_infix_operator(ParseContext *context, Token tok, AST_Node *node)
{
    node->type = AST_NODE_BINARY;
    node->tok = tok;

    stack_pop(&context->lastnode, node->inop.left);
    node->inop.right = handle_node(context);
    stack_push(&context->lastnode, node);
    return 0;
}

int handle_prefix_operator(ParseContext *context, Token tok, AST_Node *node)
{
    node->type = AST_NODE_UNARY;
    node->tok = tok;

    node->unop.operand = handle_node(context);
    stack_push(&context->lastnode, node);
    return 0;
}

int handle_postfix_operator(ParseContext *context, Token tok, AST_Node *node)
{
    node->type = AST_NODE_UNARY;
    node->tok = tok;

    stack_pop(&context->lastnode, &node->unop.operand);
    stack_push(&context->lastnode, node);
    return 0;
}

int handle_list_start(ParseContext *context, Token tok, AST_Node *node)
{
    node->type = AST_NODE_LIST;
    node->tok = tok;
    dynamicarray_initialize(&node->list.node_array, sizeof(uintptr_t), 4);

    stack_push(&context->listnodes, node);
    //stack_push(&context->lastnode, node);
    return 0;
}

int handle_list_end(ParseContext *context, Token tok, AST_Node *node)
{
    AST_Node *list;
    stack_pop(&context->listnodes, &list);
    stack_push(&context->lastnode, list);
    return 0;
}

int handle_delimit(ParseContext *context, Token tok, AST_Node *node)
{
    stack_push(&context->lastnode, NULL);

    AST_Node *list;
    stack_pop(&context->listnodes, &list);
    node = handle_node(context);
    dynamicarray_append(&list->list.node_array, &node);
}

int handle_data_node(ParseContext *context, Token tok, AST_Node *node)
{
    node->type = AST_NODE_DATA;
    node->tok = tok;
    stack_push(&context->lastnode, node);
    return 0;
}

AST_Node *handle_node(ParseContext *context)
{
    AST_Node *node = malloc(sizeof(AST_Node));
    memset(node, 0, sizeof(AST_Node));
    //get next not comment token
    Token tok = eat_next_tok(&context->lexcontext);
    while(tok.type == TOK_COMMENT)
    {
        tok = eat_next_tok(&context->lexcontext);
    }

    if(tok.type == TOK_END)
        return NULL;

    //construct the ast node
    ParseType ptype;
    for (size_t i = 0; i < context->ruleset_count; i++)
    {
        HashMap hmap = context->rulesets[i].parserules;
        if (hashmap_lookup(hmap, tok.val, tok.len, &ptype) == HASHMAP_SUCCESS)
        {
            switch (ptype)
            {
            case PARSE_BINARY_OPERATOR:
                return handle_infix_operator(context, tok, node);
            case PARSE_PREFIX_OPERATOR:
                return handle_prefix_operator(context, tok, node);
            case PARSE_POSTFIX_OPERATOR:
                return handle_postfix_operator(context, tok, node);
            case PARSE_LIST_START_OPERATOR:
                return handle_list_start(context, tok, node);
            case PARSE_LIST_END_OPERATOR:
                return handle_list_end(context, tok, node);
            case PARSE_LIST_DELIMIT_OPERATOR:
                return handle_delimit(context, tok, node);
            default:
                return handle_data_node(context, tok, node);
            }
        }
        else // generic node
        {
            return handle_data_node(context, tok, node);
        }
    }
}

int parse(ParseContext *context, DynamicArray dynarr)
{
    AST_Node *result = 0;
    int i = 0;
    for(; result != NULL; i++)
    {
        result = handle_node(context);
    }

    return i;
}

int flatten(AST_Node *node, DynamicArray dynarr, int loc)
{
    int len0 = 0;
    int len1 = 0;
    switch(node->type)
    {
    case AST_NODE_DATA:
        memcpy(&dynarr.data[loc], node->tok.val, node->tok.len);
        return node->tok.len;
    case AST_NODE_UNARY:
        len0 = flatten(node->unop.operand, dynarr, loc);
        memcpy(&dynarr.data[loc+len0], node->tok.val, node->tok.len);
        return len0 + node->tok.len;
    case AST_NODE_BINARY:
        len0 = flatten(node->inop.left, dynarr, loc);
        memcpy(&dynarr.data[loc+len0], node->tok.val, node->tok.len);
        len1 = flatten(node->inop.left, dynarr, loc);
        return len0 + node->tok.len + len1;
    case AST_NODE_LIST:
        len0 = loc;
        for (unsigned int i = 0; i < node->list.node_array.used; i++)
            len0 += flatten(node->list.node_array.data[i], dynarr, len0);
        return len0-loc;
    }
}