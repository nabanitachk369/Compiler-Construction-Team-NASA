#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode* create_node(NodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    memset(node, 0, sizeof(ASTNode));
    node->type = type;
    node->line = 0;
    return node;
}

ASTNode* create_program(NodeList* statements) {
    ASTNode* node = create_node(NODE_PROGRAM);
    node->data.program.statements = statements;
    return node;
}

ASTNode* create_declaration(const char* type, const char* name) {
    ASTNode* node = create_node(NODE_DECLARATION);
    node->data.declaration.type = strdup(type);
    node->data.declaration.name = strdup(name);
    return node;
}

ASTNode* create_assignment(const char* name, ASTNode* expr) {
    ASTNode* node = create_node(NODE_ASSIGNMENT);
    node->data.assignment.name = strdup(name);
    node->data.assignment.expr = expr;
    return node;
}

ASTNode* create_if(ASTNode* condition, ASTNode* then_stmt, ASTNode* else_stmt) {
    ASTNode* node = create_node(NODE_IF);
    node->data.if_stmt.condition = condition;
    node->data.if_stmt.then_stmt = then_stmt;
    node->data.if_stmt.else_stmt = else_stmt;
    return node;
}

ASTNode* create_while(ASTNode* condition, ASTNode* body) {
    ASTNode* node = create_node(NODE_WHILE);
    node->data.while_stmt.condition = condition;
    node->data.while_stmt.body = body;
    return node;
}

ASTNode* create_print(ASTNode* expr) {
    ASTNode* node = create_node(NODE_PRINT);
    node->data.print_stmt.expr = expr;
    return node;
}

ASTNode* create_block(NodeList* statements) {
    ASTNode* node = create_node(NODE_BLOCK);
    node->data.block.statements = statements;
    return node;
}

ASTNode* create_binary_op(const char* op, ASTNode* left, ASTNode* right) {
    ASTNode* node = create_node(NODE_BINARY_OP);
    node->data.binary_op.operator = strdup(op);
    node->data.binary_op.left = left;
    node->data.binary_op.right = right;
    return node;
}

ASTNode* create_unary_op(const char* op, ASTNode* operand) {
    ASTNode* node = create_node(NODE_UNARY_OP);
    node->data.unary_op.operator = strdup(op);
    node->data.unary_op.operand = operand;
    return node;
}

ASTNode* create_variable(const char* name) {
    ASTNode* node = create_node(NODE_VARIABLE);
    node->data.variable.name = strdup(name);
    return node;
}

ASTNode* create_integer_literal(int value) {
    ASTNode* node = create_node(NODE_INT_LITERAL);
    node->data.int_literal.value = value;
    return node;
}

ASTNode* create_float_literal(float value) {
    ASTNode* node = create_node(NODE_FLOAT_LITERAL);
    node->data.float_literal.value = value;
    return node;
}

ASTNode* create_bool_literal(int value) {
    ASTNode* node = create_node(NODE_BOOL_LITERAL);
    node->data.bool_literal.value = value;
    return node;
}

NodeList* append_node(NodeList* list, ASTNode* node) {
    NodeList* new_node = (NodeList*)malloc(sizeof(NodeList));
    new_node->node = node;
    new_node->next = NULL;
    
    if (list == NULL) return new_node;
    
    NodeList* current = list;
    while (current->next) current = current->next;
    current->next = new_node;
    return list;
}

void print_indent(int indent) {
    for (int i = 0; i < indent; i++) printf("  ");
}

void print_ast(ASTNode* node, int indent) {
    if (!node) {
        print_indent(indent);
        printf("NULL\n");
        return;
    }
    
    print_indent(indent);
    
    switch (node->type) {
        case NODE_PROGRAM:
            printf("Program\n");
            NodeList* stmt = node->data.program.statements;
            while (stmt) {
                print_ast(stmt->node, indent + 1);
                stmt = stmt->next;
            }
            break;
            
        case NODE_DECLARATION:
            printf("Declaration: %s %s\n", 
                   node->data.declaration.type,
                   node->data.declaration.name);
            break;
            
        case NODE_ASSIGNMENT:
            printf("Assignment: %s =\n", node->data.assignment.name);
            print_ast(node->data.assignment.expr, indent + 1);
            break;
            
        case NODE_IF:
            printf("If\n");
            print_indent(indent + 1);
            printf("Condition:\n");
            print_ast(node->data.if_stmt.condition, indent + 2);
            print_indent(indent + 1);
            printf("Then:\n");
            print_ast(node->data.if_stmt.then_stmt, indent + 2);
            if (node->data.if_stmt.else_stmt) {
                print_indent(indent + 1);
                printf("Else:\n");
                print_ast(node->data.if_stmt.else_stmt, indent + 2);
            }
            break;
            
        case NODE_WHILE:
            printf("While\n");
            print_indent(indent + 1);
            printf("Condition:\n");
            print_ast(node->data.while_stmt.condition, indent + 2);
            print_indent(indent + 1);
            printf("Body:\n");
            print_ast(node->data.while_stmt.body, indent + 2);
            break;
            
        case NODE_PRINT:
            printf("Print\n");
            print_ast(node->data.print_stmt.expr, indent + 1);
            break;
            
        case NODE_BLOCK:
            printf("Block\n");
            NodeList* stmt2 = node->data.block.statements;
            while (stmt2) {
                print_ast(stmt2->node, indent + 1);
                stmt2 = stmt2->next;
            }
            break;
            
        case NODE_BINARY_OP:
            printf("Binary: %s\n", node->data.binary_op.operator);
            print_indent(indent + 1);
            printf("Left:\n");
            print_ast(node->data.binary_op.left, indent + 2);
            print_indent(indent + 1);
            printf("Right:\n");
            print_ast(node->data.binary_op.right, indent + 2);
            break;
            
        case NODE_UNARY_OP:
            printf("Unary: %s\n", node->data.unary_op.operator);
            print_ast(node->data.unary_op.operand, indent + 1);
            break;
            
        case NODE_VARIABLE:
            printf("Variable: %s\n", node->data.variable.name);
            break;
            
        case NODE_INT_LITERAL:
            printf("Integer: %d\n", node->data.int_literal.value);
            break;
            
        case NODE_FLOAT_LITERAL:
            printf("Float: %f\n", node->data.float_literal.value);
            break;
            
        case NODE_BOOL_LITERAL:
            printf("Boolean: %s\n", node->data.bool_literal.value ? "true" : "false");
            break;
            
        default:
            printf("Unknown node type\n");
    }
}

void free_ast(ASTNode* node) {
    if (!node) return;

    switch (node->type) {
        case NODE_PROGRAM:
        case NODE_BLOCK: {
            NodeList* stmt = node->data.program.statements;
            while (stmt) {
                NodeList* next = stmt->next;
                free_ast(stmt->node);
                free(stmt);
                stmt = next;
            }
            break;
        }
        case NODE_DECLARATION:
            free(node->data.declaration.type);
            free(node->data.declaration.name);
            break;
        case NODE_ASSIGNMENT:
            free(node->data.assignment.name);
            free_ast(node->data.assignment.expr);
            break;
        case NODE_IF:
            free_ast(node->data.if_stmt.condition);
            free_ast(node->data.if_stmt.then_stmt);
            free_ast(node->data.if_stmt.else_stmt);
            break;
        case NODE_WHILE:
            free_ast(node->data.while_stmt.condition);
            free_ast(node->data.while_stmt.body);
            break;
        case NODE_PRINT:
            free_ast(node->data.print_stmt.expr);
            break;
        case NODE_BINARY_OP:
            free(node->data.binary_op.operator);
            free_ast(node->data.binary_op.left);
            free_ast(node->data.binary_op.right);
            break;
        case NODE_UNARY_OP:
            free(node->data.unary_op.operator);
            free_ast(node->data.unary_op.operand);
            break;
        case NODE_VARIABLE:
            free(node->data.variable.name);
            break;
        case NODE_INT_LITERAL:
        case NODE_FLOAT_LITERAL:
        case NODE_BOOL_LITERAL:
        default:
            break;
    }

    free(node);
}
