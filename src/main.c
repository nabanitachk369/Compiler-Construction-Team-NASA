#include <stdio.h>
#include <stdlib.h>
#include "ast/ast.h"
#include "symbol_table/symbol_table.h"
#include "semantic/semantic.h"
#include "codegen/tac.h"

extern int yyparse();
extern ASTNode* ast_root;
extern int yylineno;

// Function declarations
void yyerror(const char* s);

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source_file>\n", argv[0]);
        return 1;
    }
    
    // Open input file
    extern FILE* yyin;
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", argv[1]);
        return 1;
    }
    
    printf("=== Compiling %s ===\n", argv[1]);
    printf("Starting lexical and syntax analysis...\n");
    
    // Parse
    int result = yyparse();
    if (result != 0) {
        fprintf(stderr, "Compilation failed: syntax errors detected\n");
        fclose(yyin);
        return 1;
    }
    
    printf("Parsing successful!\n");
    
    // Print AST
    printf("\n--- Abstract Syntax Tree ---\n");
    print_ast(ast_root, 0);
    
    // Semantic analysis
    printf("\n--- Semantic Analysis ---\n");
    SemanticContext* sem_ctx = create_semantic_context();
    analyze_program(ast_root, sem_ctx);
    
    if (sem_ctx->has_errors) {
        fprintf(stderr, "Compilation failed: semantic errors detected\n");
        free_semantic_context(sem_ctx);
        fclose(yyin);
        return 1;
    }
    
    print_symbol_table(sem_ctx->symbol_table);
    printf("Semantic analysis passed!\n");
    
    // Generate TAC
    printf("\n--- Intermediate Code Generation ---\n");
    TACGenerator* tac_gen = create_tac_generator();
    generate_tac(ast_root, tac_gen);
    print_tac(tac_gen);
    
    // Cleanup
    free_semantic_context(sem_ctx);
    free_tac(tac_gen);
    free_ast(ast_root);
    fclose(yyin);
    
    printf("Compilation completed successfully!\n");
    return 0;
}
