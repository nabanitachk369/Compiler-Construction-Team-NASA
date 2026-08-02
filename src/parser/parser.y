%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// AST node structures will go here
typedef struct ASTNode ASTNode;
typedef struct NodeList NodeList;

// Forward declarations
void yyerror(const char* s);
extern int yylex();
extern int yylineno;

// AST construction functions (implement later)
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

ASTNode* ast_root = NULL;

%}

%code requires {
#include "src/ast/ast.h"
}

%union {
    int integer;
    float float_val;
    char* string;
    ASTNode* ast_node;
    NodeList* node_list;
}

/* Tokens */
%token KW_INT KW_FLOAT KW_BOOL
%token KW_IF KW_ELSE KW_WHILE KW_PRINT
%token <integer> KW_TRUE KW_FALSE
%token OP_AND OP_OR OP_EQ OP_NE OP_LE OP_GE
%token OP_LT OP_GT OP_PLUS OP_MINUS OP_MULT OP_DIV OP_MOD
%token OP_ASSIGN OP_NOT
%token LBRACE RBRACE LPAREN RPAREN SEMICOLON
%token <string> IDENTIFIER
%token <integer> INTEGER_LITERAL
%token <float_val> FLOAT_LITERAL
%token ERROR_TOKEN

/* Types for non-terminals */
%type <ast_node> program statement declaration assignment if_stmt while_stmt print_stmt block expression
%type <ast_node> logical_or logical_and equality relational additive multiplicative unary primary
%type <node_list> statement_list
%type <string> type

%expect 1

/* Operator precedence (lowest -> highest) */
%right OP_ASSIGN

%left OP_OR
%left OP_AND

%left OP_EQ OP_NE

%left OP_LT OP_GT OP_LE OP_GE

%left OP_PLUS OP_MINUS

%left OP_MULT OP_DIV OP_MOD

%right OP_NOT
%right UMINUS

%%

program
    : statement_list {
        $$ = create_program($1);
        $$->line = yylineno;
        ast_root = $$;
      }
    ;

statement_list
    : /* empty */ { $$ = NULL; }
    | statement_list statement {
        // Append statement to list
        $$ = append_node($1, $2);
    }
    ;

statement
    : declaration { $$ = $1; }
    | assignment { $$ = $1; }
    | if_stmt { $$ = $1; }
    | while_stmt { $$ = $1; }
    | print_stmt { $$ = $1; }
    | block { $$ = $1; }
    ;

declaration
    : type IDENTIFIER SEMICOLON {
        $$ = create_declaration($1, $2);
        $$->line = yylineno;
        free($2); // Free after use
    }
    ;

type
    : KW_INT { $$ = strdup("int"); }
    | KW_FLOAT { $$ = strdup("float"); }
    | KW_BOOL { $$ = strdup("bool"); }
    ;

assignment
    : IDENTIFIER OP_ASSIGN expression SEMICOLON {
        $$ = create_assignment($1, $3);
        $$->line = yylineno;
        free($1);
    }
    ;

if_stmt
    : KW_IF LPAREN expression RPAREN statement {
        $$ = create_if($3, $5, NULL);
        $$->line = yylineno;
    }
    | KW_IF LPAREN expression RPAREN statement KW_ELSE statement {
        $$ = create_if($3, $5, $7);
        $$->line = yylineno;
    }
    ;

while_stmt
    : KW_WHILE LPAREN expression RPAREN statement {
        $$ = create_while($3, $5);
        $$->line = yylineno;
    }
    ;

print_stmt
    : KW_PRINT expression SEMICOLON {
        $$ = create_print($2);
        $$->line = yylineno;
    }
    ;

block
    : LBRACE statement_list RBRACE {
        $$ = create_block($2);
        $$->line = yylineno;
    }
    ;

expression
    : logical_or { $$ = $1; }
    ;

logical_or
    : logical_and { $$ = $1; }
    | logical_or OP_OR logical_and {
        $$ = create_binary_op("||", $1, $3);
        $$->line = yylineno;
    }
    ;

logical_and
    : equality { $$ = $1; }
    | logical_and OP_AND equality {
        $$ = create_binary_op("&&", $1, $3);
        $$->line = yylineno;
    }
    ;

equality
    : relational { $$ = $1; }
    | equality OP_EQ relational {
        $$ = create_binary_op("==", $1, $3);
        $$->line = yylineno;
    }
    | equality OP_NE relational {
        $$ = create_binary_op("!=", $1, $3);
        $$->line = yylineno;
    }
    ;

relational
    : additive { $$ = $1; }
    | relational OP_LT additive {
        $$ = create_binary_op("<", $1, $3);
        $$->line = yylineno;
    }
    | relational OP_GT additive {
        $$ = create_binary_op(">", $1, $3);
        $$->line = yylineno;
    }
    | relational OP_LE additive {
        $$ = create_binary_op("<=", $1, $3);
        $$->line = yylineno;
    }
    | relational OP_GE additive {
        $$ = create_binary_op(">=", $1, $3);
        $$->line = yylineno;
    }
    ;

additive
    : multiplicative { $$ = $1; }
    | additive OP_PLUS multiplicative {
        $$ = create_binary_op("+", $1, $3);
        $$->line = yylineno;
    }
    | additive OP_MINUS multiplicative {
        $$ = create_binary_op("-", $1, $3);
        $$->line = yylineno;
    }
    ;

multiplicative
    : unary { $$ = $1; }
    | multiplicative OP_MULT unary {
        $$ = create_binary_op("*", $1, $3);
        $$->line = yylineno;
    }
    | multiplicative OP_DIV unary {
        $$ = create_binary_op("/", $1, $3);
        $$->line = yylineno;
    }
    | multiplicative OP_MOD unary {
        $$ = create_binary_op("%", $1, $3);
        $$->line = yylineno;
    }
    ;

unary
    : primary { $$ = $1; }
    | OP_MINUS unary %prec UMINUS {
        $$ = create_unary_op("-", $2);
        $$->line = yylineno;
    }
    | OP_NOT unary {
        $$ = create_unary_op("!", $2);
        $$->line = yylineno;
    }
    ;

primary
    : IDENTIFIER {
        $$ = create_variable($1);
        $$->line = yylineno;
        free($1);
    }
    | INTEGER_LITERAL {
        $$ = create_integer_literal($1);
        $$->line = yylineno;
    }
    | FLOAT_LITERAL {
        $$ = create_float_literal($1);
        $$->line = yylineno;
    }
    | KW_TRUE { $$ = create_bool_literal(1); $$->line = yylineno; }
    | KW_FALSE { $$ = create_bool_literal(0); $$->line = yylineno; }
    | LPAREN expression RPAREN { $$ = $2; $$->line = yylineno; }
    ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", yylineno, s);
}
