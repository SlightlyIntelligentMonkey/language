#include "lexer.h"
#include "parser.h"
#include "pattern.h"
#include <string.h>

ParseRule construct_list_parserule()
{
    ParseRule prulestart;
    prulestart.type = PARSE_LIST_START_OPERATOR;
    prulestart.identifier = "(";
    prulestart.hash = 30;

    ParseRule pruledelim;
    pruledelim.type = PARSE_LIST_DELIMIT_OPERATOR;
    pruledelim.identifier = ",";
    pruledelim.hash = 30;
 
    ParseRule prulend;
    prulend.type = PARSE_LIST_END_OPERATOR;
    prulend.identifier = ")";
    prulend.hash = 30;
}

Ruleset *construct_base_ruleset()
{
    Ruleset ruleset;

    ParseRule lprule = construct_list_parserule();


    return NULL;
}

AST_Node *basic_assign(AST_Node *node)
{
    //((AST_Node*)(node->list.node_array.data))[0].tok.val
    if(node->type == AST_NODE_LIST)
    {
        AST_Node *list = (AST_Node *)(node->list.node_array.data);
        if(list[0].tok.val[0] == '(' && strcmp(list[1].tok.val, "assign") == 0)
        {
            //symboltable_assign(list[2].tok.val, list[2].tok.len, NULL);
        }
    }
    return NULL;
}

AST_Node *basic_branch(AST_Node *node)
{
    if(node->type == AST_NODE_LIST)
    {
        AST_Node *list = (AST_Node *)(node->list.node_array.data);
        if(list[0].tok.val[0] == '(' && strcmp(list[1].tok.val, "branch") == 0)
        {
            return NULL;
        }
    }
    return NULL;
}

AST_Node *basic_operation(AST_Node *node)
{
    if(node->type == AST_NODE_LIST)
    {
        AST_Node *list = (AST_Node *)(node->list.node_array.data);
        if(list[0].tok.val[0] == '(' && strcmp(list[1].tok.val, "operation") == 0)
        {
            return NULL;
        }
    }
    return NULL;
}