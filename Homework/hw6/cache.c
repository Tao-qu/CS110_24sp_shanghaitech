#include "cache.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/* Create a cache simulator according to the config */
struct cache *cache_create(struct cache_config config, struct cache *lower_cache)
{
    /*YOUR CODE HERE*/

    struct cache *cache = (struct cache *)malloc(sizeof(struct cache));
    if (!cache)
    {
        return NULL;
    }

    cache->config = config;
    cache->lower_cache = lower_cache;

    cache->tag_bits = config.address_bits - __builtin_ctz(config.line_size) - __builtin_ctz(config.lines / config.ways);
    cache->tag_mask = (1 << cache->tag_bits) - 1;
    cache->index_bits = __builtin_ctz(config.lines / config.ways);
    cache->index_mask = (1 << cache->index_bits) - 1;
    cache->offset_bits = __builtin_ctz(config.line_size);
    cache->offset_mask = (1 << cache->offset_bits) - 1;

    cache->lines = (struct cache_line *)malloc(sizeof(struct cache_line) * config.lines);
    if (!cache->lines)
    {
        free(cache);
        return NULL;
    }

    for (uint32_t i = 0; i < config.lines; ++i)
    {
        cache->lines[i].valid = false;
        cache->lines[i].dirty = false;
        cache->lines[i].tag = 0;
        cache->lines[i].last_access = 0;
        cache->lines[i].data = (uint8_t *)malloc(config.line_size);
        if (!cache->lines[i].data)
        {
            for (uint32_t j = 0; j < i; ++j)
            {
                free(cache->lines[j].data);
            }
            free(cache->lines);
            free(cache);
            return NULL;
        }
        memset(cache->lines[i].data, 0, config.line_size);
    }

    return cache;
}

/*
Release the resources allocated for the cache simulator.
Also writeback dirty lines

The order in which lines are evicted is:
set0-slot0, set1-slot0, set2-slot0, (the 0th way)
set0-slot1, set1-slot1, set2-slot1, (the 1st way)
set0-slot2, set1-slot2, set2-slot2, (the 2nd way)
and so on.
*/
void cache_destroy(struct cache *cache)
{
    /*YOUR CODE HERE*/
    if (!cache)
    {
        return;
    }
    for (uint32_t i = 0; i < cache->config.lines; ++i)
    {
        if (cache->lines[i].dirty)
        {
            uint32_t addr = (cache->lines[i].tag << (cache->index_bits + cache->offset_bits)) | ((i % (1 << cache->index_bits)) << cache->offset_bits);
            if (cache->lower_cache)
            {
                for (uint32_t j = 0; j < cache->config.line_size; ++j)
                {
                    cache_write_byte(cache->lower_cache, addr + j, cache->lines[i].data[j]);
                }
            }
            else
            {
                mem_store(cache->lines[i].data, addr, cache->config.line_size);
            }
        }
        free(cache->lines[i].data);
    }
    free(cache->lines);
    free(cache);
    return;
}

/* Read one byte at a specific address. return hit=true/miss=false */
bool cache_read_byte(struct cache *cache, uint32_t addr, uint8_t *byte)
{
    /*YOUR CODE HERE*/
    uint32_t index = (addr >> cache->offset_bits) & cache->index_mask;
    uint32_t tag = (addr >> (cache->offset_bits + cache->index_bits)) & cache->tag_mask;
    uint32_t offset = addr & cache->offset_mask;
    for (uint32_t i = 0; i < cache->config.ways; ++i)
    {
        uint32_t line_index = i * (1 << cache->index_bits) + index;
        if (cache->lines[line_index].valid && cache->lines[line_index].tag == tag)
        {
            cache->lines[line_index].last_access = get_timestamp();
            if (byte)
            {
                *byte = cache->lines[line_index].data[offset];
            }
            return true;
        }
        if (!cache->lines[line_index].valid)
        {
            break;
        }
    }

    if (cache->lower_cache)
    {
        uint32_t victim = find_victim(cache, index);
        if (cache->lines[victim].dirty)
        {
            uint32_t addr_victim = (cache->lines[victim].tag << (cache->index_bits + cache->offset_bits)) | (index << cache->offset_bits);
            for (uint32_t i = 0; i < cache->config.line_size; ++i)
            {
                cache_write_byte(cache->lower_cache, addr_victim + i, cache->lines[victim].data[i]);
            }
        }
        cache_read_byte(cache->lower_cache, addr, NULL);
        memcpy(cache->lines[victim].data, cache->lower_cache->lines[find_line_index(cache->lower_cache, addr)].data, cache->config.line_size);
        cache->lines[victim].valid = true;
        cache->lines[victim].dirty = false;
        cache->lines[victim].tag = tag;
        cache->lines[victim].last_access = get_timestamp();
        if (byte)
        {
            *byte = cache->lines[victim].data[offset];
        }
        return false;
    }

    uint32_t victim = find_victim(cache, index);
    if (cache->lines[victim].dirty)
    {
        uint32_t addr_victim = (cache->lines[victim].tag << (cache->index_bits + cache->offset_bits)) | (index << cache->offset_bits);
        mem_store(cache->lines[victim].data, addr_victim, cache->config.line_size);
    }
    mem_load(cache->lines[victim].data, addr >> cache->offset_bits << cache->offset_bits, cache->config.line_size);
    cache->lines[victim].valid = true;
    cache->lines[victim].dirty = false;
    cache->lines[victim].tag = tag;
    cache->lines[victim].last_access = get_timestamp();
    if (byte)
    {
        *byte = cache->lines[victim].data[offset];
    }
    return false;
}

/* Write one byte into a specific address. return hit=true/miss=false*/
bool cache_write_byte(struct cache *cache, uint32_t addr, uint8_t byte)
{
    /*YOUR CODE HERE*/
    uint32_t index = (addr >> cache->offset_bits) & cache->index_mask;
    uint32_t tag = (addr >> (cache->offset_bits + cache->index_bits)) & cache->tag_mask;
    uint32_t offset = addr & cache->offset_mask;
    for (uint32_t i = 0; i < cache->config.ways; ++i)
    {
        uint32_t line_index = i * (1 << cache->index_bits) + index;
        if (cache->lines[line_index].valid && cache->lines[line_index].tag == tag)
        {
            cache->lines[line_index].last_access = get_timestamp();
            cache->lines[line_index].dirty = true;
            cache->lines[line_index].data[offset] = byte;
            if (!cache->config.write_back)
            {
                if (cache->lower_cache)
                {
                    cache_write_byte(cache->lower_cache, addr, byte);
                }
                else
                {
                    mem_store(cache->lines[line_index].data, addr >> cache->offset_bits << cache->offset_bits, cache->config.line_size);
                }
                cache->lines[line_index].dirty = false;
            }
            return true;
        }
        if (!cache->lines[line_index].valid)
        {
            break;
        }
    }

    cache_read_byte(cache, addr, NULL);
    cache_write_byte(cache, addr, byte);
    return false;
}

uint32_t find_victim(struct cache *cache, uint32_t index)
{
    uint32_t victim = index;
    uint64_t min_timestamp = cache->lines[index].last_access;
    for (uint32_t i = 0; i < cache->config.ways; ++i)
    {
        uint32_t line_index = i * (1 << cache->index_bits) + index;
        if (!cache->lines[line_index].valid)
        {
            return line_index;
        }
        if (cache->lines[line_index].last_access < min_timestamp)
        {
            min_timestamp = cache->lines[line_index].last_access;
            victim = line_index;
        }
    }
    return victim;
}

uint32_t find_line_index(struct cache *cache, uint32_t addr)
{
    uint32_t index = (addr >> cache->offset_bits) & cache->index_mask;
    uint32_t tag = (addr >> (cache->offset_bits + cache->index_bits)) & cache->tag_mask;
    for (uint32_t i = 0; i < cache->config.ways; ++i)
    {
        uint32_t line_index = i * (1 << cache->index_bits) + index;
        if (cache->lines[line_index].valid && cache->lines[line_index].tag == tag)
        {
            return line_index;
        }
    }
    return -1;
}