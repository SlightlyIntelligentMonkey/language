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

int get_relavent_items(DynamicArray *newitems, Symbol *type, DynamicArray *olditems, DynamicArray *reductions)
{
    for (int i = 0; i < olditems->used; i++)
    {
        Reduction *red = dynamicarray_get_element(olditems, i);

        for (int j = 0; j < reductions->used; j++)
        {
            Reduction *redc = dynamicarray_get_element(reductions, j);
            int k;
            for (k = 0; k < redc->src.used; k++)
            {
                Symbol **sym = dynamicarray_get_element(&red->src, k);
                Symbol **symc = dynamicarray_get_element(&redc->src, k);
                if(*sym != *symc)
                    break;
            }
            Symbol **sym = dynamicarray_get_element(&redc->src, k);
            if(type == *sym)
                dynamicarray_append(newitems, redc);
        }
    }
    return 0;
}

uint64_t reduction_items_hash(Reduction *items, uint64_t count)
{
    uint64_t hash = 0xCBF29CE484222325;
    for (uint64_t i = 0; i < count; i++)
    {
        ;
    }
    return hash;
}

State *find_state(DynamicArray *items, HashMap *redtostate, DynamicArray *states)
{
    State *state;
    if(hashmap_lookup_copy(NULL, items->data, items->used, &state) == HASHMAP_SUCCESS)
        return state;
    return NULL;
}

State *create_state(DynamicArray *reductions, DynamicArray *relavent_items, DynamicArray *types, HashMap *redtostate, DynamicArray *states)
{
    State state;
    hashmap_initialize(&state.next, types->used, sizeof(State*), 0.75);
    DynamicArray newitems;
    for (int i = 0; i < types->used; i++)
    {
        Symbol *type = dynamicarray_get_element(types, i);

        int count = get_relavent_items(&newitems, type, relavent_items, reductions);

        //find or create state corresponding to relavent items (possible bug here if there's recursion with the relavent items and state)
        State *new;
        if(hashmap_lookup_copy(redtostate, newitems.data, newitems.used, &new) == HASHMAP_NOT_FOUND)
            new = create_state(reductions, &newitems, types, redtostate, states);

        hashmap_insert(&state.next, type, 0, new);
    }
    dynamicarray_append(states, &state);
}

 void union_reductions(DynamicArray *reductions, DynamicArray *add)
{
    
}

void subtract_reductions(DynamicArray *reductions , DynamicArray *remove)
{

}

//refer to Context-sensitive parsing for programming languages
//https://www.sciencedirect.com/science/article/pii/S2590118422000697
State *next_state(State *current, DynamicArray *w, SRC_Node *src)
{
    State *state;
    if(hashmap_lookup_copy(&current->next, &src, sizeof(Item), &state) != HASHMAP_SUCCESS)
        return NULL;
    return state;
}

SRC_Node *next_parse_symbol(LexerContext *lexcontext, MemoryPool *wk, SRC_Node *src)
{
    if(src->next == NULL)
    {
        Token tok = eat_next_tok(lexcontext);
        if(tok.type == TOK_END)
            return NULL;
        //lazy recursive solution lmao
        if(tok.type == TOK_COMMENT)
            return next_parse_symbol(lexcontext, wk, src);
        SRC_Node *node = memorypool_allocate_block(wk, 1);
        node->prev = src;
        node->type = symboltable_get_symbol("Token");
        //node->data = ;
        return node;
    }
    return src->next;
}

SRC_Node *get_node(SRC_Node *src, int offset)
{
    if(offset > 0)
    {
        for (int i = 0; i < offset; i++)
            if(src->next != NULL)
                src = src->next;
    }
    else if(offset < 0)
    {
        for (int i = 0; i < -offset; i++)
            src = src->prev;
    }
    return src;
}

void reductions_from_rules(DynamicArray *reductions, DynamicArray *rules)
{
    ;
}

void create_dfa(DFA *dfa, DynamicArray *rules)
{
    dynamicarray_initialize(&dfa->reductions, sizeof(Reduction), 2048);
    dynamicarray_intialize(&dfa->srcnode_types, sizeof(Symbol *), 256);
    hashmap_initialize(&dfa->redtostate, 2048, sizeof(State *), 0.75);
    dfa->redtostate.hash = reduction_items_hash;
    reductions_from_rules(&dfa->reductions, rules);
    //create states from reductions
}

void dfa_parse(ParseContext *context)
{
    SRC_Node *head;
    MemoryPool wk;    //wk in algorithm
    memorypool_initialize(&wk, sizeof(SRC_Node), 1024);
    State *s = context->dfa.init;
    while(true) //length of W k
    {
        //get next state
        head = next_parse_symbol(context->lexcontext, &wk, head);
        if(head == NULL)
            break;
        s = next_state(s, &wk, head);

        head->state = s;

        //no reduction item exists for the current string
        //meaning we apply a reduction
        if(s->items.used == 0)
        {
            //apply the rule
            Reduction *apply = &s->apply;
            SRC_Node *start = get_node(head, -apply->pos);
            SRC_Node *next = NULL;
            if(head->next != NULL)
                next = head->next;

            SRC_Node *current, *previous = start;
            for (int i = 0; i < apply->dst.used; i++)
            {
                SRC_Node *node = dynamicarray_get_element(&apply->dst, i);
                // create nonterminal
                if (node->type == NULL)
                {
                    current = memorypool_allocate_block(&wk, 1);
                    dynamicarray_initialize(&current->children, sizeof(SRC_Node*), node->children.used);
                    for (int c = 0; c < node->children.used; c++)
                    {
                        SRC_Node *new;
                    }
                }
                // don't create nonterminal
                else
                {
                    current = get_node(start, node->data_size);  
                }
                //link
                previous->next = current;
                current->prev = previous;
                previous = current;
            }
            //last piece of linking
            if(next != NULL)
            {
                next->prev = current;
                current->next = next;
            }
            else
                current->next = NULL;
            //set state to previous state
            head = start;
            s = head->state;
        }
    }
}