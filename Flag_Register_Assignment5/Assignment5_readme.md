# Assignment 5: IA-32 Flag Register

## 1. Objective
To implement and demonstrate the Zero Flag (ZF), Sign Flag (SF), and Carry Flag (CF) using C.

## 2. Description
This assignment implements a small two-pass assembler/interpreter for a subset of IA-32 instructions. It simulates 32-bit register and flag operations but does not generate native machine code.

## 3. Instructions Supported
- MOV
- ADD
- SUB
- CMP
- JMP
- JNE
- JE
- HLT

## 4. Registers and Flags
  Registers:  EAX, EBX, ECX, EDX

  Flags:
- **ZF (Zero Flag):** Set when the result is zero.
- **SF (Sign Flag):** Represents bit 31 of the result.
- **CF (Carry Flag):** Set when addition produces a carry or subtraction produces an unsigned borrow.

## 5. Compilation Command
I compiled the program using:

gcc main.c -o ia32flags


## 6. Execution Commands
I ran the following test files:


./ia32flags carry_add.asm
./ia32flags cmp_no_change.asm
./ia32flags sign_sub.asm
./ia32flags zf_jne.asm
```

## 7. Test Results

| Test File | ZF | SF | CF | Result |
|---|---:|---:|---:|---|
| carry_add.asm | 1 | 0 | 1 | PASS |
| cmp_no_change.asm | 1 | 0 | 0 | PASS |
| sign_sub.asm | 0 | 1 | 1 | PASS |
| zf_jne.asm | 0 | 1 | 1 | PASS |

All four test files executed successfully and produced the expected results.

## 8. Conclusion
In this assignment, I implemented and tested the ZF, SF, and CF flags using C. I also tested arithmetic, comparison, and conditional jump instructions successfully.