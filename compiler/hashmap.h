#ifndef __A1_HASHMAP_H_
#define __A1_HASHMAP_H_

#include <stdlib.h>
#include <stdint.h>
#include "string.h"


/**
 * A simple hashmap implementation which takes a string
 * as a key and stores a 32-bit unsigned integer
 */
typedef struct hashmap {
  uint8_t data [1024];
} hmap;

struct hashmap* hmp_new ();
uint8_t hmp_get (struct hashmap*, string*);
void hmp_set (struct hashmap*, string*, uint8_t);
uint8_t hmp_del (struct hashmap*, string*);

#endif
