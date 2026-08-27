# Opcode Check Assignment

## 1] Assignment Overview

The Opcode Check program is a C program used to check assembly language instructions.

The program reads the opcode table from `opcode.txt` and the assembly program from `AssemblyProgram.asm`.

It checks whether the mnemonic is present in the opcode table and identifies the type of operand used in the instruction.

## 2] Team Members and Work

### 1. Dipali Tungar — Opcode Checking

Responsibilities:

- Reads the opcode table.
- Checks whether the mnemonic is present in the opcode table.
- Reads the assembly program.
- Displays whether the mnemonic is found or not found.
- Displays the result.

Branch: `dipali-opcode`

Code File:

- `OpcodeCheck.c`

GitHub: https://github.com/dipalitungar01

### 2. Mugdha Kulkarni — Operand Handling

Responsibilities:

- Handles the operands used in the assembly program.
- Checks the type of each operand.
- Identifies register operands.
- Identifies constant operands.
- Identifies memory operands.
- Identifies symbol operands.

Branch: `mugdha-operand`

Code File:

- `OpcodeCheck.c`

GitHub: https://github.com/mugdhak80

## 3] GitHub Branches

The project contains the following branches:

1. `main` - Final project
2. `dipali-opcode` - Opcode checking
3. `mugdha-operand` - Operand handling

## 4] Program Flow

The program works in the following way:

opcode.txt  
↓  
Read Opcode Table  
↓  
AssemblyProgram.asm  
↓  
Read Mnemonic and Operands  
↓  
Check Mnemonic  
↓  
FOUND / NOT FOUND  
↓  
Identify Operand Type  
↓  
Register / Constant / Memory / Symbol  
↓  
Display Output
