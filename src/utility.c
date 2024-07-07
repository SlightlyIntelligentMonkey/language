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
    dynarr->maxsize = initial_size;
    dynarr->data = calloc(dynarr->maxsize, dynarr->elsize);
}
void dynamicarray_resize(DynamicArray *dynarr, unsigned int newsize)
{
    if(newsize > dynarr->maxsize)
    {
        dynamicarray_reserve(dynarr, newsize);
    }
}
void dynamicarray_reserve(DynamicArray *dynarr, unsigned int newsize)
{
    dynarr->data = realloc(dynarr->data, newsize*dynarr->elsize);
    dynarr->maxsize = newsize;
}
void dynamicarray_clear(DynamicArray *dynarr)
{
    dynarr->used = 0;
}
void *dynamicarray_append(DynamicArray *dynarr, void *el)
{
    //resize the dynamic array if needed
    if(dynarr->used >= dynarr->maxsize)
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
    if(dynarr->used >= dynarr->maxsize)
    {
        dynamicarray_resize(dynarr, dynarr->maxsize*2);
    }
    return dynarr->data + dynarr->used*dynarr->elsize;
}
void *dynamicarray_get_element(DynamicArray *dynarr, unsigned int element)
{
    if(element < dynarr->used)
        return dynarr->data + element*dynarr->elsize;
    else
        return NULL;
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
void *stack_peek(Stack *stack)
{
    return stack->sp;
}
void stack_free(Stack *stack)
{
    free(stack->data);
}

void hashmap_initialize(HashMap *hmap, int initial_size, uint32_t elsize, float load_factor)
{
    hmap->elsize = elsize;
    hmap->pairs = calloc(initial_size, sizeof(HashmapPair) + hmap->elsize);
    hmap->pair_count = 0;
    hmap->pair_max = initial_size;
    hmap->load_factor = load_factor;
    hmap->hash = FNV_hash;
}
void hashmap_free(HashMap *hmap)
{
    if(hmap->data != NULL)
        free(hmap->data);
    free(hmap->pairs);
}

//TODO: force this to be powers of two for faster "modulo" behavior
uint32_t hashmap_findindex(void *data, void *key, uint32_t keylen, HashmapPair **pair)
{
    uint32_t index = hmap->hash(key, keylen) % hmap->pair_max;
    *pair = (HashmapPair *)(data + (index * (sizeof(HashmapPair) + hmap->elsize)));
    while(pair->occupied == true)
    {
        if(pair->keylen == keylen && memcmp(pair->key, key, keylen))
            break;
        index = (index + 1) % hmap->pair_max;
        *pair = (HashmapPair *)(data + (index * (sizeof(HashmapPair) + hmap->elsize)));
    }
    return index;
}

void hashmap_resize(HashMap *hmap)
{
    unsigned char *oldata = hmap->data;

    hmap->pair_max *= 2;
    hmap->data = calloc(hmap->pair_max, sizeof(HashmapPair) + hmap->elsize);

    //how annoying, to resize a hashmap we have to reinsert call of the pairs afterwards
    //there is probably a better way to do this
    for (size_t i = 0; i < hmap->pair_max/2; i++)
    {
        HashmapPair *oldpair = hashmap_getpair(oldata, i);
        if(oldpair->occupied == true)
            hashmap_insert(hmap, oldpair->key, oldpair->keylen, (oldpair + sizeof(HashmapPair)));
    }
    free(oldata);
}

//TODO: probably tell you if it found a duplicate and overwrote it
HashmapReturn hashmap_insert(HashMap *hmap, void *key, uint32_t keylen, void *val)
{
    if(hmap->pair_count > hmap->pair_max * hmap->load_factor)
        hashmap_resize(hmap);

    HashmapPair *pair;
    uint32_t index = hashmap_findindex(hmap->data, key, keylen, &pair);
    if(pair->occupied != true)
    {
        pair->occupied = true;
        pair->key = key;
        pair->keylen = keylen;
        hmap->pair_count += 1;
    }
    
    memcpy(pair + sizeof(HashmapPair), val, hmap->elsize);
    return HASHMAP_SUCCESS;
}

HashmapReturn hashmap_delete(HashMap *hmap, void *key, uint32_t keylen)
{
    HashmapPair *pair;
    uint32_t index = hashmap_findindex(hmap->data, key, keylen, &pair);
    if(pair->occupied == false)
        return HASHMAP_NOT_FOUND;
    pair->occupied = false;
    hmap->pair_count -= 1;
    return HASHMAP_SUCCESS;
}

HashmapReturn hashmap_lookup(HashMap *hmap, void *key, uint32_t keylen, void **val)
{
    HashmapPair *pair;
    uint32_t index = hashmap_findindex(hmap->data, key, keylen, &pair);
    if(pair->occupied == false)
        return HASHMAP_NOT_FOUND;
    if(val != NULL)
        *val = pair + sizeof(HashmapPair);
    return HASHMAP_SUCCESS;
}