#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

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

extern struct Symbol table[MAX];

extern int count;
extern int dataLC;
extern int textLC;
extern char section[20];

char *trim(char *str);

int searchSymbol(char *name);

void insertSymbol(char *name, int size, char *value,
                  char *sec, int address, char status);

int getDataSize(char *type);
int getValueSize(char *value);
int calculateDataSize(char *type, char *value);

void displayTable(void);
void displayUndefined(void);

int readToken(char *line, int *pos,
              char *token, int capacity);

int getInstructionSize(char *mnemonic);

void toLowerCase(char *str);
void checkTarget(char *line, int pos);
void processInstruction(char *line);
void processLine(char *line);

#endif

