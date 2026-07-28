#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ===== CREATE SYMBOL TABLE ===== */
SymbolTable* create_symbol_table() {
    SymbolTable* st = (SymbolTable*)malloc(sizeof(SymbolTable));
    st->current_scope = NULL;
    st->error_count = 0;
    enter_scope(st); // Create global scope
    return st;
}

/* ===== SCOPE MANAGEMENT ===== */
void enter_scope(SymbolTable* st) {
    Scope* scope = (Scope*)malloc(sizeof(Scope));
    scope->symbols = NULL;
    scope->parent = st->current_scope;
    st->current_scope = scope;
}

void exit_scope(SymbolTable* st) {
    if (!st->current_scope) return;
    
    // Free all symbols in this scope
    Symbol* sym = st->current_scope->symbols;
    while (sym) {
        Symbol* next = sym->next;
        free(sym->name);
        free(sym);
        sym = next;
    }
    
    Scope* parent = st->current_scope->parent;
    free(st->current_scope);
    st->current_scope = parent;
}

/* ===== ADD SYMBOL ===== */
int add_symbol(SymbolTable* st, const char* name, DataType type, int line) {
    // Check if already declared in current scope
    if (lookup_in_current_scope(st, name)) {
        fprintf(stderr, "Semantic error at line %d: variable '%s' already declared in this scope\n",
                line, name);
        st->error_count++;
        return 0;
    }
    
    Symbol* sym = (Symbol*)malloc(sizeof(Symbol));
    sym->name = strdup(name);
    sym->type = type;
    sym->line_declared = line;
    sym->next = st->current_scope->symbols;
    st->current_scope->symbols = sym;
    return 1;
}

/* ===== LOOKUP SYMBOL ===== */
Symbol* lookup_symbol(SymbolTable* st, const char* name) {
    Scope* scope = st->current_scope;
    while (scope) {
        Symbol* sym = scope->symbols;
        while (sym) {
            if (strcmp(sym->name, name) == 0) {
                return sym;
            }
            sym = sym->next;
        }
        scope = scope->parent;
    }
    return NULL;
}

Symbol* lookup_in_current_scope(SymbolTable* st, const char* name) {
    if (!st->current_scope) return NULL;
    
    Symbol* sym = st->current_scope->symbols;
    while (sym) {
        if (strcmp(sym->name, name) == 0) {
            return sym;
        }
        sym = sym->next;
    }
    return NULL;
}

/* ===== PRINT SYMBOL TABLE ===== */
void print_symbol_table(SymbolTable* st) {
    printf("\n========== SYMBOL TABLE ==========\n");
    
    // Collect scopes (from innermost to outermost)
    Scope* scopes[100];
    int count = 0;
    Scope* current = st->current_scope;
    while (current && count < 100) {
        scopes[count++] = current;
        current = current->parent;
    }
    
    // Print from outermost to innermost
    for (int i = count - 1; i >= 0; i--) {
        printf("Scope level %d:\n", count - 1 - i);
        Symbol* sym = scopes[i]->symbols;
        if (!sym) {
            printf("  (empty)\n");
        }
        while (sym) {
            const char* type_str;
            switch (sym->type) {
                case TYPE_INT: type_str = "int"; break;
                case TYPE_FLOAT: type_str = "float"; break;
                case TYPE_BOOL: type_str = "bool"; break;
                default: type_str = "unknown";
            }
            printf("  %s\t%s\t(line %d)\n", sym->name, type_str, sym->line_declared);
            sym = sym->next;
        }
    }
    printf("==================================\n\n");
}

/* ===== FREE SYMBOL TABLE ===== */
void free_symbol_table(SymbolTable* st) {
    while (st->current_scope) {
        exit_scope(st);
    }
    free(st);
}
