#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void decimal(int n)
{
    printf("Decimal : %d\n", n);
    printf("Hex : %08X\n", n);

    printf("Little Endian : %02X %02X %02X %02X\n\n",
           n & 0xFF,
           (n >> 8) & 0xFF,
           (n >> 16) & 0xFF,
           (n >> 24) & 0xFF);
}

void string(char str[])
{
    int i;

    printf("String : %s\n", str);//print string

    printf("ASCII : ");

    for (i = 0; str[i] != '\0'; i++)//convert to ascii check every char
        printf("%d ", (unsigned char)str[i]);

    printf("\nHex : ");

    for (i = 0; str[i] != '\0'; i++)//ascii to hex
        printf("%02X ", (unsigned char)str[i]);

    printf("\n\n");
}

int main()
{
    FILE *fp;
    char line[200];//store info from the assembly line
    char label[20];
    char type[10];
    char value[150];

    fp = fopen("AssemblyProgram_ass3.asm", "r");

    if (fp == NULL)
    {
       printf("File Opening error\n\n");
        return 1;
       
    }
    printf("\n\nAssignment 3 translation of data section\n\n");
        

    while(fgets(line, sizeof(line), fp))//fgets reads the assembly file one line at a time
    {
        if(strstr(line, "section") != NULL)//skip section line
            continue;

        if(sscanf(line, "%s %s %[^\n]",
                  label, type, value) != 3)//seperate lable ,type ,value
                continue;

        if(strcmp(type, "db") == 0)//check db
        {
           if (value[0] == '"' || value[0] == '\'')//db is string
            {
                char str[150];

                strcpy(str, value);//copy value

                
                int len = strlen(str);

                if(len > 1 &&
                    (str[len - 1] == '"' ||
                     str[len - 1] == '\''))
                {
                    str[len - 1] = '\0';//remove end quote
                    memmove(str, str + 1, strlen(str));//start quote
                }

                string(str);
            }
            else
            {
                int n = atoi(value);//atoi convert text number to integer "24" to 24

                printf("Decimal : %d\n", n);
                printf("Hex : %02X\n\n", n);
            }
        }

        else if (strcmp(type, "dd") == 0)//check dd
        {
            int n = atoi(value);//convert

            decimal(n);
        }
    }

    fclose(fp);

    return 0;
}