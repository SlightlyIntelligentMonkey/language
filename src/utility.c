#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "utility.h"


struct MemoryBlock
{
    uintptr_t address;
    size_t size;
};
struct MemoryPool
{
    size_t size;
    uintptr_t nextslot;
    uint32_t nextslot_size;
    struct MemoryBlock *blocks;
    uint32_t block_count;
    void *memory;
};
struct MemoryPool mpool;

void mpool_init(size_t pool_size)
{
    mpool.memory = malloc(pool_size);
    mpool.size = pool_size;
    mpool.blocks = malloc(pool_size / 32);
    mpool.block_count = 0;
    return;
}
PoolSection mpool_alloc(unsigned int size)
{
    PoolSection psect;
    if(mpool.nextslot_size > size)
    {
        psect.memory = (void*)((uintptr_t)mpool.memory+mpool.nextslot);
        if(mpool.nextslot_size - size > 64)
            psect.size = mpool.block_count;
        else
            psect.size = size;
    }

    return psect;
}
void mpool_free(PoolSection *psect)
{

}

void dynamicarray_initialize(DynamicArray *dynarr, unsigned int element_size, unsigned int initial_size)
{
    dynarr->elsize = element_size;
    dynarr->used = 0;
    dynamicarray_reserve(dynarr, initial_size);
}
void dynamicarray_resize(DynamicArray *dynarr, unsigned int newsize)
{
    dynarr->used = newsize;
    if(dynarr->used == dynarr->maxsize)
    {
        dynamicarray_reserve(dynarr, dynarr->maxsize*2);
    }
}
void dynamicarray_reserve(DynamicArray *dynarr, unsigned int newsize)
{
    dynarr->data = realloc(dynarr->data, newsize*dynarr->elsize);
    dynarr->maxsize = newsize;
}
void *dynamicarray_append(DynamicArray *dynarr, void *el)
{
    //resize the dynamic array if needed
    if(dynarr->used == dynarr->maxsize)
    {
        dynamicarray_resize(dynarr, dynarr->maxsize*2);
    }
    memcpy(dynarr->data + dynarr->elsize*dynarr->used, el, dynarr->elsize);
    dynarr->used++;
    return (dynarr->data + dynarr->elsize*dynarr->used);
}
void *dynamicarray_new_element(DynamicArray *dynarr)
{
    dynarr->used++;
    if(dynarr->used == dynarr->maxsize)
    {
        dynamicarray_resize(dynarr, dynarr->maxsize*2);
    }
    return dynarr->data + dynarr->used*dynarr->elsize;
}
void dynamicarray_free(DynamicArray *dynarr)
{
    free(dynarr->data);
}

void stack_initialize(Stack *stack, unsigned int element_size, unsigned int max_depth)
{
    stack->elsize = element_size;
    stack->data = malloc(stack->elsize*max_depth);
    stack->sp = stack->data;
}
void stack_push(Stack *stack, void *data)
{
    if((uintptr_t)stack->sp - (uintptr_t)stack->data <= stack->maxdepth*stack->elsize)
        return;
    memcpy(stack->sp, data, stack->elsize);
    (uintptr_t)stack->sp += stack->elsize;
}
void stack_pop(Stack *stack, void *data)
{
    memcpy(data, stack->sp, stack->elsize);
    (uintptr_t)stack->sp -= stack->elsize;
}
void stack_free(Stack *stack)
{
    free(stack->data);
}


void hashmap_initialize(HashMap *hmap)
{
    hmap->pairs = calloc(hmap->initial_size, sizeof(HashmapPair));
    hmap->pair_count = 0;
    hmap->pair_max = hmap->initial_size;
    return;
}
void hashmap_free(HashMap *hmap)
{
    return;
}

uint32_t hashmap_findindex(HashMap *hmap, char *key, unsigned int keylen)
{
    uint32_t index = hmap->hash(key, keylen) % hmap->pair_max;

    while(hmap->pairs[index].occupied == true && hmap->pairs[index].key != key)
    {
        index = (index + 1) % hmap->pair_max;
    }
    return index;
}

void hashmap_resize(HashMap *hmap)
{
    HashmapPair *oldmap = hmap->pairs;

    hmap->pair_max *= 2;
    hmap->pairs = calloc(hmap->pair_max, sizeof(HashmapPair));

    for (size_t i = 0; i < hmap->pair_max/2; i++)
    {
        uint32_t index = 0;
    }
}

HashmapReturn hashmap_insert(HashMap *hmap, char *key, unsigned int keylen, unsigned long long int val)
{
    if(hmap->pair_count > hmap->pair_max * hmap->load_factor)
        hashmap_resize(hmap);
    
    uint32_t index = hashmap_findindex(hmap, key, keylen);
    hmap->pairs[index].occupied = true;
    hmap->pairs[index].key = key;
    hmap->pairs[index].val = key;
}

HashmapReturn hashmap_delete(HashMap *hmap, char *key, unsigned int keylen)
{
    ;
}

HashmapReturn hashmap_lookup(HashMap *hmap, char *key, unsigned int keylen, unsigned long long int *val)
{
    uint32_t index = hashmap_findslot(hmap, key, keylen);
    hmap->pairs[index];
}