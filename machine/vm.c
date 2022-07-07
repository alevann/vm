#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "registers.h"

#define MEMORY_SIZE 8 * 1024 * 1024 * 32

struct mm {
  uint8_t data[MEMORY_SIZE];
  uint32_t pgend;
};

struct vm {
  struct mm* memory;
  registers* registers;
  size_t pc;
};

struct vm* vm_new ()
{
  struct vm* vm = malloc(sizeof(struct vm));
  vm->memory = malloc(sizeof(struct mm));
  vm->memory->pgend = 0;
  vm->registers = reg_new();
  vm->pc = 0;
  return vm;
}

void vm_load (struct vm* vm, FILE* file)
{
  uint32_t i = 0;
  uint8_t c = 0;
  
  // Read the file byte by byte into memory
  while ((c = fgetc(file)) != EOF)
    vm->memory->data[i++] = c;
  
  // Save where the file ends in memory
  // and therefore where "free" memory begins
  vm->memory->pgend = i;
}
