#include "hashmap.h"

#include "ANSI.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool   HASHMAP_DEBUG_MODE = false;
size_t HASHMAP_COLLISIONS = 0;

const size_t BUCKET_MAX_SIZE = 10;

size_t hash_function(char *string, size_t mod)
{
	size_t result = 0;

	unsigned char c;
	unsigned i = 0;

	while ((c = string[i]) != '\0')
	{
		result = (result * 31 + c) % mod;
		i++;
	}

	return result;
}

hashmap_t *hashmap_construct(size_t mod)
{
	hashmap_t *hashmap = (hashmap_t *)malloc(sizeof(hashmap_t));

	hashmap->mod = mod;

	hashmap->buckets = (bucket_t *)malloc(sizeof(bucket_t) * mod);

	for (size_t m = 0; m < mod; m++)
	{
		bucket_t *bucket = &hashmap->buckets[m];
		bucket->pairs = (pair_t *)malloc(sizeof(pair_t) * BUCKET_MAX_SIZE);
		bucket->pair_count = 0;
	}

	return hashmap;
}

void hashmap_set(hashmap_t *hashmap, char *key, void *value)
{
	size_t hash = hash_function(key, hashmap->mod);
	bucket_t *bucket = &hashmap->buckets[hash];

	size_t i;
	for (i = 0; i < bucket->pair_count; i++)
	{
		pair_t *pair = &bucket->pairs[i];
		if (strcmp(key, pair->key) == 0)
		{
			pair->value = value;
			if (HASHMAP_DEBUG_MODE)
				printf(ANSI_GREEN "[%p] \"%s\" modified in hashmap.\n" ANSI_OFF, hashmap, key);
			return;
		}
	}

	if (i < BUCKET_MAX_SIZE)
	{
		pair_t *pair = &bucket->pairs[i];
		pair->key = key;
		pair->value = value;
		bucket->pair_count++;
		if (HASHMAP_DEBUG_MODE)
			printf(ANSI_GREEN "[%p] \"%s\" added to hashmap.\n" ANSI_OFF, hashmap, key);
	}
	else
	{
		printf("Bucket is full...\n");
	}

	if (bucket->pair_count > 1)
	{
		if (HASHMAP_DEBUG_MODE)
			printf(ANSI_MAGENTA "[%p][Bucket: %lu, Size: %lu] Collision occurred.\n" ANSI_OFF, hashmap, hash, bucket->pair_count);
		HASHMAP_COLLISIONS++;
	}
}

void *hashmap_get(hashmap_t *hashmap, char *key)
{
	size_t hash = hash_function(key, hashmap->mod);
	bucket_t *bucket = &hashmap->buckets[hash];

	for (size_t i = 0; i < bucket->pair_count; i++)
	{
		pair_t *pair = &bucket->pairs[i];
		if (strcmp(key, pair->key) == 0)
		{
			if (HASHMAP_DEBUG_MODE)
				printf(ANSI_YELLOW "[%p] \"%s\" found in hashmap.\n" ANSI_OFF, hashmap, key);
			return pair->value;
		}
	}

	if (HASHMAP_DEBUG_MODE)
		printf(ANSI_YELLOW "[%p] \"%s\" not found in hashmap.\n" ANSI_OFF, hashmap, key);
	return NULL;
}

void hashmap_destruct(hashmap_t *hashmap)
{
	for (size_t m = 0; m < hashmap->mod; m++)
	{
		free(hashmap->buckets[m].pairs);
	}
	free(hashmap->buckets);
	free(hashmap);
}
