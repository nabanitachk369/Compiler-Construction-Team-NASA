#include "tac.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ===== CREATE TAC GENERATOR ===== */
TACGenerator* create_tac_generator() {
    TACGenerator* gen = (TACGenerator*)malloc(sizeof(TACGenerator));
    gen->head = NULL;
    gen->tail = NULL;
    gen->temp_counter = 0;
    gen->label_counter = 0;
    return gen;
}

/* ===== HELPER FUNCTIONS ===== */
char* new_temp(TACGenerator* gen) {
    char* temp = (char*)malloc(16);
    sprintf(temp, "t%d", gen->temp_counter++);
    return temp;
}

char* new_label(TACGenerator* gen) {
    char* label = (char*)malloc(16);
    sprintf(label, "L%d", gen->label_counter++);
    return label;
}

/* ===== ADD INSTRUCTION ===== */
void add_instruction(TACGenerator* gen, TACType type, char* result, 
                     char* arg1, char* arg2, char* operator) {
    TACInstruction* instr = (TACInstruction*)malloc(sizeof(TACInstruction));
    instr->type = type;
    instr->result = result ? strdup(result) : NULL;
    instr->arg1 = arg1 ? strdup(arg1) : NULL;
    instr->arg2 = arg2 ? strdup(arg2) : NULL;
    instr->operator = operator ? strdup(operator) : NULL;
    instr->next = NULL;
    
    if (gen->head == NULL) {
        gen->head = instr;
        gen->tail = instr;
    } else {
        gen->tail->next = instr;
        gen->tail = instr;
    }
}

/* ===== GENERATE EXPRESSION ===== */
void generate_expression(ASTNode* node, TACGenerator* gen, char** result) {
    if (!node) {
        *result = NULL;
        return;
    }
    
    switch (node->type) {
        case NODE_VARIABLE:
            *result = strdup(node->data.variable.name);
            break;
            
        case NODE_INT_LITERAL: {
            char* val = (char*)malloc(32);
            sprintf(val, "%d", node->data.int_literal.value);
            *result = val;
            break;
        }
        
        case NODE_FLOAT_LITERAL: {
            char* val = (char*)malloc(32);
            sprintf(val, "%f", node->data.float_literal.value);
            *result = val;
            break;
        }
        
        case NODE_BOOL_LITERAL:
            *result = strdup(node->data.bool_literal.value ? "1" : "0");
            break;
            
        case NODE_BINARY_OP: {
            char* left_result;
            char* right_result;
            generate_expression(node->data.binary_op.left, gen, &left_result);
            generate_expression(node->data.binary_op.right, gen, &right_result);
            
            char* temp = new_temp(gen);
            add_instruction(gen, TAC_BINARY, temp, left_result, right_result, 
                          node->data.binary_op.operator);
            
            *result = temp;
            break;
        }
        
        case NODE_UNARY_OP: {
            char* operand_result;
            generate_expression(node->data.unary_op.operand, gen, &operand_result);
            
            char* temp = new_temp(gen);
            add_instruction(gen, TAC_UNARY, temp, operand_result, NULL, 
                          node->data.unary_op.operator);
            
            *result = temp;
            break;
        }
        
        default:
            *result = NULL;
    }
}

/* ===== GENERATE STATEMENT ===== */
void generate_statement(ASTNode* node, TACGenerator* gen) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_ASSIGNMENT: {
            char* result;
            generate_expression(node->data.assignment.expr, gen, &result);
            add_instruction(gen, TAC_ASSIGN, strdup(node->data.assignment.name), 
                          result, NULL, NULL);
            break;
        }
        
        case NODE_PRINT: {
            char* result;
            generate_expression(node->data.print_stmt.expr, gen, &result);
            add_instruction(gen, TAC_PRINT, NULL, result, NULL, NULL);
            break;
        }
        
        case NODE_IF: {
            char* cond_result;
            generate_expression(node->data.if_stmt.condition, gen, &cond_result);
            
            char* label_false = new_label(gen);
            char* label_end = new_label(gen);
            
            add_instruction(gen, TAC_IF_GOTO, NULL, cond_result, label_false, "==");
            
            generate_statement(node->data.if_stmt.then_stmt, gen);
            add_instruction(gen, TAC_GOTO, NULL, NULL, label_end, NULL);
            
            add_instruction(gen, TAC_LABEL, label_false, NULL, NULL, NULL);
            
            if (node->data.if_stmt.else_stmt) {
                generate_statement(node->data.if_stmt.else_stmt, gen);
            }
            
            add_instruction(gen, TAC_LABEL, label_end, NULL, NULL, NULL);
            break;
        }
        
        case NODE_WHILE: {
            char* label_start = new_label(gen);
            char* label_end = new_label(gen);
            
            add_instruction(gen, TAC_LABEL, label_start, NULL, NULL, NULL);
            
            char* cond_result;
            generate_expression(node->data.while_stmt.condition, gen, &cond_result);
            add_instruction(gen, TAC_IF_GOTO, NULL, cond_result, label_end, "==");
            
            generate_statement(node->data.while_stmt.body, gen);
            add_instruction(gen, TAC_GOTO, NULL, NULL, label_start, NULL);
            add_instruction(gen, TAC_LABEL, label_end, NULL, NULL, NULL);
            break;
        }
        
        case NODE_BLOCK: {
            NodeList* stmt = node->data.block.statements;
            while (stmt) {
                generate_statement(stmt->node, gen);
                stmt = stmt->next;
            }
            break;
        }
        
        default:
            break;
    }
}

/* ===== GENERATE TAC ===== */
void generate_tac(ASTNode* node, TACGenerator* gen) {
    if (!node || node->type != NODE_PROGRAM) return;
    
    NodeList* stmt = node->data.program.statements;
    while (stmt) {
        generate_statement(stmt->node, gen);
        stmt = stmt->next;
    }
}

/* ===== PRINT TAC ===== */
void print_tac(TACGenerator* gen) {
    printf("\n========== THREE-ADDRESS CODE ==========\n");
    TACInstruction* instr = gen->head;
    int count = 0;
    
    while (instr) {
        printf("%3d: ", count++);
        
        switch (instr->type) {
            case TAC_ASSIGN:
                printf("%s = %s\n", instr->result, instr->arg1);
                break;
            case TAC_BINARY:
                printf("%s = %s %s %s\n", instr->result, instr->arg1, 
                       instr->operator, instr->arg2);
                break;
            case TAC_UNARY:
                printf("%s = %s %s\n", instr->result, instr->operator, instr->arg1);
                break;
            case TAC_LABEL:
                printf("%s:\n", instr->result);
                break;
            case TAC_GOTO:
                printf("goto %s\n", instr->arg2);
                break;
            case TAC_IF_GOTO:
                printf("if %s %s 0 goto %s\n", instr->arg1, instr->operator, instr->arg2);
                break;
            case TAC_PRINT:
                printf("print %s\n", instr->arg1);
                break;
            default:
                printf("Unknown instruction\n");
        }
        
        instr = instr->next;
    }
    printf("========================================\n\n");
}

/* ===== FREE TAC ===== */
void free_tac(TACGenerator* gen) {
    TACInstruction* instr = gen->head;
    while (instr) {
        TACInstruction* next = instr->next;
        if (instr->result) free(instr->result);
        if (instr->arg1) free(instr->arg1);
        if (instr->arg2) free(instr->arg2);
        if (instr->operator) free(instr->operator);
        free(instr);
        instr = next;
    }
    free(gen);
}
