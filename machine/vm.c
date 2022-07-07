#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "registers.h"
#include "a1/op.h"
#include "a1/string.h"

#define MEMORY_SIZE 1024 * 1024 * 32

struct mm {
  uint8_t data[MEMORY_SIZE];
  uint32_t pgend;
};

struct vm {
  struct mm* memory;
  registers* registers;
  size_t pc;
  uint8_t ci;
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
  int c = 0;
  
  // Read the file byte by byte into memory
  while ((c = fgetc(file)) != EOF)
    vm->memory->data[i++] = c;

  // Save where the file ends in memory
  // and therefore where "free" memory begins
  vm->memory->pgend = i;
}

void dbg_log_vm (struct vm* vm)
{
  printf("Virtual machine state:\n");
  printf("\tCurrent instruction: %s (%d)\n", OP_STR[vm->ci], vm->ci);
  printf("\tCurrent execution offset: %d\n\t", vm->pc);
  dbg_log_registers(vm->registers);
  printf("\tMemory size: %d\n", MEMORY_SIZE);
  printf("\tROM end offset: %d\n", vm->memory->pgend);
}

void vm_err (struct vm* vm)
{
  printf("\n");
  dbg_log_vm(vm);
  printf("Program terminated with exit code 1\n");
  exit(0);
}

void vm_end (struct vm* vm)
{
  printf("\n");
  dbg_log_vm(vm);
  printf("Program terminated with exit code 0\n");
  exit(0);
}

string* gtkn (struct vm* vm)
{
  // Get a pointer to the beginning of the token
  string* str = str_new(2);
  
  str_addc(str, vm->memory->data[vm->pc++]);
  str_addc(str, '\0');

  return str;
}

// Returns either the value of the register if the string
// is a register or the number it represents (hoping) it
// represents a number lmao
uint32_t reg_or_literal (registers* r, char* str)
{
  if (reg_isreg(str))
    return REG(r, str);
  else
    return atoi(str);
}

void vm_set (struct vm* vm)
{
  string* lhs = gtkn(vm);
  string* rhs = gtkn(vm);

  REG(vm->registers, lhs->data) = reg_or_literal(vm->registers, rhs->data);

  str_free(lhs);
  str_free(rhs);
}

void vm_add (struct vm* vm)
{
  string* lhs = gtkn(vm);
  string* rhs = gtkn(vm);

  REG(vm->registers, lhs->data) = REG(vm->registers, lhs->data) + reg_or_literal(vm->registers, rhs->data);

  str_free(lhs);
  str_free(rhs);
}

void vm_sub (struct vm* vm)
{
  string* lhs = gtkn(vm);
  string* rhs = gtkn(vm);

  REG(vm->registers, lhs->data) = REG(vm->registers, lhs->data) - reg_or_literal(vm->registers, rhs->data);

  str_free(lhs);
  str_free(rhs);
}

void vm_mul (struct vm* vm)
{
  string* lhs = gtkn(vm);
  string* rhs = gtkn(vm);

  REG(vm->registers, lhs->data) = REG(vm->registers, lhs->data) * reg_or_literal(vm->registers, rhs->data);

  str_free(lhs);
  str_free(rhs);
}

void vm_div (struct vm* vm)
{
  string* lhs = gtkn(vm);
  string* rhs = gtkn(vm);

  REG(vm->registers, lhs->data) = REG(vm->registers, lhs->data) / reg_or_literal(vm->registers, rhs->data);

  str_free(lhs);
  str_free(rhs);
}

void vm_out (struct vm* vm)
{
  string* lhs = gtkn(vm);

  printf("%d\n", REG(vm->registers, lhs->data));

  str_free(lhs);
}

void vm_cmp (struct vm* vm)
{
  string* lhs = gtkn(vm);
  string* rhs = gtkn(vm);

  uint8_t bit = REG(vm->registers, lhs->data) == reg_or_literal(vm->registers, rhs->data);
  vm->registers->F ^= (-(unsigned long)bit ^ vm->registers->F) & (1UL << 0);

  str_free(lhs);
  str_free(rhs);
}

void vm_jmp (struct vm* vm)
{
  string* lhs = gtkn(vm);
  
  if ((vm->registers->F >> 0) & 1UL)
    vm->pc = (uint8_t) *lhs->data;
  
  str_free(lhs);
}

void vm_jne (struct vm* vm)
{
  string* lhs = gtkn(vm);
  
  if (!((vm->registers->F >> 0) & 1UL))
    vm->pc = (uint8_t) *lhs->data;
  
  str_free(lhs);
}

void vm_gop (struct vm* vm)
{
  vm->ci = vm->memory->data[vm->pc++];
}

int main (int argc, char* argv[])
{
  if (argc == 1)
  {
    fprintf(stderr, "Error: incorrect number of arguments");
    return 1;
  }

  char* filename = argv[1];
  printf("A1 >> Executing file: %s\n", filename);

  FILE* file = fopen(filename, "rb");
  struct vm* vm = vm_new();
  vm_load(vm, file);
  fclose(file);
  printf("A1 >> Program loaded!\n\n");

  void* funcs[] = {
    vm_err,
    vm_set,
    vm_out,
    vm_end,
    vm_add,
    vm_mul,
    vm_sub,
    vm_div,
    vm_cmp,
    NULL,
    vm_jmp,
    vm_jne
  };

  while (1)
  {
    if (vm->pc >= vm->memory->pgend) {
      printf("\nWarning: program was not terminated correctly.");
      vm_end(vm);
    }

    // Read the current operation
    vm_gop(vm);
    // Execute the operation
    ((void (*)(struct vm*))funcs[vm->ci])(vm);
  }
}
