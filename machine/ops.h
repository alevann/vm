#ifndef __A1_OPS_H_
#define __A1_OPS_H_

#include "registers.h"

void op_set(registers*, char*, char*);
void op_add(registers*, char*, char*);
void op_sub(registers*, char*, char*);
void op_mul(registers*, char*, char*);
void op_div(registers*, char*, char*);
void op_cmp(registers*, char*, char*);
void op_jmp(registers*, char*, FILE*);
void op_jne(registers*, char*, FILE*);
void op_out(registers*, char*);

#endif