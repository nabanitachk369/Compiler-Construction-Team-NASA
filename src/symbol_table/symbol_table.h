#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

/* ===== DATA TYPES ===== */
typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_BOOL,
    TYPE_UNKNOWN
} DataType;

/* ===== SYMBOL ===== */
typedef struct Symbol {
    char* name;
    DataType type;
    int line_declared;
    struct Symbol* next;
} Symbol;

/* ===== SCOPE ===== */
typedef struct Scope {
    Symbol* symbols;
    struct Scope* parent;
} Scope;

/* ===== SYMBOL TABLE ===== */
typedef struct SymbolTable {
    Scope* current_scope;
    int error_count;
} SymbolTable;

/* ===== FUNCTION PROTOTYPES ===== */
SymbolTable* create_symbol_table();
void enter_scope(SymbolTable* st);
void exit_scope(SymbolTable* st);
int add_symbol(SymbolTable* st, const char* name, DataType type, int line);
Symbol* lookup_symbol(SymbolTable* st, const char* name);
Symbol* lookup_in_current_scope(SymbolTable* st, const char* name);
int is_symbol_in_current_scope(SymbolTable* st, const char* name);
void print_symbol_table(SymbolTable* st);
void free_symbol_table(SymbolTable* st);

#endif

