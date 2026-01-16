#pragma once
#include <stddef.h>

#if defined(__x86_64__) || defined(_M_X64)
#include "includes/arch/x86_64/io.h"
#endif

typedef struct cache_t
{

    list_t full, partial, free;
    size_t obj_size;
    size_t obj_per_slab;
    size_t total_objs;
} cache_t;

typedef struct slab_t
{
    list_t slabs;
    cache_t* cache;
    virt_addr_t addr;
    size_t free_idx;
} slab_t;

virt_addr_t cache_alloc(cache_t *cache);
void init_cache(cache_t *cache, size_t obj_size);
cache_t *new_cache(cache_t *main_cache, size_t obj_size);