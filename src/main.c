#include <stdio.h>
#include "utility.h"
#include "lexer.h"
#include "parser.h"

LexerContext get_lexer_context(char *path)
{
    FILE *fp = fopen(path, "r");
    fseek(fp, 0L, SEEK_END);
    size_t sz = ftell(fp);
    rewind(fp);
    char *text = malloc(sz);
    fread(text, 1, sz, fp);
    fclose(fp);

    LexerContext lctx;
    lctx.input = text;
    lctx.inputlen = sz;

    return lctx;
}

int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        printf("give some sort of argument");
    }
    for(int i = 1; i < argc; i++)
    {
        ParseContext pctx;
        pctx.lexcontext = get_lexer_context(argv[i]);
        stack_initialize(&pctx.lastnode, sizeof(AST_Node*), 100);
        stack_initialize(&pctx.listnodes, sizeof(AST_Node*), 100);

    }
}