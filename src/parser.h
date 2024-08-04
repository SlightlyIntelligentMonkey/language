#include "utility.h"
#include "lexer.h"
#include "sourcetree.h"

typedef enum e_ParseType
{
    PARSE_BINARY_OPERATOR,
    PARSE_PREFIX_OPERATOR,
    PARSE_POSTFIX_OPERATOR,
    PARSE_LIST_START_OPERATOR,
    PARSE_LIST_END_OPERATOR,
    PARSE_LIST_DELIMIT_OPERATOR,

    PARSE_DATA
} ParseType;

typedef struct ParseRule
{
    char *identifier;
    uint64_t hash;
    ParseType type;
} ParseRule;

typedef struct ParseContext
{
    LexerContext *lexcontext;
    Ruleset *ruleset;
    Stack listnodes;
    Stack lastnode;
} ParseContext;

//returns array of root parse tree create from the parsecontext
int parse(ParseContext *context, DynamicArray dynarr);

SRC_Node *parse_ast_tree(ParseContext *context, DynamicArray dynarr);