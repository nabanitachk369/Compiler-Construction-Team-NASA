#include "semantic.h"
#include <stdio.h>
#include <string.h>

static const char* data_type_to_string(DataType type) {
    switch (type) {
        case TYPE_INT: return "int";
        case TYPE_FLOAT: return "float";
        case TYPE_BOOL: return "bool";
        default: return "unknown";
    }
}

static int is_compatible_type(DataType left, DataType right) {
    if (left == TYPE_UNKNOWN || right == TYPE_UNKNOWN) {
        return 1;
    }
    if (left == TYPE_FLOAT || right == TYPE_FLOAT) {
        return left == TYPE_FLOAT || right == TYPE_FLOAT;
    }
    return left == right;
}

static DataType infer_expression_type(ASTNode* node, SemanticContext* ctx) {
    if (!node) return TYPE_UNKNOWN;

    switch (node->type) {
        case NODE_VARIABLE: {
            Symbol* sym = lookup_symbol(ctx->symbol_table, node->data.variable.name);
            if (!sym) {
                fprintf(stderr, "Semantic error at line %d: variable '%s' not declared\n",
                        node->line, node->data.variable.name);
                ctx->has_errors = 1;
                return TYPE_UNKNOWN;
            }
            if (!is_symbol_in_current_scope(ctx->symbol_table, node->data.variable.name) &&
                lookup_in_current_scope(ctx->symbol_table, node->data.variable.name) == NULL) {
                fprintf(stderr, "Semantic error at line %d: variable '%s' is out of scope\n",
                        node->line, node->data.variable.name);
                ctx->has_errors = 1;
            }
            return sym->type;
        }
        case NODE_INT_LITERAL:
            return TYPE_INT;
        case NODE_FLOAT_LITERAL:
            return TYPE_FLOAT;
        case NODE_BOOL_LITERAL:
            return TYPE_BOOL;
        case NODE_UNARY_OP:
            return infer_expression_type(node->data.unary_op.operand, ctx);
        case NODE_BINARY_OP: {
            DataType left = infer_expression_type(node->data.binary_op.left, ctx);
            DataType right = infer_expression_type(node->data.binary_op.right, ctx);
            if (!is_compatible_type(left, right)) {
                fprintf(stderr, "Semantic error at line %d: incompatible types in expression\n", node->line);
                ctx->has_errors = 1;
            }
            if (strcmp(node->data.binary_op.operator, "&&") == 0 ||
                strcmp(node->data.binary_op.operator, "||") == 0) {
                return TYPE_BOOL;
            }
            if (strcmp(node->data.binary_op.operator, "==") == 0 ||
                strcmp(node->data.binary_op.operator, "!=") == 0 ||
                strcmp(node->data.binary_op.operator, "<") == 0 ||
                strcmp(node->data.binary_op.operator, ">") == 0 ||
                strcmp(node->data.binary_op.operator, "<=") == 0 ||
                strcmp(node->data.binary_op.operator, ">=") == 0) {
                return TYPE_BOOL;
            }
            if (left == TYPE_FLOAT || right == TYPE_FLOAT) {
                return TYPE_FLOAT;
            }
            return TYPE_INT;
        }
        default:
            return TYPE_UNKNOWN;
    }
}

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

    if (!add_symbol(ctx->symbol_table, node->data.declaration.name, type, node->line)) {
        ctx->has_errors = 1;
    }
}

void analyze_assignment(ASTNode* node, SemanticContext* ctx) {
    Symbol* sym = lookup_symbol(ctx->symbol_table, node->data.assignment.name);
    if (!sym) {
        fprintf(stderr, "Semantic error at line %d: variable '%s' not declared\n",
                node->line, node->data.assignment.name);
        ctx->has_errors = 1;
        return;
    }

    DataType expr_type = infer_expression_type(node->data.assignment.expr, ctx);
    if (!is_compatible_type(sym->type, expr_type)) {
        fprintf(stderr, "Semantic error at line %d: cannot assign %s to %s\n",
                node->line, data_type_to_string(expr_type), data_type_to_string(sym->type));
        ctx->has_errors = 1;
    }
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
            infer_expression_type(node->data.if_stmt.condition, ctx);
            analyze_statement(node->data.if_stmt.then_stmt, ctx);
            if (node->data.if_stmt.else_stmt) {
                analyze_statement(node->data.if_stmt.else_stmt, ctx);
            }
            break;
        case NODE_WHILE:
            infer_expression_type(node->data.while_stmt.condition, ctx);
            analyze_statement(node->data.while_stmt.body, ctx);
            break;
        case NODE_PRINT:
            infer_expression_type(node->data.print_stmt.expr, ctx);
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
        default:
            infer_expression_type(node, ctx);
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
