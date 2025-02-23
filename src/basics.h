#include <string.h>
#include <stdint.h>
#include "lexer.h"
#include "parser.h"
#include "sourcetree.h"

/*
function funame = (int32 a, int32, b) => int
{
    if (a < b)
    {
        return b - a
    }
    int32 x = a*a
    return x + b
}

function something = defun(int32 in, int32 ptr out) => ptr
{
    deref(out) = in 
}

struct TestStruct = defstruct
(
    sint8 type,
     
)

OR AND


macro ifmacro = defmacro( "if" (boolean) {code}) =>
{
    //
}

macro elseifmacro = defmacro(ifmacro "else if" (boolean) {code})
macro elsemacro = defmacro((ifmacro or elseifmacro) "else" {code})

macro condmacro = defmacro( "cond" ((boolean) {code}, adinfinitum ) )
{
    //  
}

macro addition = defmacro( (the number x) "+" (the number y) )
{
    //
}
*/

//the basics of any turing machine are branches, assignment, and operation
//branchs change which instructions are being executed,
//assignment "assigns" data to a location in memory
//operation does some sort of transformation on data

//basically defining the IR
//branch:
//unconditional, conditional, call, return
//assign:
//move push pop
//operation:
//+ - / * or and xor not etc

struct RuleDef
{
    Symbol *elements;
};

char *NUMBER = "NUMBER";
char *binary_ops_number[] = {"+", "-", "*", "/", "^", "<<", ">>", "<|", "|>", "or", "and", "xor"};
char *unary_ops_number[] = {"-", "~", "not"};

void create_type_symbols()
{
    Symbol *sym = symboltable_get_symbol(NUMBER);
    for (int i = 0; i < 12; i++)
    {
        sym = symboltable_get_symbol(binary_ops_number[i]);
    }
}

void construct_basic_rules(DynamicArray *rules)
{
    create_type_symbols();

    dynamicarray_initialize(rules, sizeof(Symbol**), 16);
    dynamicarray_append(rules, );

    dynamicarray_append(rules, );
}

//variable declaration
//      variable name
//      /       \
//  type         =
//                \
//                 value 

//have to manually construct all of these
SRC_Node operation_add = {false, SRC_OPERATOR, "+", NULL};
SRC_Node operation_sub = {false, SRC_OPERATOR, "-", NULL};
SRC_Node operation_mul = {false, SRC_OPERATOR, "*", NULL};
SRC_Node operation_div = {false, SRC_OPERATOR, "/", NULL};
SRC_Node operation_ior = {false, SRC_OPERATOR, "|", NULL};
SRC_Node operation_xor = {false, SRC_OPERATOR, "^", NULL};
SRC_Node operation_and = {false, SRC_OPERATOR, "&", NULL};
SRC_Node operation_not = {false, SRC_OPERATOR, "~", NULL};
SRC_Node operation_shl = {false, SRC_OPERATOR, "<<", NULL};
SRC_Node operation_shr = {false, SRC_OPERATOR, ">>", NULL};
SRC_Node operation_rol = {false, SRC_OPERATOR, "<|", NULL};
SRC_Node operation_ror = {false, SRC_OPERATOR, "|>", NULL};
SRC_Node operation_sqrt = {false, SRC_OPERATOR, "sqrt", NULL};

SRC_Node branch_unconditional = {false, SRC_OPERATOR, "branch uncond", NULL};
// children: destination
SRC_Node branch_conditional = {false, SRC_OPERATOR, "branch cond", NULL};
// children: destination, logical comparison
SRC_Node branch_call = {false, SRC_OPERATOR, "branch call", NULL};
// children: destination
SRC_Node branch_return = {false, SRC_OPERATOR, "branch retn", NULL};
// children: null

SRC_Node assign_move = {false, SRC_OPERATOR, "assign move", NULL};
// children: destination source
SRC_Node assign_push = {false, SRC_OPERATOR, "assign push", NULL};
// children: source
SRC_Node assign_pop = {false, SRC_OPERATOR, "assign pop", NULL};
// children: destination


//basic parsing rules for the language
ParseRule list_start =  { "(", 30, PARSE_LIST_START_OPERATOR };
ParseRule list_delim =  { ",", 30, PARSE_LIST_DELIMIT_OPERATOR };
ParseRule list_end =    { ")", 30, PARSE_LIST_END_OPERATOR };

ParseRule code_start =  { "{", 30, PARSE_LIST_START_OPERATOR };
ParseRule code_delim =  { ",", 30, PARSE_LIST_DELIMIT_OPERATOR };
ParseRule code_end =    { "}", 30, PARSE_LIST_END_OPERATOR };

//arithmetic
ParseRule op_plus = { "+", 30, PARSE_BINARY_OPERATOR };
ParseRule op_minus = { "-", 30, PARSE_BINARY_OPERATOR };
ParseRule op_mul = { "*", 30, PARSE_BINARY_OPERATOR };
ParseRule op_div = { "/", 30, PARSE_BINARY_OPERATOR };
ParseRule op_mod = { "%", 30, PARSE_BINARY_OPERATOR };

//bitwise
ParseRule op_not = { "~", 30, PARSE_PREFIX_OPERATOR };
ParseRule op_ior = { "|", 30, PARSE_BINARY_OPERATOR };
ParseRule op_xor = { "^", 30, PARSE_BINARY_OPERATOR };
ParseRule op_and = { "&", 30, PARSE_BINARY_OPERATOR };
ParseRule op_shl = { "<<", 30, PARSE_BINARY_OPERATOR };
ParseRule op_shr = { ">>", 30, PARSE_BINARY_OPERATOR };
ParseRule op_rol = { "<|", 30, PARSE_BINARY_OPERATOR };
ParseRule op_ror = { "|>", 30, PARSE_BINARY_OPERATOR };

//logical
ParseRule op_logical_not = { "not", 30, PARSE_PREFIX_OPERATOR };
ParseRule op_logical_ior = { "or", 30, PARSE_BINARY_OPERATOR };
ParseRule op_logical_xor = { "xor", 30, PARSE_BINARY_OPERATOR };
ParseRule op_logical_and = { "and", 30, PARSE_BINARY_OPERATOR };

ParseRule assign = { "=", 30, PARSE_BINARY_OPERATOR };

void assignment(SRC_Node *node)
{
    
}

Ruleset *construct_base_ruleset()
{
    Ruleset *ruleset = ruleset_create();

    LexerRules lrules;

    ruleset_add_parserule(ruleset, &list_start);
    ruleset_add_parserule(ruleset, &list_delim);
    ruleset_add_parserule(ruleset, &list_end);

    ruleset_add_parserule(ruleset, &op_plus);
    ruleset_add_parserule(ruleset, &op_minus);
    ruleset_add_parserule(ruleset, &op_mul);
    ruleset_add_parserule(ruleset, &op_div);
    
    ruleset_add_parserule(ruleset, &op_not);
    ruleset_add_parserule(ruleset, &op_ior);
    ruleset_add_parserule(ruleset, &op_xor);
    ruleset_add_parserule(ruleset, &op_and);
    ruleset_add_parserule(ruleset, &op_shl);
    ruleset_add_parserule(ruleset, &op_shr);
    ruleset_add_parserule(ruleset, &op_rol);
    ruleset_add_parserule(ruleset, &op_ror);

    ruleset_add_parserule(ruleset, &op_logical_not);
    ruleset_add_parserule(ruleset, &op_logical_ior);
    ruleset_add_parserule(ruleset, &op_logical_xor);
    ruleset_add_parserule(ruleset, &op_logical_and);

    return NULL;
}