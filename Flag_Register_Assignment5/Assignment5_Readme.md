# Assignment 5 - Flag Register

## What this program does
This C program shows how the flag register of a computer works.
You give one instruction and two numbers. The program prints the result
and the value of 5 flags.

## Instructions it supports
ADD, SUB, CMP, AND, OR, XOR

Numbers must be between 0 and 255 (8-bit).

## Flags used
CF (Carry Flag)    = 1 if ADD result is more than 255, or SUB has a borrow
ZF (Zero Flag)     = 1 if the result is 0
SF (Sign Flag)     = 1 if the top bit of the result is 1 (negative)
OF (Overflow Flag) = 1 if the signed result is wrong
PF (Parity Flag)   = 1 if the result has an even number of 1 bits

Note:
- AND, OR and XOR always make CF = 0 and OF = 0.
- CMP works like SUB, but it does not store the result. It only sets flags.

## How to compile
gcc Assignment5_flagregister.c -o flagregister

## How to run
./flagregister

Then type the instruction and the two numbers when it asks.

## Sample output
Enter Instruction : sub
Enter first operand (0-255): 255
Enter second operand (0-255): 3

Instruction: SUB 255, 3
Result: 252 (0xFC)

Flag Register
CF (Carry Flag)    = 0
ZF (Zero Flag)     = 0
SF (Sign Flag)     = 1
OF (Overflow Flag) = 0
PF (Parity Flag)   = 1

## Explanation of sample output
255 - 3 = 252, which is 11111100 in binary.
CF = 0 because there was no borrow.
ZF = 0 because the result is not zero.
SF = 1 because the first bit of the result is 1.
OF = 0 because there was no signed overflow.
PF = 1 because the result has six 1 bits (even number).

