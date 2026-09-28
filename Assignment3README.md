# Assignment 3: Translation of Data Section

 Aim

To write a C program to convert data values into ASCII, hexadecimal, and little-endian format.

 Description

This C program performs the following conversions:

- String to ASCII
- String to Hexadecimal
- Decimal to Hexadecimal
- Decimal to Little Endian

 Input

The program uses the following data values:

- Dipali
- mugdha
- 23
- This is Assignment
- 500

 Output

Assignment 3 translation of data section

String : Dipali
ASCII : 68 105 112 97 108 105
Hex : 44 69 70 61 6C 69

String : mugdha
ASCII : 109 117 103 100 104 97
Hex : 6D 75 67 64 68 61

Decimal : 23
Hex : 00000017
Little Endian : 17 00 00 00

String : This is Assignment
ASCII : 84 104 105 115 32 105 115 32 65 115 115 105 103 110 109 101 110 116
Hex : 54 68 69 73 20 69 73 20 41 73 73 69 67 6E 6D 65 6E 74

Decimal : 500
Hex : 000001F4
Little Endian : F4 01 00 00

Functions Used

decimal()
This function converts a decimal number into hexadecimal and displays its 4-byte little-endian form.

string()
This function converts each character of a string into its ASCII value and hexadecimal value.

Conversion

String:
Dipali → ASCII → Hexadecimal

Decimal:
500 → Hexadecimal → Little Endian

Conclusion

The C program successfully converts string and decimal data into ASCII, hexadecimal, and little-endian formats.
