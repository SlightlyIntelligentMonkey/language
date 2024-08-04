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

int handle_infix_operator(ParseContext *context, Token tok, SRC_Node *node)
{
    node->type = SRC_OPERATOR;
    node->data = (void*)tok;
    dynamicarray_initialize(&node->children, sizeof(SRC_Node), 2);
    SRC_Node *left = dynamicarray_get_element(&node->children, 0);
    SRC_Node *right = dynamicarray_get_element(&node->children, 1);

    stack_pop(&context->lastnode, left);
    right = handle_node(context);
    stack_push(&context->lastnode, node);
    return 0;
}

int handle_prefix_operator(ParseContext *context, Token tok, SRC_Node *node)
{
    node->type = SRC_OPERATOR;
    node->data = (void*)tok;
    dynamicarray_initialize(&node->children, sizeof(SRC_Node), 1);
    SRC_Node *unop = dynamicarray_get_element(&node->children, 0);

    unop = handle_node(context);
    stack_push(&context->lastnode, node);
    return 0;
}

int handle_postfix_operator(ParseContext *context, Token tok, SRC_Node *node)
{
    node->type = SRC_OPERATOR;
    node->data = (void*)tok;
    dynamicarray_initialize(&node->children, sizeof(SRC_Node), 1);
    SRC_Node *unop = dynamicarray_get_element(&node->children, 0);

    stack_pop(&context->lastnode, unop);
    stack_push(&context->lastnode, node);
    return 0;
}

int handle_list_start(ParseContext *context, Token tok, SRC_Node *node)
{
    node->type = SRC_LIST;
    node->data = tok;
    dynamicarray_initialize(&node->children, sizeof(uintptr_t), 2);

    stack_push(&context->listnodes, node);
    //stack_push(&context->lastnode, node);
    return 0;
}

int handle_list_end(ParseContext *context, Token tok, SRC_Node *node)
{
    SRC_Node *list;
    stack_pop(&context->listnodes, &list);
    stack_push(&context->lastnode, list);
    return 0;
}

int handle_delimit(ParseContext *context, Token tok, SRC_Node *node)
{
    stack_push(&context->lastnode, NULL);
    SRC_Node *list = stack_peek(&context->listnodes);
    //stack_pop(&context->listnodes, &list);
    node = handle_node(context);
    dynamicarray_append(&list->children, &node);
}

int handle_data_node(ParseContext *context, Token tok, SRC_Node *node)
{
    node->type = SRC_DATA;
    node->tok = tok;
    stack_push(&context->lastnode, node);
    return 0;
}

SRC_Node *handle_node(ParseContext *context)
{
    SRC_Node *node = malloc(sizeof(SRC_Node));
    memset(node, 0, sizeof(SRC_Node));
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
    HashMap *hmap = &context->ruleset->parserules;
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

SRC_Node *parse_ast_tree(ParseContext *context, DynamicArray dynarr)
{
    return handle_node(context);
}