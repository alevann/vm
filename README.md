# A1

I thought it'd be interesting to make a simple virtual machine of sorts, a thing that can make calculations and do things, kinda.

## Compiling the compiler and the VM

The A1 comes with both a compiler and a VM that executes the code compiled by the compiler.

Compile the compiler:
```
gcc compiler/*.c -o a1c
```

Compile the vm:
```
gcc vm.c -o vm
```

Currently only linux is supported (MacOS probably works as well?)

## A1 Specification

There is no specification, I'm winging it as hard as humanly possible.

### Registers

The A1 has 6 registers named from A to F.

#### Registers A-D

Registers from A to D can be used freely and have no special behaviour or meaning.

#### Registers E and F

Register E will be used for functions return values.

Register F is the 'flag' register - useless as of now.

### Instruction set

#### ADD

The ADD instruction takes two values and performs an addition.

Arguments: (2)
1. A register
1. A register or a literal

The result of the addition is stored in the first register.

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
