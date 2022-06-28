# A1

I thought it'd be interesting to make a simple virtual machine of sorts, a thing that can make calculations and do things, kinda. The objective is for the A1 to be turing complete as well as support some nice featues that I think of along the way.

The A1 comes with both a compiler and a VM that executes the code compiled by the compiler.

Currently only linux is supported (MacOS probably works as well?)

## Compiling the compiler and the VM

Both the compiler and the VM can be compiled with make:
```bash
$> make compiler # Output executable in build/a1c
$> make machine  # Output executable in build/a1m
```

All build files are stored in the `build` folder, including the output executables.

There is also a make rule which builds everything and executes a script right after, `cbcates` (Clean Build and Compile All Targets and Execute a Script) takes a `SCRIPT` argument which is the filename of an `a1` file in the `scripts` directory without the extension:
```bash
$> make cbcates SCRIPT=lbl
```

## A1 Specification

There is no specification, I'm winging it as hard as humanly possible. Here's some documentation tho.

### Registers

The A1 has 6 32-bit registers named from A to F:

* Registers A-D can be used freely
* Register E is used for functions return values
* Register F is used for flags

#### The flags register

This is the current layout for of the F register:

```
   Currently planned to be 
  reserved for custom flags
      ↓               ↓
      00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
                                                    ↑
                                              Equality flag
```

### Instruction set 

#### ADD

The ADD instruction takes two values and performs an addition.

Arguments: (2)
1. A register
1. A register or a literal

The result of the addition is stored in the first register.

#### SUB

The SUB instructions takes two values and performs a substraction.

Arguments: (2)
1. A register
1. A register or literal

The result of the substraction is stored in the first register.

#### MUL

The MUL instructions takes two values and performs a multiplication.

Arguments: (2)
1. A register
1. A register or literal

The result of the multiplication is stored in the first register.

#### DIV

The DIV instructions takes two values and performs a division.

Arguments: (2)
1. A register
1. A register or literal

The result of the division is stored in the first register.
#### SET

The SET instruction writes a value to a register.

Arguments: (2)
1. A register
1. A register or a literal

The value previously stored in the register is completely overwritten.

#### OUT

The OUT instruction writes the value stored in a register to STDOUT.

Arguments: (1)
1. A register

#### LBL

The LBL instruction is removed at compile time and it's only purpose is to define an anchor that can be jumped to.

Arguments: (1)
1. A literal string, can be anything

Labels must be declared before usage and are removed during the compilation process.

#### JMP

The JMP instruction jumps to a label that has been defined by a previous LBL instruction, if and only if the equality flag is set.

Arguments: (1)
1. A label that has already been defined

#### JNE

The JNE instruction jumps to a label that has been defined by a previous LBL instruction, if and only if the equality flag is not set.

Arguments: (1)
1. A label that has already been defined


### Executable files structure

Files compiled by the A1C have a simple structure, each token in a `.a1` file is compiled as a 8-bit unsigned integer.


## Future stuff

Section for future stuff and notes.

TODO:
* Labels can't be used before being declared, not nice
* A better hashmap implementation is needed, also it should be moved in the `include` directory
* `string` should be moved in the `include` directory and it should have a method to append other strings (`str_app` and `str_app_s`)
* The `type` field in the compiler's `token` is pretty useless right now, I feel the direction is right but it's not quite there yet
* The VM has 32-bit registers whereas the compiler compiles 8-bit tokens... should fix that lol
* The JMP instruction should not check for equality, instead a new instruction like JIE (Jump If Equal) should be implemented, JMP should always jump
* POP and PUSH instructions to store more than 4 things at a time

### VM Refactor / Rework

Points of the rework:
1. Have a memory of sorts
1. Have an abstraction over the raw file pointer
1. Operations should be implemented more uniformly

Operations could be abstracted as a function like this:

```c
// The return code could influence the
// flow of execution of the program (eg. quit)
int op_XXX (struct vm* vm);
```

The VM struct could look something like this:

```c
struct vm {
  registers* regs;
  memory* mem;
  uint32_t pc;  // Program counter / pointer sort of thing
};
```

The memory might not need to be a struct but I feel at some point I will need an abstraction over it anyways so... It would probably be something like this:

```c
typedef struct {
  uint8_t data [MEMORY_SIZE];
} memory;
```

With a setup like that the program could be loaded into memory (point 1) and the `pc` could be used to keep track of where we are in memory, also moving that we could move into other parts of the program so point 2 is addressed as well. A part of the memory could be ROM (?) and contain the program code and data, and the RAM part where a possible future stack or something lives could be after that?

This also addresses point 3 as functions could be stored in a hashmap like `HashMap<Op, Func>` where `Op` is the enum and `Func` is a pointer to a function that follows the abstraction outlined above.
