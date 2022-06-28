#include <stdio.h>
#include "hashmap.h"

uint8_t joaath (string* str)
{
    uint8_t hash, i;
    for(hash = i = 0; i < str->length; ++i)
    {
        hash += str->data[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

struct hashmap* hmp_new ()
{
  return malloc(sizeof(struct hashmap));
}

void hmp_set (struct hashmap* hmp, string* key, uint8_t data)
{
  uint8_t idx = joaath(key) % 1024;
  hmp->data[idx] = data;
}

uint8_t hmp_get (struct hashmap* hmp, string* key)
{
  uint8_t idx = joaath(key) % 1024;
  return hmp->data[idx];
}

uint8_t hmp_del (struct hashmap* hmp, string* key)
{
  uint8_t idx = joaath(key) % 1024;
  uint8_t dat = hmp->data[idx];
  hmp->data[idx] = 0;
  return dat;
}
