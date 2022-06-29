#ifndef __A1_HASHMAP_H_
#define __A1_HASHMAP_H_


#include <stdlib.h>


/**
 * Hash generator function which takes a key and returns an hash.
 * Can optionally be passed to the hashmap if needed.
 */
typedef size_t (*hashgen)(struct hashkey*);

/**
 * An hashmap which supports any pointer type
 * and supports custom hashing per hashmap.
 */
typedef struct hashmap {
  void** data;
  size_t size;
  hashgen hgen;
} hmap;

typedef struct hashkey {
  void* data;
  size_t size;
} hkey;


#define HKEY(k) (&((hkey) { .data = k, .size = sizeof(k) - 1 }))
#define HASHMAP_ALLOC_SIZE 1024

struct hashmap* hmp_new (size_t, hashgen);
void  hmp_set (struct hashmap*, struct hashkey*, void*);
void* hmp_get (struct hashmap*, struct hashkey*);
void* hmp_del (struct hashmap*, struct hashkey*);

#endif
