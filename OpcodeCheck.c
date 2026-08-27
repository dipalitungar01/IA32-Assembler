#include<stdio.h>
#include<string.h>
#include<ctype.h>

#define MAX_OPCODE 100

int isRegister(char op[])
{
     if(strcmp(op, "EAX") == 0 || 
        strcmp(op, "EBX") == 0 ||
        strcmp(op, "ECX") == 0 ||
        strcmp(op, "EDX") == 0 ||                
        strcmp(op, "ESI") == 0 ||
        strcmp(op, "EDI") == 0 ||
        strcmp(op, "EBP") == 0 ||
        strcmp(op, "ESP") == 0 )
        return 1;
                
     return 0;
}

int isConstant(char op[])
{
     int i = 0;
     
    if (op[0] == '-' || op[0] == '+')
       i++;

    if(op[i] == '\0')
    return 0;
    
    for(; op[i] != '\0'; i++)
    {
        if(!isdigit((unsigned char)op[i]))
           return 0;
    }

    return 1;
}

int isMemory(char op[])
{
    int len = strlen(op);

    if(len >= 2 && op[0] == '[' && op[len - 1] == ']')
    return 1;

    return 0;
}

void upperCase(char str[])
{
    int i;

    for (i = 0; str[i] != '\0'; i++)
       str[i] = toupper((unsigned char)str[i]);

}

void printType(char op[])
{
    char temp[50];

    strcpy(temp, op);
    upperCase(temp);

    if(isRegister(temp))
      printf("Register");
    else if(isConstant(temp))
      printf("Constant");
    else if(isMemory(temp))
      printf("Memory");
    else 
      printf("Symbol");   
}

int checkOpcode(char mnemonic[])
{
    FILE *fp;
    char line[200];
    char word[20];


    fp = fopen("opcode.txt", "r");

    if(fp == NULL)
    return 0;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if(sscanf(line, "%19s", word) == 1)
        {
            upperCase(word);

            if(strcmp(mnemonic, word) == 0)
            {
                fclose(fp);
                return 1;

            }
        }
    }
     fclose(fp);

     return 0;
}

int main()
{
    FILE *fp;

    char line[200];
    char mnemonic[20];
    char op1[50];
    char op2[50];

    int lineNumber = 1;
    int len;

    fp = fopen("AssemblyProgram.asm", "r");

    if(fp == NULL)
    {
        printf("File opening error\n");
        return 1;
    }

    printf("OPCODE CHECK PROGRAM\n");

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        op1[0] = '\0';
        op2[0] = '\0';

        if(sscanf(line, "%19s %49s %49s", mnemonic, op1, op2) < 1)
        {
            lineNumber++;
            continue;
        }

        upperCase(mnemonic);

        len = strlen(op1);

        if(len > 0 && op1[len -1] == ',')
           op1[len - 1] = '\0';

        printf("\nLine %d\n", lineNumber);
        printf("Mnemonic : %s\n", mnemonic);
        
        if(checkOpcode(mnemonic))
        printf("Exists   : YES\n");
        else
        printf("Exists   : NO\n");

        if(strlen(op1) > 0)
        {
            printf("Operand 1: %s\n", op1);
            printf("Type   : ");
            printType(op1);
            printf("\n");
    
        }

        if(strlen(op2) > 0)
        {
            printf("Operand 2: %s\n", op2);
            printf("Type   : ");
            printType(op2);
            printf("\n");
    
        }

        lineNumber++;
    }

    fclose(fp);

    printf("\nCHECKING COMPLETED\n");

    return 0;
    

}


       