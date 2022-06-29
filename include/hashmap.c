#include <stdint.h>
#include <stdio.h>
#include "hashmap.h"


size_t hash (struct hashkey* key)
{
  uint8_t hash, i;
  
  for (hash = i = 0; i < key->size; ++i)
  {
    hash += ((char*) key->data)[i];
    hash += (hash << 10);
    hash ^= (hash >>  6);
  }
  
  hash += (hash <<  3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

size_t hmp_idx (struct hashmap* hmp, struct hashkey* key)
{
  return (*hmp->hgen)(key) % HASHMAP_ALLOC_SIZE;
}


struct hashmap* hmp_new (size_t size, hashgen hgen)
{
  struct hashmap* hmp = malloc(sizeof(struct hashmap));
  hmp->data = malloc(size * HASHMAP_ALLOC_SIZE);
  hmp->size = size;
  hmp->hgen = hgen;
  return hmp;
}

struct hashmap* hmp_new (size_t size)
{
  return hmp_new(size, hash);
}

void* hmp_get (struct hashmap* hmp, struct hashkey* key)
{
  return hmp->data[hmp_idx(hmp, key)];
}

void  hmp_set (struct hashmap* hmp, struct hashkey* key, void* data)
{
  hmp->data[hmp_idx(hmp, key)] = data;
}

void* hmp_del (struct hashmap* hmp, struct hashkey* key)
{
  size_t idx = hmp_idx(hmp, key);
  void* data = hmp->data[idx];

  hmp->data[idx] = NULL;
  
  return data;
}
