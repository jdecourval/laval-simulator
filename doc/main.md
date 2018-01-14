# LAVAL COMPUTER reference manual

## Introduction
LAVAL COMPUTER is the first computer to implement the revolutionary LAVAL CPU architecture.
 Like every good acronym, LAVAL is recursively defined: Laval Advanced Vectorized Architecture Laboratory.

Its a novel architecture set to revolutionize the computing world with its massively parallel core organisation.

## Architecture

LAVAL architecture lies somewhere between those of CPU, GPU and FPGA.
It consists of a large number of simple locally connected cores.
Cores execute instructions stored in their linked memory bank.
Branches are made by switching execution from one memory bank to another one.

### Core
The main component of the LAVAL architecture is the CORE unit.
It is a very low complexity core capable of executing a couple of different 8 bits instructions *(arguments included)*.

pipeline depth = 2

#### VAL register


#### MUX register
Two cores loading at the same time ...


#### Status register

#### PC register

### Memory bank


## Instructions

### Basic instructions

#### NOP
No operation

**Argument:** None

**Description:**

Do nothing for one cycle


#### SYN
Sync

**Argument:** None

**Description:**

Sync VAL with connected mux(es)


#### DBG
Output to debugger


**Argument:** None

**Description:**

Output core status to connected debugger


#### HLT
Halt

**Argument:** None

**Description:**

Stop CPU execution and return VAL. Two cores returning at the same time return an undefined VAL.


#### HCF
**Argument:** None

**Description:**

???



### Controlling mux
#### MUX
Set multiplexer to another core

**Arguments:**

| Size | Description          |
|:---:|:-------------------|
| 0..2 | Offset on dimension 0 |
| 0..2 | Offset on dimension 1 |
| 0..2 | Offset on dimension 2 |

**Description:**

Point mux to another core as indicated by arguments

**Example:**

```
MUX CURRENT, BEFORE, AFTER
```


#### MXR
Set multiplexer to a register

**Argument:**

| Size | Description       |
|:---:|:-----------------|
| 0..1 | 0: PC, 1: MEMBANK |


**Description:**

Point mux to a core register as indicated by argument


#### CTC
Connect to carry

**Argument:** None

**Description:**



#### CTV
Connect to VAL

**Argument:** None

**Description:**





### Reading from mux
#### MXD
Multiplexer discard

**Argument:** None

**Description:**

Fetch and discard a value from the mux. Use this instruction to unlock a syncing core.


#### MXL
Multiplexer load

**Argument:** None

**Description:**

Fetch and load into VAL the value from mux.


#### MXA
Multiplexer addition

**Argument:** None

**Description:**

Fetch and add the value from the mux to VAL.


#### MXS
Multiplexer subtraction

**Argument:** None

**Description:**

Fetch and subtract the value from the mux to VAL.



### Jumping
#### JMP
Jump

**Argument:**

| Size | Description       |
|:---:|:-----------------|
| 0..15 | Membank id |

**Description:**

Point current core to a new membank as indicated by the argument. PC is reset to 0.


#### JLZ
Jump if less than zero

**Argument:**

| Size | Description       |
|:---:|:-----------------|
| 0..15 | Membank id |

**Description:**

Point current core to a new membank as indicated by the argument if VAL if less than 0. PC is reset to 0.


#### JEZ
Jump if equal to zero

**Argument:**

| Size | Description       |
|:---:|:-----------------|
| 0..15 | Membank id |

**Description:**

Point current core to a new membank as indicated by the argument if VAL if equal to 0. PC is reset to 0.


#### JGZ
Jump if greater than zero

**Argument:**

| Size | Description       |
|:---:|:-----------------|
| 0..15 | Membank id |

**Description:**

Point current core to a new membank as indicated by the argument if VAL if greater than 0. PC is reset to 0.



### Using constants
#### LCL
Load constant into low part

**Argument:** None

| Size | Description       |
|:---:|:-----------------|
| 0..15 | Value to load |

**Description:**

Load a 4 bits constant into the 4 lower bits of VAL.

**Notes:**

To load the higher bits, use LCH.


#### LCH
Load constant into high part

**Argument:** None

| Size | Description       |
|:---:|:-----------------|
| 0..15 | Value to load |

**Description:**

Load a 4 bits constant into the 4 higher bits of VAL.

**Notes:**

To load the lower bits, use LCL.


#### LLS
Left logical shift

**Argument:** None

| Size | Description       |
|:---:|:-----------------|
| 0..15 | Value to shift by |

**Description:**

Logically left shift VAL by a number of bits indicated by the argument.


#### RLS
Right logical shift

**Argument:** None

| Size | Description       |
|:---:|:-----------------|
| 0..15 | Value to shift by |

**Description:**

Logically right shift VAL by a number of bits indicated by the argument.


#### CAD
Constant addition

**Argument:** None

| Size | Description       |
|:---:|:-----------------|
| 0..15 | Value to add |

**Description:**

Add a constant to VAL.


#### CSU
Constant subtraction

**Argument:** None

| Size | Description       |
|:---:|:-----------------|
| 0..15 | Value to subtract |

**Description:**

Subtract a constant to VAL.


#### CAN
Constant AND

**Argument:** None

| Size | Description       |
|:---:|:-----------------|
| 0..15 | Second AND operand |

**Description:**

Apply a logical AND to the four lower bits of VAL. The 4 higher bits are cleared.


#### COR
Constant OR

**Argument:** None

| Size | Description       |
|:---:|:-----------------|
| 0..15 | Second OR operand |

**Description:**

Apply a logical OR to the four lower bits of VAL. The 4 higher bits are kept unmodified.



### Input and output

#### RET
Return

**Argument:** None

**Description:**



#### INP
Input

**Argument:** None

**Description:**




## Simulator


## LAVAL-M
LAVAL-M1 is the first version of the official embedded subset of LAVAL.
Its made to have a very low memory requirement, power consumption and an high AL<sup>[1](#AL)</sup>.

TODO: two complement

TODO: Signed behaviours:
- Shifts
- Overflows

<a name="AL">1. </a>Annoyance level: Defined as (1 - Percentage of student who would repeat their programming experiments with the CPU architecture)

