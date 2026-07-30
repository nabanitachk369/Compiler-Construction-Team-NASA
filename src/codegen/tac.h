#ifndef TAC_H
#define TAC_H

#include "../ast/ast.h"

/* ===== TAC INSTRUCTION TYPES ===== */
typedef enum {
    TAC_ASSIGN,
    TAC_BINARY,
    TAC_UNARY,
    TAC_LABEL,
    TAC_GOTO,
    TAC_IF_GOTO,
    TAC_PRINT
} TACType;

/* ===== TAC INSTRUCTION ===== */
typedef struct TACInstruction {
    TACType type;
    char* result;
    char* arg1;
    char* arg2;
    char* operator;
    struct TACInstruction* next;
} TACInstruction;

/* ===== TAC GENERATOR ===== */
typedef struct {
    TACInstruction* head;
    TACInstruction* tail;
    int temp_counter;
    int label_counter;
} TACGenerator;

/* ===== FUNCTION PROTOTYPES ===== */
TACGenerator* create_tac_generator();
void generate_tac(ASTNode* node, TACGenerator* gen);
void print_tac(TACGenerator* gen);
void free_tac(TACGenerator* gen);

char* new_temp(TACGenerator* gen);
char* new_label(TACGenerator* gen);

#endif
