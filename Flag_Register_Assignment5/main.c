#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 512
#define LINE_LEN 256
#define MAX_LABELS 128
#define LABEL_LEN 64

typedef struct {
    char text[LINE_LEN];
    int source_line;
} Instruction;

typedef struct {
    char name[LABEL_LEN];
    int address;
} Label;

typedef struct {
    uint32_t eax, ebx, ecx, edx;
    int zf, sf, cf;
} CPU;

static Instruction program[MAX_LINES];
static Label labels[MAX_LABELS];
static int instruction_count = 0, label_count = 0;

static void trim(char *s) {
    size_t n;
    char *p = s;
    while (isspace((unsigned char)*p)) p++;
    if (p != s) memmove(s, p, strlen(p) + 1);
    n = strlen(s);
    while (n && isspace((unsigned char)s[n - 1])) s[--n] = '\0';
}

static void remove_comment(char *s) {
    char *p1 = strchr(s, ';');
    char *p2 = strchr(s, '#');
    char *p = p1;
    if (!p || (p2 && p2 < p)) p = p2;
    if (p) *p = '\0';
}

static void uppercase(char *s) {
    for (; *s; ++s) *s = (char)toupper((unsigned char)*s);
}

static int find_label(const char *name) {
    for (int i = 0; i < label_count; ++i)
        if (strcmp(labels[i].name, name) == 0) return labels[i].address;
    return -1;
}

static int add_label(const char *name, int address, int source_line) {
    if (label_count >= MAX_LABELS) {
        fprintf(stderr, "Line %d: too many labels\n", source_line);
        return 0;
    }
    if (find_label(name) >= 0) {
        fprintf(stderr, "Line %d: duplicate label '%s'\n", source_line, name);
        return 0;
    }
    if (strlen(name) >= LABEL_LEN) {
        fprintf(stderr, "Line %d: label name too long\n", source_line);
        return 0;
    }
    strcpy(labels[label_count].name, name);
    labels[label_count++].address = address;
    return 1;
}


static int assemble(FILE *in) {
    char line[LINE_LEN];
    int source_line = 0;

    while (fgets(line, sizeof line, in)) {
        source_line++;
        if (!strchr(line, '\n') && !feof(in)) {
            fprintf(stderr, "Line %d: line too long\n", source_line);
            return 0;
        }
        remove_comment(line);
        trim(line);
        if (!line[0]) continue;

       
        char *colon = strchr(line, ':');
        if (colon) {
            *colon = '\0';
            trim(line);
            uppercase(line);
            if (!line[0] || !add_label(line, instruction_count, source_line))
                return 0;
            memmove(line, colon + 1, strlen(colon + 1) + 1);
            trim(line);
            if (!line[0]) continue;
        }

        if (instruction_count >= MAX_LINES) {
            fprintf(stderr, "Line %d: program too large\n", source_line);
            return 0;
        }
        uppercase(line);
       
        memcpy(program[instruction_count].text, line, strlen(line) + 1);
        program[instruction_count].source_line = source_line;
        instruction_count++;
    }

   
    for (int i = 0; i < instruction_count; ++i) {
        char op[16] = "", arg1[80] = "", arg2[80] = "";
        int fields = sscanf(program[i].text, "%15s %79[^,], %79s", op, arg1, arg2);
        if (!strcmp(op, "JMP") || !strcmp(op, "JNE") || !strcmp(op, "JE")) {
            char target[80];
            if (sscanf(program[i].text, "%15s %79s", op, target) != 2 ||
                find_label(target) < 0) {
                fprintf(stderr, "Line %d: unknown or missing jump label\n",
                        program[i].source_line);
                return 0;
            }
        }
        (void)fields; (void)arg1; (void)arg2;
    }
    return 1;
}

static uint32_t *register_ptr(CPU *cpu, const char *name) {
    if (!strcmp(name, "EAX")) return &cpu->eax;
    if (!strcmp(name, "EBX")) return &cpu->ebx;
    if (!strcmp(name, "ECX")) return &cpu->ecx;
    if (!strcmp(name, "EDX")) return &cpu->edx;
    return NULL;
}

static int parse_value(CPU *cpu, const char *token, uint32_t *out) {
    uint32_t *r = register_ptr(cpu, token);
    if (r) { *out = *r; return 1; }

    errno = 0;
    char *end = NULL;
    unsigned long long value = strtoull(token, &end, 0);
    if (errno || end == token || *end != '\0' || value > UINT32_MAX)
        return 0;
    *out = (uint32_t)value;
    return 1;
}

static void update_zf_sf(CPU *cpu, uint32_t result) {
    cpu->zf = (result == 0);
    cpu->sf = (int)((result >> 31) & 1U);
}

static int copy_token(char *destination, size_t destination_size,
                      const char *source) {
    size_t length = strlen(source);
    if (length >= destination_size) return 0;
    memcpy(destination, source, length + 1);
    return 1;
}

static int split_instruction(const char *text, char *op, size_t op_size,
                             char *a, size_t a_size, char *b, size_t b_size) {
    char copy[LINE_LEN];
    size_t text_length = strlen(text);
    if (text_length >= sizeof copy) return 0;
    memcpy(copy, text, text_length + 1);
    trim(copy);

    char *space = strpbrk(copy, " \t");
    if (!space) {
        if (!copy_token(op, op_size, copy)) return 0;
        a[0] = b[0] = '\0';
        return 1;
    }

    *space++ = '\0';
    if (!copy_token(op, op_size, copy)) return 0;
    trim(space);
    char *comma = strchr(space, ',');
    if (comma) {
        *comma++ = '\0';
        trim(space);
        trim(comma);
        if (!copy_token(a, a_size, space) ||
            !copy_token(b, b_size, comma)) return 0;
    } else {
        if (!copy_token(a, a_size, space)) return 0;
        b[0] = '\0';
    }
    return 1;
}

static void print_state(const CPU *cpu) {
    printf("EAX=%08" PRIX32 " EBX=%08" PRIX32
           " ECX=%08" PRIX32 " EDX=%08" PRIX32 "\n",
           cpu->eax, cpu->ebx, cpu->ecx, cpu->edx);
    printf("FLAGS: ZF=%d SF=%d CF=%d\n", cpu->zf, cpu->sf, cpu->cf);
}

static int execute(CPU *cpu) {
    int pc = 0;
    unsigned long steps = 0;
    const unsigned long max_steps = 100000;

    while (pc >= 0 && pc < instruction_count) {
        if (++steps > max_steps) {
            fprintf(stderr, "Execution stopped: step limit reached (possible loop)\n");
            return 0;
        }

        Instruction *ins = &program[pc];
        char op[16], a[80], b[80];
        if (!split_instruction(ins->text, op, sizeof op, a, sizeof a, b, sizeof b)) {
            fprintf(stderr, "Line %d: instruction or operand is too long\\n",
                    ins->source_line);
            return 0;
        }
        int next_pc = pc + 1;
        uint32_t *dest = NULL, lhs = 0, rhs = 0, result = 0;

        if (!strcmp(op, "HLT")) {
            printf("HLT encountered at instruction %d.\n", pc);
            break;
        } else if (!strcmp(op, "MOV")) {
            dest = register_ptr(cpu, a);
            if (!dest || !parse_value(cpu, b, &rhs)) goto bad_operands;
            *dest = rhs;
        } else if (!strcmp(op, "ADD")) {
            dest = register_ptr(cpu, a);
            if (!dest || !parse_value(cpu, a, &lhs) || !parse_value(cpu, b, &rhs))
                goto bad_operands;
            uint64_t wide = (uint64_t)lhs + (uint64_t)rhs;
            result = (uint32_t)wide;
            cpu->cf = (wide > UINT32_MAX);
            *dest = result;
            update_zf_sf(cpu, result);
        } else if (!strcmp(op, "SUB") || !strcmp(op, "CMP")) {
            dest = register_ptr(cpu, a);
            if (!dest || !parse_value(cpu, a, &lhs) || !parse_value(cpu, b, &rhs))
                goto bad_operands;
            result = lhs - rhs; /* unsigned 32-bit wraparound */
            cpu->cf = (lhs < rhs); /* x86 subtraction CF indicates borrow */
            update_zf_sf(cpu, result);
            if (!strcmp(op, "SUB")) *dest = result;
        } else if (!strcmp(op, "JMP") || !strcmp(op, "JNE") || !strcmp(op, "JE")) {
            int take = !strcmp(op, "JMP") ||
                       (!strcmp(op, "JNE") && cpu->zf == 0) ||
                       (!strcmp(op, "JE") && cpu->zf == 1);
            if (take) next_pc = find_label(a);
        } else {
            fprintf(stderr, "Line %d: unsupported instruction '%s'\n",
                    ins->source_line, op);
            return 0;
        }
        pc = next_pc;
        continue;

bad_operands:
        fprintf(stderr, "Line %d: invalid operands in '%s'\n",
                ins->source_line, ins->text);
        return 0;
    }
    printf("\nFinal CPU state:\n");
    print_state(cpu);
    return 1;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <assembly-file>\n", argv[0]);
        fprintf(stderr, "Registers: EAX, EBX, ECX, EDX; immediates: decimal or 0xHEX\n");
        return EXIT_FAILURE;
    }
    FILE *in = fopen(argv[1], "r");
    if (!in) {
        perror(argv[1]);
        return EXIT_FAILURE;
    }
    int ok = assemble(in);
    fclose(in);
    if (!ok) return EXIT_FAILURE;

    CPU cpu = {0};
    printf("Loaded %d instructions and %d labels.\n\n", instruction_count, label_count);
    if (!execute(&cpu)) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
