# Note dump

A dump of unformatted notes, don't necessarly make sense, don't read

CMP
lhs -> register
rhs -> register or literal
Sets least significant bit in big endian in F register

Labels:
Labels are replaced at compile time with address which are just offsets from the start of the file
When a label is used but not found it's saved in a special map which is then reprocessed at the end of the compilation process to see if it gets defined later on, if so good otherwise an error is output and the compilation fails!

JMP:
Takes one argument which is the address to jump to and it jumps only if the flag in F is set

Functions:
Are exactly like labels but end with a RET instruction, before invoking a function the current offset is saved in memory (so we know where to return to)

RET:
Quits the function, takes one argument which is the return value which will go in E

Registers are to be printed with the cool \r thing
Also SUB MUL and DIV must be implemented 

Also a thing I remembered just now
.. I need a NOT instruction of sorts for jumps like jump if not JIN? JMP and JIN? NJP? idk I'll need think about that one still

PSH
lhs -> register
pushes the value of a register in the memory stack thing

POP
lhs -> register
pops the last pushed value in the register

Not really ergonomic?
When implementing functions should save the stack and restore it when returning
