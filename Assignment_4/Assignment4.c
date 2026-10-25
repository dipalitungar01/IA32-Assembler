#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100
#define NAME_SIZE 64
#define VALUE_SIZE 160

struct Symbol
{
    int id;
    char name[NAME_SIZE];
    int size;
    char value[VALUE_SIZE];
    char section[20];
    int address;
    char status;
};

struct Symbol table[MAX];

int count = 0;
int dataLC = 0;
int textLC = 0;

char section[20] = "";



char *trim(char *str)
{
    while (isspace((unsigned char)*str))
    {
        str++;
    }

    char *end = str + strlen(str);

    while (end > str &&
           isspace((unsigned char)end[-1]))
    {
        end--;
    }

    *end = '\0';

    return str;
}



int readToken(char **cursor, char *token, size_t capacity)
{
    char *p = *cursor;
    size_t i = 0;

    while (isspace((unsigned char)*p))
    {
        p++;
    }

    if (*p == '\0')
    {
        *cursor = p;
        return 0;
    }

    while (*p != '\0' &&
           !isspace((unsigned char)*p))
    {
        if (i + 1 < capacity)
        {
            token[i++] = *p;
        }

        p++;
    }

    token[i] = '\0';
    *cursor = p;

    return 1;
}



int searchSymbol(const char name[])
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(table[i].name, name) == 0)
        {
            return i;
        }
    }

    return -1;
}



void insertSymbol(const char name[],
                  int size,
                  const char value[],
                  const char sec[],
                  int address,
                  char status)
{
    int i = searchSymbol(name);

    if (i != -1)
    {
        if (table[i].status == 'U' && status == 'D')
        {
            table[i].size = size;

            snprintf(table[i].value,
                     sizeof(table[i].value),
                     "%s", value);

            snprintf(table[i].section,
                     sizeof(table[i].section),
                     "%s", sec);

            table[i].address = address;
            table[i].status = 'D';
        }
        else if (table[i].status == 'D' && status == 'D')
        {
            printf("Duplicate symbol: %s\n", name);
        }

        return;
    }

    if (count >= MAX)
    {
        printf("Symbol table is full!\n");
        return;
    }

    table[count].id = count + 1;

    snprintf(table[count].name,
             sizeof(table[count].name),
             "%s", name);

    table[count].size = size;

    snprintf(table[count].value,
             sizeof(table[count].value),
             "%s", value);

    snprintf(table[count].section,
             sizeof(table[count].section),
             "%s", sec);

    table[count].address = address;
    table[count].status = status;

    count++;
}



int getDataSize(const char type[])
{
    if (strcmp(type, "db") == 0)
        return 1;

    if (strcmp(type, "dw") == 0)
        return 2;

    if (strcmp(type, "dd") == 0)
        return 4;

    if (strcmp(type, "dq") == 0)
        return 8;

    if (strcmp(type, "dt") == 0)
        return 10;

    return 0;
}



int getValueSize(char token[], int unitSize)
{
    char *item = trim(token);

    if (*item == '\0')
    {
        return 0;
    }

   
    if (*item == '\'' || *item == '"')
    {
        char quote = *item;
        int size = 0;

        item++;

        while (*item != '\0' && *item != quote)
        {
            if (*item == '\\' && item[1] != '\0')
            {
                item++;
            }

            size++;
            item++;
        }

        return size;
    }

   
    return unitSize;
}



int calculateDataSize(const char type[], const char value[])
{
    int unitSize = getDataSize(type);
    int totalSize = 0;

    char token[VALUE_SIZE];
    int tokenLength = 0;
    char quote = '\0';

    for (int i = 0; ; i++)
    {
        char ch = value[i];

        if (quote != '\0')
        {
            if (ch == '\0')
            {
                break;
            }

            if (ch == '\\' && value[i + 1] != '\0')
            {
                if (tokenLength + 2 < VALUE_SIZE)
                {
                    token[tokenLength++] = ch;
                    token[tokenLength++] = value[++i];
                }
                else
                {
                    i++;
                }

                continue;
            }

            if (ch == quote)
            {
                quote = '\0';
            }

            if (tokenLength + 1 < VALUE_SIZE)
            {
                token[tokenLength++] = ch;
            }

            continue;
        }

        if (ch == '\'' || ch == '"')
        {
            quote = ch;

            if (tokenLength + 1 < VALUE_SIZE)
            {
                token[tokenLength++] = ch;
            }

            continue;
        }

       
        if (ch == ',' || ch == '\0')
        {
            token[tokenLength] = '\0';

            totalSize += getValueSize(token, unitSize);

            tokenLength = 0;

            if (ch == '\0')
            {
                break;
            }

            continue;
        }

        if (tokenLength + 1 < VALUE_SIZE)
        {
            token[tokenLength++] = ch;
        }
    }

    return totalSize;
}



int getInstructionSize(const char name[])
{
    if (strcmp(name, "mov") == 0)
        return 5;

    if (strcmp(name, "add") == 0)
        return 5;

    if (strcmp(name, "sub") == 0)
        return 5;

    if (strcmp(name, "jmp") == 0)
        return 5;

    if (strcmp(name, "call") == 0)
        return 5;

    if (strcmp(name, "inc") == 0)
        return 2;

    if (strcmp(name, "dec") == 0)
        return 2;

    if (strcmp(name, "ret") == 0)
        return 1;

    return 1;
}

void toLowerCase(char str[])
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

void checkTarget(const char *instruction,
                 const char *operands)
{
    if (strcmp(instruction, "jmp") != 0 &&
        strcmp(instruction, "call") != 0)
    {
        return;
    }

    char target[NAME_SIZE] = "";
    char extra[NAME_SIZE] = "";

    if (sscanf(operands, "%63s", target) != 1)
    {
        return;
    }

   
    target[strcspn(target, ",")] = '\0';

    if (*target == '\0')
    {
        return;
    }

    /* Ignore numeric targets */
    if (isdigit((unsigned char)target[0]) ||
        target[0] == '-')
    {
        return;
    }

   
    if (strncmp(target, "eax", 3) == 0 ||
        strncmp(target, "ebx", 3) == 0 ||
        strncmp(target, "ecx", 3) == 0 ||
        strncmp(target, "edx", 3) == 0)
    {
        return;
    }

    (void)extra;

    if (searchSymbol(target) == -1)
    {
        insertSymbol(target, 0, "-", "Text", -1, 'U');
    }
}

void processInstruction(char *instructionText)
{
    char instruction[NAME_SIZE] = "";
    char *cursor = instructionText;

    if (!readToken(&cursor, instruction, sizeof(instruction)))
    {
        return;
    }

    toLowerCase(instruction);

    char *operands = trim(cursor);

    checkTarget(instruction, operands);

    textLC += getInstructionSize(instruction);
}

void processLine(char line[])
{
    char *comment = strchr(line, ';');

   
    if (comment != NULL)
    {
        *comment = '\0';
    }

    char *text = trim(line);

    if (*text == '\0')
    {
        return;
    }

    char first[NAME_SIZE] = "";
    char second[NAME_SIZE] = "";

    char *cursor = text;

    if (!readToken(&cursor, first, sizeof(first)))
    {
        return;
    }

   
    if (strcmp(first, "section") == 0)
    {
        if (readToken(&cursor, second, sizeof(second)))
        {
            if (strcmp(second, ".data") == 0)
            {
                strcpy(section, "Data");
            }
            else if (strcmp(second, ".text") == 0)
            {
                strcpy(section, "Text");
            }
        }

        return;
    }

 
    if (strcmp(first, "global") == 0)
    {
        return;
    }

    
    if (strcmp(section, "Data") == 0)
    {
        char name[NAME_SIZE] = "";
        char directive[NAME_SIZE] = "";

        cursor = text;

        if (!readToken(&cursor, name, sizeof(name)) ||
            !readToken(&cursor, directive, sizeof(directive)))
        {
            return;
        }

        int unitSize = getDataSize(directive);

        if (unitSize == 0)
        {
            return;
        }

        char *valueText = trim(cursor);

        char value[VALUE_SIZE] = "-";
        int size = 0;

        if (*valueText != '\0')
        {
            snprintf(value, sizeof(value), "%s", valueText);

            size = calculateDataSize(directive, value);
        }

        insertSymbol(
            name,
            size,
            value,
            "Data",
            dataLC,
            'D'
        );

        dataLC += size;

        return;
    }

    
    if (strcmp(section, "Text") == 0)
    {
        
        char *colon = strchr(text, ':');

        if (colon != NULL)
        {
            *colon = '\0';

            char *label = trim(text);

            if (*label != '\0')
            {
                insertSymbol(
                    label,
                    0,
                    "-",
                    "Text",
                    textLC,
                    'D'
                );
            }

            
            char *afterLabel = trim(colon + 1);

            if (*afterLabel != '\0')
            {
                processInstruction(afterLabel);
            }

            return;
        }

        processInstruction(text);
    }
}



void displayTable(void)
{
    printf("\nSYMBOL TABLE\n");

    printf("------------------------------------------------------------------------------------------------\n");

    printf("%-5s %-18s %-8s %-24s %-12s %-12s %-8s\n",
           "#ID",
           "Name",
           "Size",
           "Value",
           "Section",
           "Address",
           "Status");

    printf("------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        printf("%-5d %-18s %-8d %-24.24s %-12s ",
               table[i].id,
               table[i].name,
               table[i].size,
               table[i].value,
               table[i].section);

        if (table[i].address == -1)
        {
            printf("%-12s ", "-");
        }
        else
        {
            printf("%-12d ", table[i].address);
        }

        printf("%-8c\n", table[i].status);
    }

    printf("------------------------------------------------------------------------------------------------\n");
}



void displayUndefined(void)
{
    int found = 0;

    printf("\nUndefined Symbols:\n");

    for (int i = 0; i < count; i++)
    {
        if (table[i].status == 'U')
        {
            printf("%s\n", table[i].name);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No undefined symbols.\n");
    }
}


int main(void)
{
    FILE *fp;
    char line[512];

    fp = fopen("ass4.asm", "r");

    if (fp == NULL)
    {
        printf("Cannot open ass4.asm\n");
        return 1;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        processLine(line);
    }

    fclose(fp);

    displayTable();
    displayUndefined();

    printf("Assembly symbol-table processing completed.\n");

    return 0;
}
