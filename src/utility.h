#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct Buffer
{
    void *data;
    size_t size;
} Buffer;

typedef struct MemoryPool
{
    unsigned char *block;
    bool *blockused;
    int *block_alloc_length;
    unsigned char *temp;
    size_t size;
    int block_size;
} MemoryPool;
void memorypool_initialize(MemoryPool *mpool, int initial_size, int block_size);
void memorypool_clear(MemoryPool *mpool);
void *memorypool_allocate_block(MemoryPool *mpool, int count);
void memorypool_free_block(MemoryPool *mpool, void *mem);
void memorypool_free(MemoryPool *mpool);

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
void dynamicarray_clear(DynamicArray *dynarr);
void *dynamicarray_append(DynamicArray *dynarr, void *el);
void *dynamicarray_new_element(DynamicArray *dynarr);
void *dynamicarray_new_element_array(DynamicArray *dynarr, unsigned int num);
void *dynamicarray_get_element(DynamicArray *dynarr, unsigned int element);
void dynamicarray_insert(DynamicArray *dynarr, int index, void *data);
void dynamicarray_delete(DynamicArray *dynarr, int index);
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
void *stack_peek(Stack *stack);
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
typedef enum HashmapResult
{
    HASHMAP_SUCCESS,
    HASHMAP_DUPLICATE,
    HASHMAP_NOT_FOUND
} HashmapResult;
//TODO: restructure this to have better cache locality maybe force key member to be static length
typedef struct HashmapPair
{
    uint64_t hash;
    bool occupied;
} HashmapPair;
typedef struct HashMap
{
    float load_factor;
    unsigned char *data;
    size_t pair_count;
    size_t pair_max;
    uint32_t elsize;
    HashFunc hash;
} HashMap;

//0.75 is a good number for load_factor
void hashmap_initialize(HashMap *hmap, int initial_size, uint32_t elsize, float load_factor);
void hashmap_free(HashMap *hmap);
//note this breaks references to the data contained
void hashmap_resize(HashMap *hmap);
HashmapResult hashmap_insert(HashMap *hmap, void *key, uint32_t keylen, void *val);
HashmapResult hashmap_insert_prehashed(HashMap *hmap, uint64_t hash, void *val);
HashmapResult hashmap_delete(HashMap *hmap, void *key, uint32_t keylen);
HashmapResult hashmap_delete_prehashed(HashMap *hmap, uint64_t hash);
//note you can pass null to val to just see if the object does exist
HashmapResult hashmap_lookup(HashMap *hmap, void *key, uint32_t keylen, void **val);
HashmapResult hashmap_lookup_prehashed(HashMap *hmap, uint64_t hash, void **val);
//treats the data stored as a pointer and writes it directly to the pointer provided
HashmapResult hashmap_lookup_copy(HashMap *hmap, void *key, uint32_t keylen, void **val);
void hashmap_map(HashMap *hmap, void *func);