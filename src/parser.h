#include "lexer.h"
#include "pattern.h"
#include "utility.h"

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
    unsigned long long int hash;
    ParseType type;
} ParseRule;

typedef struct ParseContext
{
    LexerContext lexcontext;
    Ruleset *rulesets;
    size_t ruleset_count;
    Stack listnodes;
    Stack lastnode;
} ParseContext;

//indices are used instead of pointers in case of memory copies
struct AST_Node;
struct InfixOperator_Node
{
    AST_Node *left;
    AST_Node *right;
};

struct UnaryOperator_Node
{
    AST_Node *operand;
};

struct List_Node
{
    DynamicArray node_array;
};

enum AST_Node_Type
{
    AST_NODE_DATA,
    AST_NODE_UNARY,
    AST_NODE_BINARY,
    AST_NODE_LIST
};

typedef struct AST_Node
{
    enum AST_Node_Type type;
    Token tok;      //token used to construct the node
    union
    {
        struct InfixOperator_Node inop;
        struct UnaryOperator_Node unop;
        struct List_Node list;
    };
} AST_Node;

//returns array of root parse tree create from the parsecontext
int parse(ParseContext *context, DynamicArray dynarr);

//flattens the ast tree starting from node given to an array
int flatten(AST_Node *node, DynamicArray dynarr, int loc);