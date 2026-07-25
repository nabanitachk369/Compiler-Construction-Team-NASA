%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

extern int yylineno;
extern char* yytext;

// Function to handle errors
void yyerror(const char* s);
%}

%option noyywrap
%option yylineno
%option debug

%%

/* ===== KEYWORDS ===== */
"int"           { return KW_INT; }
"float"         { return KW_FLOAT; }
"bool"          { return KW_BOOL; }
"if"            { return KW_IF; }
"else"          { return KW_ELSE; }
"while"         { return KW_WHILE; }
"print"         { return KW_PRINT; }
"true"          { return KW_TRUE; }
"false"         { return KW_FALSE; }

/* ===== OPERATORS ===== */
"&&"            { return OP_AND; }
"||"            { return OP_OR; }
"=="            { return OP_EQ; }
"!="            { return OP_NE; }
"<="            { return OP_LE; }
">="            { return OP_GE; }
"<"             { return OP_LT; }
">"             { return OP_GT; }
"+"             { return OP_PLUS; }
"-"             { return OP_MINUS; }
"*"             { return OP_MULT; }
"/"             { return OP_DIV; }
"%"             { return OP_MOD; }
"="             { return OP_ASSIGN; }

/* ===== DELIMITERS ===== */
"{"             { return LBRACE; }
"}"             { return RBRACE; }
"("             { return LPAREN; }
")"             { return RPAREN; }
";"             { return SEMICOLON; }

/* ===== IDENTIFIERS ===== */
[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.string = strdup(yytext);
    return IDENTIFIER;
}

/* ===== INTEGER LITERALS ===== */
[0-9]+ {
    yylval.integer = atoi(yytext);
    return INTEGER_LITERAL;
}

/* ===== FLOAT LITERALS ===== */
[0-9]+\.[0-9]+ {
    yylval.float_val = atof(yytext);
    return FLOAT_LITERAL;
}

/* ===== SINGLE-LINE COMMENTS ===== */
"//".*          { /* ignore */ }

/* ===== MULTI-LINE COMMENTS ===== */
"/*"([^*]|\*+[^*/])*\*+"/" { /* ignore */ }

/* ===== WHITESPACE ===== */
[ \t\n\r]       { /* ignore */ }

/* ===== ERROR HANDLING ===== */
. {
    fprintf(stderr, "Lexical error at line %d: unrecognized character '%s'\n", 
            yylineno, yytext);
    return ERROR_TOKEN;
}

%%
