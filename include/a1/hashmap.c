#include <stdint.h>
#include <stdio.h>
#include "hashmap.h"


/// Implementation of the default hash function
/// Jenkins one-at-a-time hashing algoritm
size_t hash (void* key, size_t size)
{
  uint8_t hash, i;
  
  for (hash = i = 0; i < size; ++i)
  {
    hash += ((char*) key)[i];
    hash += (hash << 10);
    hash ^= (hash >>  6);
  }
  
  hash += (hash <<  3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

/// Returns an index given a key
size_t hmp_idx (struct hashmap* hmp, void* key)
{
  return (*hmp->hgen)(key, hmp->ksiz) % HASHMAP_ALLOC_SIZE;
}


struct hashmap* hmp_new (size_t dsiz, size_t ksiz, hashgen hgen)
{
  struct hashmap* hmp = malloc(sizeof(struct hashmap));
  hmp->data = malloc(dsiz * HASHMAP_ALLOC_SIZE);
  hmp->keys = list_new();
  hmp->dsiz = dsiz;
  hmp->ksiz = ksiz;
  hmp->hgen = hgen == NULL ? hash : hgen;
  return hmp;
}

void hmp_free (struct hashmap* hmp)
{
  free(hmp->data);
  free(hmp);
}

void* hmp_get (struct hashmap* hmp, void* key)
{
  return hmp->data[hmp_idx(hmp, key)];
}

void  hmp_set (struct hashmap* hmp, void* key, void* data)
{
  hmp->data[hmp_idx(hmp, key)] = data;
  list_add(hmp->keys, key);
}

void* hmp_del (struct hashmap* hmp, void* key)
{
  size_t idx = hmp_idx(hmp, key);
  void* data = hmp->data[idx];

  hmp->data[idx] = NULL;

  // FIXME: not all data can be compared with identity
  list_del(hmp->keys, key, identity);
  
  return data;
}

uint8_t hmp_has (struct hashmap* hmp, void* key)
{
  return hmp_contains(hmp, key, identity);
}

uint8_t hmp_contains (struct hashmap* hmp, void* key, comparator cmp)
{
  return list_contains(hmp->keys, key, cmp);
}
