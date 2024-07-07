//typedef struct Special

enum TokType
{
    TOK_VALUE,
    TOK_SPECIAL,
    TOK_COMMENT,
    TOK_END
};

typedef struct Token
{
    enum TokType type;
    //raw token text
    char *val;
    unsigned int len;
    //whitespace preceding the token
    char *precwhitespace;
    unsigned int whitespacelen;

    //unsigned int pos;
    //location information for error message help
    unsigned int line, col, colend;
} Token;

typedef struct LexerRules
{
    char *whitespace;
    char **special;
} LexerRules;
typedef struct LexerContext
{
    LexerRules *lrules;
    char *input;
    unsigned int inputlen;
    unsigned int pos;
    unsigned int line;
    unsigned int linestart;
    int error;
} LexerContext;

Token peek_next_tok(LexerContext *context);
Token eat_next_tok(LexerContext *context);