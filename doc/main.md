# LAVAL COMPUTER reference manual

## Introduction
LAVAL COMPUTER is the first computer to implement the revolutionary LAVAL CPU architecture.
 Like every good acronym, LAVAL is recursively defined: Laval Advanced Vectorized Architecture Laboratory.

Its a novel architecture set to revolutionize the computing world with its massively parallel architecture.
Almost every implementation detail is configurable.

## Architecture

LAVAL architecture lies somewhere between those of CPU, GPU and FPGA.
It consists of a large number of simple locally connected cores.
Cores execute instructions stored in their linked memory bank.
Branches are made by switching execution from one memory bank to another one.
The MUX 

### Core
The main component of the LAVAL architecture is the CORE unit.
It is a very low complexity core capable of executing a couple of different 8 bits instructions *(arguments included)*.

#### VAL register


#### MUX register

#### Status register

#### PC register

### Memory bank


## Instructions

## Simulator

## LAVAL-M
LAVAL-M1 is the first version of the official embedded subset of LAVAL.
Its made to have a very low memory requirement, power consumption and an high AL<sup>[1](#AL)</sup>.

<a name="AL">1</a>Annoyance level: Defined as (1 - Percentage of student who would repeat their programming experiments with the CPU architecture)

