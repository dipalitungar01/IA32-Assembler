# Assignment 4: IA-32 Assembler Symbol Table Generation

## Student Details

- Student 1: Dipali Tungar
- Student 2: Mugdha Kulkarni

## Files

- Assignment4.c  — Contains the C program to generate the symbol table.
- ass4.asm  — Contains the assembly input used to test the program.

## Work Distribution

### Dipali Tungar

- Created the symbol table.
- Implemented symbol insertion and searching.
- Calculated data sizes and addresses.
- Displayed the symbol table.
- Checked for undefined symbols.

### Mugdha Kulkarni

- Implemented reading and processing of assembly input.
- Identified instructions and labels.
- Calculated instruction sizes and addresses.
- Checked jump and call targets.
- Tested the program.

## Compilation and Execution

### Step 1: Open the directory

cd ~/IA32-Assembler/Assignment_4


### Step 2: Compile the C program

h
gcc Assignment4.c -o Assignment4


### Step 3: Run the program


./Assignment4


## Conclusion

The program reads assembly input and generates a symbol table containing symbol names, sizes, values, sections, addresses, and status. It also identifies undefined symbols, such as abc.