#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct PoolSection
{
    void *memory;
    unsigned int size;
    uint32_t id;
} PoolSection;

void mpool_init(size_t pool_size);
PoolSection mpool_alloc(unsigned int size);
void mpool_free(PoolSection *psect);

typedef struct DynamicArray
{
    unsigned char *data;
    uintptr_t used;
    uintptr_t maxsize;
    uintptr_t elsize;
} DynamicArray;
void dynamicarray_initialize(DynamicArray *dynarr, unsigned int element_size, unsigned int initial_size);
void dynamicarray_resize(DynamicArray *dynarr, unsigned int newsize);
void dynamicarray_reserve(DynamicArray *dynarr, unsigned int newsize);
void *dynamicarray_append(DynamicArray *dynarr, void *el);
void *dynamicarray_new_element(DynamicArray *dynarr);
void dynamicarray_free(DynamicArray *dynarr);

typedef struct Stack
{
    unsigned char *data;
    unsigned char *sp;
    uintptr_t elsize;
    uintptr_t maxdepth;
} Stack;
void stack_initialize(Stack *stack, unsigned int element_size, unsigned int max_depth);
void stack_push(Stack *stack, void *data);
void stack_pop(Stack *stack, void *data);
void stack_free(Stack *stack);

inline uint64_t FNV_hash(const char *input, uint64_t len)
{
    const uint64_t FNV_offset_basis = 0xCBF29CE484222325;
    const uint64_t FNV_prime = 0x00000100000001B3;

    uint64_t hash = FNV_offset_basis;
    for(uint64_t i = 0; i < len; i++)
    {
        hash = hash ^ input[i];
        
        hash *= FNV_prime;
    }
    return hash;
}

typedef uint64_t (*HashFunc)(const char *, uint64_t);

//simple linear probing hashmap
typedef enum HashmapReturn
{
    HASHMAP_SUCCESS,
    HASHMAP_DUPLICATE,
    HASHMAP_NOT_FOUND
} HashmapReturn;
typedef struct HashmapPair
{
    uint64_t key;
    uint64_t val;
    bool occupied;
} HashmapPair;
typedef struct HashMap
{
    const float load_factor = 0.75;
    const int initial_size = 8;

    HashmapPair *pairs;
    size_t pair_count;
    size_t pair_max;
    DynamicArray data;
    HashFunc hash;
} HashMap;

void hashmap_initialize(HashMap *hmap);
void hashmap_free(HashMap *hmap);
HashmapReturn hashmap_insert(HashMap *hmap, char *key, uint64_t keylen, void *val);
HashmapReturn hashmap_delete(HashMap *hmap, char *key, uint64_t keylen);
HashmapReturn hashmap_lookup(HashMap *hmap, char *key, uint64_t keylen, void **val);
HashmapReturn hashmap_lookup2(HashMap *hmap, uint64_t hash, void **val);