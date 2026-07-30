CC = gcc
CFLAGS = -Wall -g -I./src -I./src/ast -I./src/symbol_table -I./src/semantic -I./src/codegen
LEX = flex
YACC = bison
YFLAGS = -d -v

SRC_DIR = src
LEX_DIR = $(SRC_DIR)/lexer
PARSER_DIR = $(SRC_DIR)/parser
AST_DIR = $(SRC_DIR)/ast
SYM_DIR = $(SRC_DIR)/symbol_table
SEM_DIR = $(SRC_DIR)/semantic
CODEGEN_DIR = $(SRC_DIR)/codegen

LEX_SRC = $(LEX_DIR)/lexer.l
PARSER_SRC = $(PARSER_DIR)/parser.y
AST_SRC = $(AST_DIR)/ast.c
SYM_SRC = $(SYM_DIR)/symbol_table.c
SEM_SRC = $(SEM_DIR)/semantic.c
CODEGEN_SRC = $(CODEGEN_DIR)/tac.c
MAIN_SRC = $(SRC_DIR)/main.c

LEX_OUT = lex.yy.c
PARSER_OUT = parser.tab.c
PARSER_HEADER = parser.tab.h

OBJS = $(LEX_OUT:.c=.o) $(PARSER_OUT:.c=.o) ast.o symbol_table.o semantic.o tac.o main.o

TARGET = compiler

.PHONY: all clean distclean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

lex.yy.c: $(LEX_SRC)
	$(LEX) -o $@ $^

parser.tab.c parser.tab.h: $(PARSER_SRC)
	$(YACC) $(YFLAGS) -o parser.tab.c $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

ast.o: $(AST_DIR)/ast.c $(AST_DIR)/ast.h
	$(CC) $(CFLAGS) -c $< -o $@

symbol_table.o: $(SYM_DIR)/symbol_table.c $(SYM_DIR)/symbol_table.h
	$(CC) $(CFLAGS) -c $< -o $@

semantic.o: $(SEM_DIR)/semantic.c $(SEM_DIR)/semantic.h
	$(CC) $(CFLAGS) -c $< -o $@

tac.o: $(CODEGEN_DIR)/tac.c $(CODEGEN_DIR)/tac.h
	$(CC) $(CFLAGS) -c $< -o $@

main.o: $(MAIN_SRC) $(PARSER_HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) lex.yy.c parser.tab.c parser.tab.h parser.output

distclean: clean
	rm -f $(TARGET)
