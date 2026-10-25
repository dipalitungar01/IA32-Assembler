#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "symbol_table.h"

struct Symbol table[MAX];

int count = 0;
int dataLC = 0;
int textLC = 0;

char section[20] = "";


char *trim(char *str)
{
    char *end;

    while (isspace((unsigned char)*str))
        str++;

    if (*str == '\0')
        return str;

    end = str + strlen(str) - 1;

    while (end > str && isspace((unsigned char)*end))
        end--;

    *(end + 1) = '\0';

    return str;
}


int searchSymbol(char *name)
{
    int i;

    for (i = 0; i < count; i++)
    {
        if (strcmp(table[i].name, name) == 0)
            return i;
    }

    return -1;
}


void insertSymbol(char *name, int size, char *value,
                  char *sec, int address, char status)
{
    int index = searchSymbol(name);

    if (index >= 0)
    {
        if (status == 'D')
        {
            table[index].size = size;

            snprintf(table[index].value,
                     VALUE_SIZE, "%s", value);

            snprintf(table[index].section,
                     sizeof(table[index].section), "%s", sec);

            table[index].address = address;
            table[index].status = 'D';
        }

        return;
    }

    if (count >= MAX)
    {
        printf("Error: Symbol table is full.\n");
        return;
    }

    table[count].id = count + 1;

    snprintf(table[count].name, NAME_SIZE, "%s", name);
    table[count].size = size;

    snprintf(table[count].value, VALUE_SIZE, "%s", value);

    snprintf(table[count].section,
             sizeof(table[count].section), "%s", sec);

    table[count].address = address;
    table[count].status = status;

    count++;
}


int getDataSize(char *type)
{
    if (strcmp(type, "db") == 0)
        return 1;

    if (strcmp(type, "dw") == 0)
        return 2;

    if (strcmp(type, "dd") == 0)
        return 4;

    if (strcmp(type, "dq") == 0)
        return 8;

    return 1;
}


int getValueSize(char *value)
{
    int size = 0;
    int inString = 0;
    int tokenHasValue = 0;
    int i;

    for (i = 0; ; i++)
    {
        char ch = value[i];

        if (ch == '\'')
        {
            if (!inString)
            {
                inString = 1;
                tokenHasValue = 0;
            }
            else
            {
                inString = 0;
                tokenHasValue = 0;
            }
        }
        else if (inString)
        {
            size++;
        }
        else if (ch == ',' || ch == '\0')
        {
            if (tokenHasValue)
                size++;

            tokenHasValue = 0;

            if (ch == '\0')
                break;
        }
        else if (!isspace((unsigned char)ch))
        {
            tokenHasValue = 1;
        }
    }

    return size;
}


int calculateDataSize(char *type, char *value)
{
    int elementSize = getDataSize(type);
    int valueSize = getValueSize(value);

    return elementSize * valueSize;
}


void displayTable(void)
{
    int i;

    printf("\nSYMBOL TABLE\n");

    printf("------------------------------------------------------------------------------------------------\n");

    printf("%-5s %-18s %-8s %-24s %-12s %-12s %-7s\n",
           "#ID", "Name", "Size", "Value",
           "Section", "Address", "Status");

    printf("------------------------------------------------------------------------------------------------\n");

    for (i = 0; i < count; i++)
    {
        printf("%-5d %-18s %-8d %-24s %-12s %-12d %-7c\n",
               table[i].id,
               table[i].name,
               table[i].size,
               table[i].value,
               table[i].section,
               table[i].address,
               table[i].status);
    }

    printf("------------------------------------------------------------------------------------------------\n");
}


void displayUndefined(void)
{
    int i;
    int found = 0;

    printf("\nUndefined Symbols:\n");

    for (i = 0; i < count; i++)
    {
        if (table[i].status == 'U')
        {
            printf("%s\n", table[i].name);
            found = 1;
        }
    }

    if (!found)
        printf("None\n");
}

