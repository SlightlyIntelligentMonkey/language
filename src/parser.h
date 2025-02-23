#include "utility.h"
#include "lexer.h"
#include "sourcetree.h"

/*
parser :: Int -> Int -> Int
       -> Maybe Item
       -> ([(Bool,Cell)],[(Bool,Cell)])
       -> [Trace Cell]

data Reduction
  = Reduction {
      redId :: Int,
      redSrc :: [Symbol],
      redDst :: [DstSymbol],
      redOrig :: AST.Reduction }

instance Eq Reduction where
  r1==r2 = (redId r1)==(redId r2)
instance Ord Reduction where
  compare r1 r2 = compare (redId r1) (redId r2)

data CSR
  = CSR {
      csrSymbs :: [Symbol],
      csrTerms :: [Symbol],
      csrReds :: [Reduction],
      csrBeg :: Symbol,
      csrEnd :: Symbol,
      csrGoal :: Symbol }

type Item = (Reduction,Int)

data State
  = State {
      stId :: Int,
      stItems :: Set Item,
      stApp :: Maybe Item,
      stGoto :: Map Symbol State }
*/

/*typedef struct Reduction
{
    int id;
    int *src;   //symbol
    int *dst;   //dstsymbol
    SRC_Node *origin;   //AST
} Reduction;

typedef struct Item
{
    Reduction red;
    int integer;
} Item;*/

typedef struct Rule
{
    DynamicArray elements;
} Rule;

typedef struct Reduction
{
    int id;
    DynamicArray src;   //symbol
    DynamicArray dst;   //symbol
    int pos;
} Reduction;

typedef struct Item
{
    DynamicArray a1;
    DynamicArray a2;
} Item;

typedef struct State
{
    int id;
    DynamicArray items;
    SRC_Node *from;
    HashMap next;
    //rule to apply
    Reduction apply;
} State;

typedef struct DFA
{
    State *init;
    DynamicArray rules;
    DynamicArray srcnode_types;
    DynamicArray reductions;
    DynamicArray states;
    HashMap redtostate;
} DFA;

typedef struct ParseContext
{
    LexerContext *lexcontext;
    MemoryPool srcnodes;
    DynamicArray roots;
    DFA dfa;
} ParseContext;

SRC_Node *parse(ParseContext *context);