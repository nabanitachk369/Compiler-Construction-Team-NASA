#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    NODE_PROGRAM,
    NODE_DECLARATION,
    NODE_ASSIGNMENT,
    NODE_IF,
    NODE_WHILE,
    NODE_PRINT,
    NODE_BLOCK,
    NODE_BINARY_OP,
    NODE_UNARY_OP,
    NODE_VARIABLE,
    NODE_INT_LITERAL,
    NODE_FLOAT_LITERAL,
    NODE_BOOL_LITERAL
} NodeType;

typedef struct ASTNode {
    NodeType type;
    int line;
    union {
        struct {
            struct NodeList* statements;
        } program;
        struct {
            char* type;
            char* name;
        } declaration;
        struct {
            char* name;
            struct ASTNode* expr;
        } assignment;
        struct {
            struct ASTNode* condition;
            struct ASTNode* then_stmt;
            struct ASTNode* else_stmt;
        } if_stmt;
        struct {
            struct ASTNode* condition;
            struct ASTNode* body;
        } while_stmt;
        struct {
            struct ASTNode* expr;
        } print_stmt;
        struct {
            struct NodeList* statements;
        } block;
        struct {
            char* operator;
            struct ASTNode* left;
            struct ASTNode* right;
        } binary_op;
        struct {
            char* operator;
            struct ASTNode* operand;
        } unary_op;
        struct {
            char* name;
        } variable;
        struct {
            int value;
        } int_literal;
        struct {
            float value;
        } float_literal;
        struct {
            int value; // 0 for false, 1 for true
        } bool_literal;
    } data;
} ASTNode;

typedef struct NodeList {
    ASTNode* node;
    struct NodeList* next;
} NodeList;

// Constructor functions
ASTNode* create_program(NodeList* statements);
ASTNode* create_declaration(const char* type, const char* name);
ASTNode* create_assignment(const char* name, ASTNode* expr);
ASTNode* create_if(ASTNode* condition, ASTNode* then_stmt, ASTNode* else_stmt);
ASTNode* create_while(ASTNode* condition, ASTNode* body);
ASTNode* create_print(ASTNode* expr);
ASTNode* create_block(NodeList* statements);
ASTNode* create_binary_op(const char* op, ASTNode* left, ASTNode* right);
ASTNode* create_unary_op(const char* op, ASTNode* operand);
ASTNode* create_variable(const char* name);
ASTNode* create_integer_literal(int value);
ASTNode* create_float_literal(float value);
ASTNode* create_bool_literal(int value);

NodeList* append_node(NodeList* list, ASTNode* node);

// Print AST (for debugging)
void print_ast(ASTNode* node, int indent);

// Free AST
void free_ast(ASTNode* node);

#endif
