#pragma once

#include <stdbool.h>
#include <stddef.h>

extern bool   HASHMAP_DEBUG_MODE;
extern size_t HASHMAP_COLLISIONS;

typedef struct
{
	char *key;
	void *value;
} pair_t;

typedef struct
{
	pair_t *pairs;
	size_t pair_count;
} bucket_t;

typedef struct
{
	bucket_t *buckets;
	size_t mod;
} hashmap_t;

size_t hash(char *string, size_t mod);

hashmap_t *hashmap_construct(size_t mod);
void       hashmap_set      (hashmap_t *hashmap, char *key, void *value);
void      *hashmap_get      (hashmap_t *hashmap, char *key);
void       hashmap_destruct (hashmap_t *hashmap);
