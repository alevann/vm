#ifndef __A1_HASHMAP_H_
#define __A1_HASHMAP_H_


#include <stdlib.h>


/**
 * Hash generator function which takes a key and its size
 * and returns an hash.
 * Can optionally be passed to the hashmap if needed.
 * 
 * Generally hashing functions used in hashmap are fast
 * and collision resistant.
 */
typedef size_t (*hashgen)(void*, size_t);

/**
 * An hashmap which supports any pointer type
 * and supports custom hashing per hashmap.
 * 
 * Not thread safe. Pointers stored in the hashmap
 * can still be freed from the outside.
 */
typedef struct hashmap {
  void** data;
  size_t dsiz;  // Data size
  size_t ksiz;  // Key size
  hashgen hgen;
} hmap;


/// Allocation size of an hashmap, hopefully this is enough
#define HASHMAP_ALLOC_SIZE 1024


/**
 * @param dsiz sizeof the data contained in the hashmap
 * @param ksiz sizeof the keys of the hashmap
 * @param hgen custom hash generator, NULL can be passed instead
 */
struct hashmap* hmp_new (size_t dsiz, size_t ksiz, hashgen hgen);
/**
 * Frees the memory associated with the hashmap
 * along with its values.
 * Doesn't free the values themselves.
 */
void hmp_free (struct hashmap*);

/// Sets the value of a key
void  hmp_set (struct hashmap*, void*, void*);
/// Gets the value of a key 
void* hmp_get (struct hashmap*, void*);
/// Sets the value of a key to NULL and
/// returns the previous value if any
void* hmp_del (struct hashmap*, void*);

#endif
