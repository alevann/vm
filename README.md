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

Register E is the 'result' register - i.e. the result of mathematical operations will be written here overriding what was in there before.

Register F is the 'flag' register - useless as of now.

### Instruction set

The A1 currently supports a few instructions:

* SET: sets the value of the right hand operand to that of the left hand operand
* ADD: adds the value of the right hand operand to that of the left hand operand and writes the value in E
* OUT: prints the passed value to stdout (i need it for debugging lol)

## Ingore this

I need this for later:

```
gcc ./cmp.c -o cmp && ./cmp ./add.a1 && xxd ./add.ax && gcc vm.c -o vm && ./vm add.ax
```
