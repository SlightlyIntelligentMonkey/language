#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "error.h"
#include "lexer.h"

unsigned long long int FNV_hash(const char *input, unsigned int len)
{
    const unsigned long long int FNV_offset_basis = 0xCBF29CE484222325;
    const unsigned long long int FNV_prime = 0x00000100000001B3;

    unsigned long long int hash = FNV_offset_basis;
    for(unsigned int i = 0; i < len; i++)
    {
        hash = hash ^ input[i];
        hash *= FNV_prime;
    }
    return hash;
}

inline void delete_token(Token *tok)
{
    if(tok->val != NULL)
        free(tok->val);
    free(tok);
}

//checks if a character is whitespace
inline bool check_whitespace(const char input)
{
    const char whitespace[] = { ' ', '\t', '\n', '\r' };
    for(unsigned int i = 0; i < sizeof(whitespace); i++)
        if(input == whitespace[i])
            return true;
    return false;
}

char *special[] =
{
    ".", "{", "}", "(", ")", "<", ">", "[", "]", "=", "+", "-", "*", "/", "?", "'", "\"", "`", "~", "!", "@", "#", "$", "%", "^", "&"
};
unsigned int special_count = sizeof(special)/sizeof(char*);
bool check_special(const char *input, unsigned int inputlen, char *str)
{
    for(unsigned int i = 0; i < special_count; i++)
    {
        if(inputlen < strlen(special[i]))
            continue;
        for(unsigned int c = 0; c < strlen(special[i]); c++)
        {
            if(input[c] != special[i][c])
                goto breakouter;
        }
        str = special[i];
        return true;
        breakouter:;
    }
    return false;
}
int check_comment(const char *input, unsigned int inputlen)
{
    if(inputlen < 2)
        return 0;
    if(input[0] == '/')
    {
        if(input[1] == '/')
        {
            unsigned int i = 2;
            for(; i < inputlen; i++)
            {
                if(input[i] == '\n')
                    break;
            }
            return i;
        }
        if(input[1] == '*')
        {
            unsigned int i = 2;
            for(; i < inputlen; i++)
            {
                if(input[i] == '*' && input[i+1] == '/')
                    break;
            }
            return i+1;
        }
    }
    return 0;
}

Token next_token(LexerContext *context)
{
    int state = 0;
    char *sptok;
    Token tok;
    unsigned int localpos = context->pos;

    //preceding whitespace handling
    tok.precwhitespace = context->input + localpos;
    tok.whitespacelen = 0;
    while(check_whitespace(context->input + localpos))
    {
        if(context->input[localpos] == '\n')
        {
            context->line++;
            context->linestart = localpos;
        }
        localpos++;
        tok.whitespacelen++;
    }

    if(localpos == context->inputlen)
    {
        tok.type = TOK_END;
        return tok;
    }

    int comlen = check_comment(context->input, context->inputlen);
    if(comlen)
    {
        tok.type = TOK_COMMENT;
        tok.len = comlen;
    }
    else if(check_special(context->input, context->inputlen, sptok))
    {
        tok.type = TOK_SPECIAL;
        tok.len = strlen(sptok);
    }
    else
    {
        tok.type = TOK_VALUE;
        unsigned int i = 1;
        for(; i < context->inputlen; i++)
        {
            if(check_whitespace(context->input[i]) || check_special(context->input, context->inputlen, sptok) || check_comment(context->input, context->inputlen))
                break;
        }
        tok.len = i;
    }

    tok.line = context->line;
    tok.col = context->linestart - localpos;
    tok.colend = tok.col + tok.len;
    return tok;
}

Token peek_next_tok(LexerContext *context)
{
    Token tok = next_token(context);
    return tok;
}
Token eat_next_tok(LexerContext *context)
{
    Token tok = next_token(context);
    context->pos += tok.whitespacelen + tok.len;
    return tok;
}