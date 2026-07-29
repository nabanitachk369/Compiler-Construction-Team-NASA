#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "../ast/ast.h"
#include "../symbol_table/symbol_table.h"

typedef struct {
    SymbolTable* symbol_table;
    int has_errors;
} SemanticContext;

SemanticContext* create_semantic_context();
void analyze_program(ASTNode* node, SemanticContext* context);
void free_semantic_context(SemanticContext* context);

#endif
