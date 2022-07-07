#ifndef __A1_OP_H_
#define __A1_OP_H_

enum Op
{
  ERR,
  SET,
  OUT,
  END,
  ADD,
  MUL,
  SUB,
  DIV,
  CMP,
  LBL,
  JMP,
  JNE,
  CALL
};

extern char* OP_STR [];

#endif
