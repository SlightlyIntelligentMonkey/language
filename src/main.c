#include <stdio.h>
#include <string.h>
#include "utility.h"
#include "lexer.h"
#include "parser.h"

Buffer read_file(char *path)
{
    Buffer buf;
    FILE *fp = fopen(path, "r");
    fseek(fp, 0L, SEEK_END);
    buf.size = ftell(fp);
    rewind(fp);
    buf.data = malloc(buf.size);
    fread(buf.data, 1, buf.size, fp);
    fclose(fp);
    return buf;
}

void pile(char *data, int len, Ruleset *ruleset)
{
    LexerContext lcontext;
    memset(&lcontext, 0, sizeof(LexerContext));
    lcontext.lrules = &ruleset->lrules;
    lcontext.input = data;
    lcontext.inputlen = len;

    ParseContext pcontext;
    memset(&pcontext, 0, sizeof(ParseContext));
    pcontext.lexcontext = &lcontext;
    pcontext.ruleset = ruleset;
    stack_initialize(&pcontext.listnodes, sizeof(AST_Node*), 100);
    stack_initialize(&pcontext.lastnode, sizeof(AST_Node*), 100);

    //
}

void handle_option(const char *form1, const char *form2, char *arg)
{
    //match
    if(strcmp(form1, arg) == 0 || strcmp(form2, arg) == 0)
    {

    }
}
bool check_option(char *arg)
{
    if(arg[0] == '-')
        return true;
}

int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        printf("Help thing goes here");
        return 0;
    }

    //process options
    for(int i = 1; i < argc; i++)
    {
        //check for options
        if(argv[i][0] == '-')
            continue;
        
        char *sourcefile = argv[2];
        Buffer source = read_file(sourcefile);
        pile(source.data, source.size, NULL);
        free(source.data);
    }
}