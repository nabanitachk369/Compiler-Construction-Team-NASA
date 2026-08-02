
%{
#include "parser.tab.h"
#include <stdlib.h>
#include <string.h>

int lexical_error_count = 0;


#define TOK(name) do { \
    printf("  line %-4d  %-12s  %s\n", yylineno, #name, yytext); \
    return name; \
} while (0)
%}

%option yylineno
%option noyywrap
%option noinput
%option nounput

%x COMMENT

DIGIT    [0-9]
LETTER   [a-zA-Z_]
ID       {LETTER}({LETTER}|{DIGIT})*
INTLIT   {DIGIT}+
FLOATLIT {DIGIT}+"."{DIGIT}+

%%


"//".*                  { /* single-line comment: discard */ }

"/*"                    { BEGIN(COMMENT); }
<COMMENT>"*/"           { BEGIN(INITIAL); }
<COMMENT>\n             { /* yylineno auto-increments (yylineno option) */ }
<COMMENT>.              { /* discard comment body char-by-char */ }


[ \t\r]+                { /* discard spaces/tabs */ }
\n                       { /* discard newline */ }



"int"                    { TOK(KW_INT); }
"float"                  { TOK(KW_FLOAT); }
"bool"                   { TOK(KW_BOOL); }
"if"                     { TOK(KW_IF); }
"else"                   { TOK(KW_ELSE); }
"while"                  { TOK(KW_WHILE); }
"print"                  { TOK(KW_PRINT); }
"true"                   { yylval.integer = 1; TOK(KW_TRUE); }
"false"                  { yylval.integer = 0; TOK(KW_FALSE); }


{FLOATLIT}               { yylval.float_val = (float)atof(yytext); TOK(FLOAT_LITERAL); }
{INTLIT}                 { yylval.integer = atoi(yytext); TOK(INTEGER_LITERAL); }

    /* -------------------- Identifiers -------------------- */
{ID}                     { yylval.string = strdup(yytext); TOK(IDENTIFIER); }

    /* -------------------- Arithmetic operators -------------------- */
"+"                      { TOK(OP_PLUS); }
"-"                      { TOK(OP_MINUS); }
"*"                      { TOK(OP_MULT); }
"/"                      { TOK(OP_DIV); }
"%"                      { TOK(OP_MOD); }

    /* ---------- Relational operators (2-char before 1-char) ---------- */
"<="                     { TOK(OP_LE); }
">="                     { TOK(OP_GE); }
"=="                     { TOK(OP_EQ); }
"!="                     { TOK(OP_NE); }
"<"                      { TOK(OP_LT); }
">"                      { TOK(OP_GT); }

    /* -------------------- Logical operators -------------------- */
"&&"                     { TOK(OP_AND); }
"||"                     { TOK(OP_OR); }
"!"                      { TOK(OP_NOT); }

    /* -------------------- Assignment -------------------- */
"="                      { TOK(OP_ASSIGN); }

    /* -------------------- Delimiters -------------------- */
"{"                      { TOK(LBRACE); }
"}"                      { TOK(RBRACE); }
"("                      { TOK(LPAREN); }
")"                      { TOK(RPAREN); }
";"                      { TOK(SEMICOLON); }

    /* -------------------- Invalid tokens -------------------- */
.                        {
                            fprintf(stderr,
                                "Lexical Error at line %d: unexpected character '%s'\n",
                                yylineno, yytext);
                            lexical_error_count++;
                         }

%%








