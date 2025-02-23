#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "utility.h"


void memorypool_initialize(MemoryPool *mpool, int initial_size, int block_size)
{
    mpool->block_size = block_size;
    mpool->size = initial_size;
    mpool->block = malloc(initial_size * block_size);
    mpool->temp = malloc(initial_size * block_size);
    mpool->blockused = calloc(initial_size, 1);
    mpool->block_alloc_length = calloc(initial_size, sizeof(int));
}
void memorypool_clear(MemoryPool *mpool)
{
    memset(mpool->block, 0, mpool->block_size*mpool->size);
    memset(mpool->blockused, false, mpool->size*sizeof(bool));
    memset(mpool->block_alloc_length, 0, mpool->size*sizeof(int));
}
void memorypool_set_used(MemoryPool *mpool, int block, int count)
{
    mpool->block_alloc_length[block] = count;
    for (int i = block; i < block + count; i++)
    {
        mpool->blockused[i] = true;
    }
}
void *memorypool_allocate_block(MemoryPool *mpool, int count)
{
    int i = 0, current = 0;
    while(i < mpool->size)
    {
        int unusedlength = 0;
        for (int j = i; j < mpool->size; j++)
        {
            unusedlength++;
            if(mpool->blockused[j])
                break;
            if(unusedlength >= count)
            {
                memorypool_set_used(mpool, i, count);
                return (void*)&mpool->block[i * mpool->block_size];
            }
        }
        //TODO this might be wrong
        i += unusedlength+1;
    }
    
    memcpy(mpool->temp, mpool->block, mpool->size * mpool->block_size);
    free(mpool->block);
    mpool->block = malloc(mpool->size * mpool->block_size * 2);
    memcpy(mpool->block, mpool->temp, mpool->size * mpool->block_size);

    memcpy(mpool->temp, mpool->blockused, mpool->size);
    free(mpool->blockused);
    mpool->blockused = malloc(mpool->size * 2);
    memcpy(mpool->blockused, mpool->temp, mpool->size);
    
    memcpy(mpool->temp, mpool->block_alloc_length, mpool->size * sizeof(int));
    free(mpool->block_alloc_length);
    mpool->block_alloc_length = malloc(mpool->size * sizeof(int) * 2);
    memcpy(mpool->block_alloc_length, mpool->temp, mpool->size * sizeof(int));
    
    memset(&mpool->blockused[mpool->size], 0, mpool->size);
    memset(&mpool->block_alloc_length[mpool->size], 0, (mpool->size) * sizeof(int));
    mpool->size *= 2;
    free(mpool->temp);
    mpool->temp = malloc(mpool->size * mpool->block_size);

    memorypool_set_used(mpool, i, count);
    return (void*)&mpool->block[i * mpool->block_size];
}
void memorypool_free_block(MemoryPool *mpool, void *mem)
{
    int block = ((unsigned char*)mem - mpool->block)/mpool->block_size;
    int alloc_length = mpool->block_alloc_length[block];
    for (int i = block; i < block + alloc_length; i++)
    {
        mpool->block_alloc_length[i] = 0;
        mpool->blockused[i] = false;
    }
}
void memorypool_free(MemoryPool *mpool)
{
    free(mpool->block);
    free(mpool->blockused);
    free(mpool->block_alloc_length);
}

/* DYNAMIC ARRAY CODe */
void dynamicarray_initialize(DynamicArray *dynarr, unsigned int element_size, unsigned int initial_size)
{
    dynarr->elsize = element_size;
    dynarr->used = 0;
    dynarr->data = calloc(initial_size, dynarr->elsize);
    dynarr->maxsize = initial_size;
}
void dynamicarray_resize(DynamicArray *dynarr, unsigned int newsize)
{
    if (newsize > dynarr->maxsize)
    {
        dynamicarray_reserve(dynarr, newsize);
    }
}
void dynamicarray_reserve(DynamicArray *dynarr, unsigned int newsize)
{
    uint64_t msize = dynarr->maxsize * dynarr->elsize;
    unsigned char *temp = malloc(msize);
    memcpy(temp, dynarr->data, msize);
    free(dynarr->data);
    dynarr->data = malloc(newsize * dynarr->elsize);
    memcpy(dynarr->data, temp, msize);
    memset(&dynarr->data[msize], 0, msize);
    free(temp);
    //realloc(dynarr->data, newsize * dynarr->elsize);
    dynarr->maxsize = newsize;
}
void dynamicarray_clear(DynamicArray *dynarr)
{
    dynarr->used = 0;
}
void *dynamicarray_append(DynamicArray *dynarr, void *el)
{
    // resize the dynamic array if needed
    if (dynarr->used >= dynarr->maxsize)
    {
        dynamicarray_resize(dynarr, dynarr->maxsize * 2);
    }
    uintptr_t index = (uintptr_t)dynarr->data + dynarr->elsize * dynarr->used;
    dynarr->used++;
    memcpy((char *)index, el, dynarr->elsize);
    return (char *)index;
}
void *dynamicarray_new_element(DynamicArray *dynarr)
{
    if (dynarr->used >= dynarr->maxsize)
    {
        dynamicarray_resize(dynarr, dynarr->maxsize * 2);
    }
    uintptr_t address = (uintptr_t)dynarr->data + dynarr->used * dynarr->elsize;
    dynarr->used++;
    return (char *)address;
}
void *dynamicarray_new_element_array(DynamicArray *dynarr, unsigned int num)
{
    dynarr->used += num;
    if (dynarr->used >= dynarr->maxsize)
    {
        dynamicarray_resize(dynarr, dynarr->maxsize * 2);
    }
    uintptr_t address = (uintptr_t)dynarr->data + dynarr->used * dynarr->elsize;
    dynarr->used += num;
    return (char *)address;
}
void *dynamicarray_get_element(DynamicArray *dynarr, unsigned int element)
{
    if (element < dynarr->used)
        return (char *)((uintptr_t)dynarr->data + element * dynarr->elsize);
    else
        return NULL;
}
void dynamicarray_insert(DynamicArray *dynarr, int index, void *data)
{
    //idiocy check
    //if(index > dynarr->used)
    dynarr->used += 1;
    if (dynarr->used >= dynarr->maxsize)
    {
        dynamicarray_resize(dynarr, dynarr->maxsize * 2);
    }
    uintptr_t address = (uintptr_t)dynarr->data + index * dynarr->elsize;
    memcpy((char*)(address + dynarr->elsize), (char*)address, (dynarr->used - index - 1)*dynarr->elsize);
}
void dynamicarray_delete(DynamicArray *dynarr, int index)
{
    char *buffer = calloc((dynarr->used - index), dynarr->elsize);
    memcpy(buffer, dynarr->data + (index + 1)*dynarr->elsize, (dynarr->used - index - 1)*dynarr->elsize);
    memcpy(dynarr->data + index*dynarr->elsize, buffer, (dynarr->used - index - 1)*dynarr->elsize);
    free(buffer);
    dynarr->used -= 1;
}
void dynamicarray_free(DynamicArray *dynarr)
{
    free(dynarr->data);
}

/* STACK CODE */
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

/* HASHMAP CODE */
void hashmap_initialize(HashMap *hmap, int initial_size, uint32_t elsize, float load_factor)
{
    hmap->elsize = elsize;
    hmap->data = calloc(initial_size, sizeof(HashmapPair) + hmap->elsize);
    hmap->pair_count = 0;
    hmap->pair_max = initial_size;
    hmap->load_factor = load_factor;
    hmap->hash = FNV_hash;
}
void hashmap_free(HashMap *hmap)
{
    if (hmap->data != NULL)
        free(hmap->data);
}

// TODO: force this to be powers of two for faster "modulo" behavior
static inline uint32_t hashmap_findindex(HashmapPair **pair, uint64_t *rethash, HashMap *hmap, void *key, uint32_t keylen)
{
    uint64_t hash = hmap->hash(key, keylen);
    uint32_t index = hash % hmap->pair_max;
    *pair = (HashmapPair *)(hmap->data + (index * (sizeof(HashmapPair) + hmap->elsize)));
    while ((*pair)->occupied == true)
    {
        if ((*pair)->hash == hash)
            break;
        index = (index + 1) % hmap->pair_max;
        *pair = (HashmapPair *)(hmap->data + (index * (sizeof(HashmapPair) + hmap->elsize)));
    }
    if (rethash != NULL)
        *rethash = hash;
    return index;
}

static inline uint32_t hashmap_findindex_prehashed(HashmapPair **pair, HashMap *hmap, uint64_t hash)
{
    uint32_t index = hash % hmap->pair_max;
    *pair = (HashmapPair *)(hmap->data + (index * (sizeof(HashmapPair) + hmap->elsize)));
    while ((*pair)->occupied == true)
    {
        if ((*pair)->hash == hash)
            break;
        index = (index + 1) % hmap->pair_max;
        *pair = (HashmapPair *)(hmap->data + (index * (sizeof(HashmapPair) + hmap->elsize)));
    }
    return index;
}

void hashmap_resize(HashMap *hmap)
{
    unsigned char *oldata = hmap->data;

    hmap->pair_max *= 2;
    hmap->pair_count = 0;
    hmap->data = calloc(hmap->pair_max, sizeof(HashmapPair) + hmap->elsize);

    // how annoying, to resize a hashmap we have to reinsert call of the pairs afterwards
    // there is probably a better way to do this
    for (size_t i = 0; i < hmap->pair_max / 2; i++)
    {
        HashmapPair *oldpair = (HashmapPair *)(oldata + (i * (sizeof(HashmapPair) + hmap->elsize)));
        if (oldpair->occupied != false)
            hashmap_insert_prehashed(hmap, oldpair->hash, ((unsigned char*)oldpair + sizeof(HashmapPair)));
    }
    free(oldata);
}

// TODO: probably tell you if it found a duplicate and overwrote it
HashmapResult hashmap_insert(HashMap *hmap, void *key, uint32_t keylen, void *val)
{
    if (hmap->pair_count > hmap->pair_max * hmap->load_factor)
        hashmap_resize(hmap);

    HashmapPair *pair;
    uint64_t hash;
    hashmap_findindex(&pair, &hash, hmap, key, keylen);
    if (pair->occupied == false)
    {
        pair->occupied = true;
        pair->hash = hash;
        hmap->pair_count += 1;
    }

    memcpy(pair + 1, val, hmap->elsize);
    return HASHMAP_SUCCESS;
}
HashmapResult hashmap_insert_prehashed(HashMap *hmap, uint64_t hash, void *val)
{
    if (hmap->pair_count > hmap->pair_max * hmap->load_factor)
        hashmap_resize(hmap);

    HashmapPair *pair;
    hashmap_findindex_prehashed(&pair, hmap, hash);
    if (pair->occupied == false)
    {
        pair->occupied = true;
        pair->hash = hash;
        hmap->pair_count += 1;
    }

    memcpy(pair + 1, val, hmap->elsize);
    return HASHMAP_SUCCESS;
}

HashmapResult hashmap_delete(HashMap *hmap, void *key, uint32_t keylen)
{
    HashmapPair *pair;
    hashmap_findindex(&pair, NULL, hmap, key, keylen);
    if (pair->occupied == false)
        return HASHMAP_NOT_FOUND;
    pair->occupied = false;
    hmap->pair_count -= 1;
    return HASHMAP_SUCCESS;
}

HashmapResult hashmap_delete_prehashed(HashMap *hmap, uint64_t hash)
{
    HashmapPair *pair;
    hashmap_findindex_prehashed(&pair, hmap, hash);
    if (pair->occupied == false)
        return HASHMAP_NOT_FOUND;
    pair->occupied = false;
    hmap->pair_count -= 1;
    return HASHMAP_SUCCESS;
}

HashmapResult hashmap_lookup(HashMap *hmap, void *key, uint32_t keylen, void **val)
{
    HashmapPair *pair;
    hashmap_findindex(&pair, NULL, hmap, key, keylen);
    if (pair->occupied == false)
        return HASHMAP_NOT_FOUND;
    if (val != NULL)
        *val = pair + 1;
    return HASHMAP_SUCCESS;
}
HashmapResult hashmap_lookup_prehashed(HashMap *hmap, uint64_t hash, void **val)
{
    HashmapPair *pair;
    hashmap_findindex_prehashed(&pair, hmap, hash);
    if (pair->occupied == false)
        return HASHMAP_NOT_FOUND;
    if (val != NULL)
        *val = pair + 1;
    return HASHMAP_SUCCESS;
}
HashmapResult hashmap_lookup_copy(HashMap *hmap, void *key, uint32_t keylen, void **val)
{
    HashmapPair *pair;
    hashmap_findindex(&pair, NULL, hmap, key, keylen);
    if (pair->occupied == false)
        return HASHMAP_NOT_FOUND;
    if (val != NULL)
        memcpy(val, pair + 1, sizeof(val));
    return HASHMAP_SUCCESS;
}