#ifndef __A1_A1MEM_H_
#define __A1_A1MEM_H_

// 32 MB of memory if I'm not an idiot (god I hope not)
#define A1_MEMORY_SIZE 8 * 1024 * 1024 * 32

typedef struct memory {
  char data[A1_MEMORY_SIZE];
} mem;

#endif
