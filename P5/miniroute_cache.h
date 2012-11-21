#ifndef __CACHE_H__
#define __miniroute_cache_H__

#include "miniroute_cache_private.h"
#include "miniroute.h"

/* Data structure to record a path */
typedef struct miniroute_path *miniroute_path_t;
typedef struct miniroute_item *miniroute_item_t;

typedef struct miniroute_cache *miniroute_cache_t;

/* Create a cache with size as table size, return NULL if fails */
extern miniroute_cache_t miniroute_cache_new(size_t size);

/*
 * Get an item from cache
 * Return -1 if the destination is not found
 * Return 0 if the destination is found and put it in item
 */
extern int miniroute_cache_get_by_dest(miniroute_cache_t cache, char dest[],
                                       void **it);

/*
 * Get an item with network address from cache
 * Return -1 if not found
 * Return 0 if found and put it in item
 */
extern int miniroute_cache_get_by_addr(miniroute_cache_t cache,
                                       network_address_t addr, void **it);

/*
 * Put an item into cache
 * If the cache is full, find a victim to evict
 * Return 0 on success, -1 on failure
 */
extern int miniroute_cache_put_item(miniroute_cache_t cache, void *item);

/*
 * Put an item into cache by header
 * If the cache is full, find a victim to evict
 * Return 0 on success, -1 on failure
 */
extern int miniroute_cache_put_path_from_hdr(miniroute_cache_t cache,
        miniroute_header_t header);

/*
 * Delete an item from cache
 * Return 0 on success, -1 on failure
 */
extern int miniroute_cache_delete_item(miniroute_cache_t cache, void *item);

/* Set the maximum item number of cache */
extern void miniroute_cache_set_max_num(miniroute_cache_t cache, int num);

/* If a item has expired. Return -1 if not, 0 if yes */
extern int miniroute_cache_is_expired(miniroute_item_t item);

/* Destroy a cache */
extern void miniroute_cache_destroy(miniroute_cache_t cache);

/*
 * Construct a new miniroute path from a header.
 * The cached route reverses the path in the header.
 */
extern miniroute_path_t miniroute_path_from_hdr(miniroute_header_t header);

/* Print whole cache with path as items, for debugging */
extern void miniroute_cache_print_path(miniroute_cache_t cache);

#endif
