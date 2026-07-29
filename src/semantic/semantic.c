#include "semantic.h"
#include <stdio.h>
#include <string.h>

SemanticContext* create_semantic_context() {
    SemanticContext* ctx = (SemanticContext*)malloc(sizeof(SemanticContext));
    ctx->symbol_table = create_symbol_table();
    ctx->has_errors = 0;
    return ctx;
}

void analyze_declaration(ASTNode* node, SemanticContext* ctx) {
    DataType type;
    if (strcmp(node->data.declaration.type, "int") == 0) {
        type = TYPE_INT;
    } else if (strcmp(node->data.declaration.type, "float") == 0) {
        type = TYPE_FLOAT;
    } else if (strcmp(node->data.declaration.type, "bool") == 0) {
        type = TYPE_BOOL;
    } else {
        type = TYPE_UNKNOWN;
    }
    
    // For now, we don't have line number in AST node
    // You'll need to add line tracking to your lexer/parser
    if (!add_symbol(ctx->symbol_table, node->data.declaration.name, type, 0)) {
        ctx->has_errors = 1;
    }
}

void analyze_assignment(ASTNode* node, SemanticContext* ctx) {
    Symbol* sym = lookup_symbol(ctx->symbol_table, node->data.assignment.name);
    if (!sym) {
        fprintf(stderr, "Semantic error: variable '%s' not declared\n", 
                node->data.assignment.name);
        ctx->has_errors = 1;
        return;
    }
    
    // Analyze the expression (simplified for now)
    // You need to implement type checking for expressions
}

void analyze_statement(ASTNode* node, SemanticContext* ctx) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_DECLARATION:
            analyze_declaration(node, ctx);
            break;
        case NODE_ASSIGNMENT:
            analyze_assignment(node, ctx);
            break;
        case NODE_IF:
            analyze_statement(node->data.if_stmt.condition, ctx);
            analyze_statement(node->data.if_stmt.then_stmt, ctx);
            if (node->data.if_stmt.else_stmt) {
                analyze_statement(node->data.if_stmt.else_stmt, ctx);
            }
            break;
        case NODE_WHILE:
            analyze_statement(node->data.while_stmt.condition, ctx);
            analyze_statement(node->data.while_stmt.body, ctx);
            break;
        case NODE_BLOCK:
            enter_scope(ctx->symbol_table);
            NodeList* stmt = node->data.block.statements;
            while (stmt) {
                analyze_statement(stmt->node, ctx);
                stmt = stmt->next;
            }
            exit_scope(ctx->symbol_table);
            break;
        // ... handle other node types
        default:
            break;
    }
}

void analyze_program(ASTNode* node, SemanticContext* ctx) {
    if (!node || node->type != NODE_PROGRAM) return;
    
    NodeList* stmt = node->data.program.statements;
    while (stmt) {
        analyze_statement(stmt->node, ctx);
        stmt = stmt->next;
    }
}

void free_semantic_context(SemanticContext* ctx) {
    free_symbol_table(ctx->symbol_table);
    free(ctx);
}
