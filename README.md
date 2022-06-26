# A1 Specification

I thought it'd be interesting to make a simple virtual machine of sorts, a thing that can make calculations and do things, kinda.

## Registers

The A1 has 6 registers named from A to F.

### Registers A-D

Registers from A to D can be used freely and have no special behaviour or meaning.

### Registers E and F

Register E is the 'result' register - i.e. the result of mathematical operations will be written here overriding what was in there before.

Register F is the 'flag' register - useless as of now.

## Instruction set

The A1 currently supports few instructions:

* SET: sets the value of the right hand operand to that of the left hand operand
* ADD: adds the value of the right hand operand to that of the left hand operand and writes the value in E
* OUT: prints the passed value to stdout (i need it for debugging lol)
 