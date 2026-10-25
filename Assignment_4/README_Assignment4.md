# Assignment 4: IA-32 Assembler Symbol Table Generation

## Student Details

- Student 1:    Dipali Tungar
- Student 2:    Mugdha Kulkarni

## Files

- Assignment4.c — Contains the C program for generating the symbol table.
- ass4.asm — Contains the assembly input used to test the program.

## Work Distribution

### Dipali Tungar
- Created the symbol table structure.
- Implemented symbol insertion and searching.
- Calculated data declaration sizes and addresses.
- Displayed the symbol table.
- Identified undefined symbols.

### Mugdha Kulkarni
- Worked on reading and processing assembly input.
- Identified assembly instructions and labels.
- Calculated instruction sizes and addresses.
- Checked jump targets and identified undefined symbols.
- Tested the program output.

## Compilation and Execution

### Step 1: Open the directory


cd ~/IA32-Assembler/Assignment_4


### Step 2: Compile the C program

gcc Assignment4.c -o Assignment4


### Step 3: Run the program

./Assignment4


## Program Output


SYMBOL TABLE
------------------------------------------------------------------------------------------------
#ID   Name               Size     Value                    Section      Address      Status
------------------------------------------------------------------------------------------------
1     a                  8        4, 8                     Data         0            D
2     a1                 7        'HELLO',10,0             Data         8            D
3     a2                 8        40, 83                   Data         15           D
4     a3                 8        4, 8                     Data         23           D
5     a4                 7        'WORLD',10,0             Data         31           D
6     a5                 8        40, 83                   Data         38           D
7     a6                 8        4, 8                     Data         46           D
8     a7                 9        'WELCOME',10,0           Data         54           D
9     a8                 8        40, 83                   Data         63           D
10    main               0        -                        Text         0            D
11    abc                0        -                        Text         -            U
12    pqr                0        -                        Text         13           D
------------------------------------------------------------------------------------------------

Undefined Symbols:
abc
Assembly symbol-table processing completed.


## Conclusion

The program reads assembly input and generates a symbol table containing symbol names, sizes, values, sections, addresses, and definition status. It also identifies undefined symbols, such as abc.