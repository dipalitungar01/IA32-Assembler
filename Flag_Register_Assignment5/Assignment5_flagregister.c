/*
 * Assignment5_flagregister.c - Flag Register simulation (8-bit operations)
 * Instructions: ADD, SUB, CMP, AND, OR, XOR
 * Flags: CF, ZF, SF, OF, PF
 *
 * Compile: gcc Assignment5_flagregister.c -o flagregister
 * Run:     ./flagregister
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Flags: 1 = set, 0 = clear */
int CF = 0, ZF = 0, SF = 0, OF = 0, PF = 0;

/* ZF, SF, PF depend only on the result */
void set_common_flags(unsigned char result)
{
    ZF = (result == 0);                 /* result is zero            */
    SF = (result & 0x80) != 0;          /* top bit (bit 7) is 1      */

    int count = 0;                      /* count the 1-bits          */
    for (int i = 0; i < 8; i++)
        if (result & (1 << i))
            count++;
    PF = (count % 2 == 0);              /* even number of 1-bits     */
}

unsigned char do_add(unsigned char a, unsigned char b)
{
    int full = a + b;
    unsigned char result = full & 0xFF;

    CF = (full > 255);                                   /* carry out of 8 bits */
    OF = ((a & 0x80) == (b & 0x80)) &&                   /* same sign inputs,   */
         ((a & 0x80) != (result & 0x80));                /* different sign out  */
    set_common_flags(result);
    return result;
}

unsigned char do_sub(unsigned char a, unsigned char b)
{
    unsigned char result = (a - b) & 0xFF;

    CF = (a < b);                                        /* borrow needed       */
    OF = ((a & 0x80) != (b & 0x80)) &&                   /* different sign      */
         ((a & 0x80) != (result & 0x80));                /* inputs, result sign */
    set_common_flags(result);                            /* differs from a      */
    return result;
}

unsigned char do_logic(unsigned char a, unsigned char b, const char *op)
{
    unsigned char result;

    if (strcmp(op, "AND") == 0)      result = a & b;
    else if (strcmp(op, "OR") == 0)  result = a | b;
    else                             result = a ^ b;

    CF = 0;                             /* logic instructions clear CF and OF */
    OF = 0;
    set_common_flags(result);
    return result;
}

int main(void)
{
    char op[10];
    int a, b;
    unsigned char result;

    printf("FLAG REGISTER\n\n");
    printf("Instructions :\n");
    printf("ADD, SUB, CMP, AND, OR, XOR\n\n");

    printf("Enter Instruction : ");
    scanf("%9s", op);
    for (int i = 0; op[i]; i++)         /* accept lowercase too */
        op[i] = toupper((unsigned char)op[i]);

    printf("Enter first operand (0-255): ");
    scanf("%d", &a);
    printf("Enter second operand (0-255): ");
    scanf("%d", &b);

    if (a < 0 || a > 255 || b < 0 || b > 255) {
        printf("\nError: operands must be between 0 and 255.\n");
        return 1;
    }

    if (strcmp(op, "ADD") == 0)
        result = do_add(a, b);
    else if (strcmp(op, "SUB") == 0 || strcmp(op, "CMP") == 0)
        result = do_sub(a, b);          /* CMP is SUB without storing result */
    else if (strcmp(op, "AND") == 0 || strcmp(op, "OR") == 0 || strcmp(op, "XOR") == 0)
        result = do_logic(a, b, op);
    else {
        printf("\nError: unknown instruction '%s'.\n", op);
        return 1;
    }

    printf("\nInstruction: %s %d, %d\n", op, a, b);
    if (strcmp(op, "CMP") == 0)
        printf("Result: %d (0x%02X) [CMP does not store the result]\n", result, result);
    else
        printf("Result: %d (0x%02X)\n", result, result);

    printf("\nFlag Register\n");
    printf("CF (Carry Flag)    = %d\n", CF);
    printf("ZF (Zero Flag)     = %d\n", ZF);
    printf("SF (Sign Flag)     = %d\n", SF);
    printf("OF (Overflow Flag) = %d\n", OF);
    printf("PF (Parity Flag)   = %d\n", PF);

    return 0;
}
