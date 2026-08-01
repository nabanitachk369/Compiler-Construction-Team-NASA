# Compiler-Construction-Team-NASA

**Compiler Construction Lab Project**
**Department of Computer Science & Engineering**
**Metropolitan University, Bangladesh**

A mini compiler front-end developed using **Flex** and **Bison** as part of the Compiler Construction Lab (CSE 416). The compiler accepts a simplified C-like language and processes it through lexical analysis, syntax analysis, semantic analysis, Abstract Syntax Tree (AST) generation, symbol table management, and Three-Address Code (TAC) generation.

---

# Team — NASA

| Name                 | ID          | GitHub                    |
| -------------------- | ----------- | ------------------------- |
| Nabanita Chakraborty | 231-115-265 | @nabanitachk369           |
| Arpita Das Trisha    | 231-115-262 | @arpitadastrisha2003-cmyk |

**Repository:** https://github.com/nabanitachk369/Compiler-Construction-Team-NASA

---

# What This Compiler Does

Given a source program written in our simplified language, the compiler performs the following phases:

### 🔹 Lexical Analysis

Scans the source program and converts it into a stream of tokens such as identifiers, keywords, literals, operators, and delimiters.

### 🔹 Syntax Analysis

Parses the token stream according to the language grammar and constructs an Abstract Syntax Tree (AST).

### 🔹 Semantic Analysis

Validates the program by checking:

* Variable declarations
* Redeclarations
* Scope rules
* Type compatibility
* Assignment correctness

using a symbol table.

### 🔹 Intermediate Code Generation

Traverses the AST and generates **Three-Address Code (TAC)** using temporary variables.

If any lexical, syntax, or semantic error is encountered, compilation stops immediately and reports the corresponding error.

---

# The Language

```c
int x;
int y;
bool flag;

x = 10;
y = 0;
flag = true;

while (x > 0) {
    y = y + x;
    x = x - 1;
}

if (flag == true) {
    print y;
}
else {
    print x;
}
```

---

# Supported Features

### Data Types

* int
* float
* bool

### Statements

* Variable declarations
* Assignment statements
* print statement
* if
* if-else
* while

### Operators

**Arithmetic**

* +
* -
* *
* /
* %

**Relational**

* <
* >
* <=
* > =
* ==
* !=

**Logical**

* &&
* ||
* !

### Scope

* Nested blocks
* Block-level scope handling
* Symbol table with nested scopes

---

# Not Supported

The following language features are intentionally outside the scope of this project:

* Functions
* Arrays
* Strings
* For loops
* Switch statements
* Type casting
* Structures
* Pointers

---

# Requirements

* Ubuntu / Linux
* GCC
* Flex(Lex)
* Bison(Yacc)
* Make

Install the required packages:

```bash
sudo apt update
sudo apt install build-essential flex bison
```

---

# How to Build

Clone the repository:

```bash
git clone https://github.com/nabanitachk369/Compiler-Construction-Team-NASA.git
cd Compiler-Construction-Team-NASA
```

Compile the project:

```bash
make
```

This generates the lexer and parser from **src/lexer/lexer.l** and **src/parser/parser.y**, compiles all compiler modules, and creates the executable.

To remove generated files:

```bash
make clean
```

---

# Running the Compiler

Compile a source program:

```bash
./compiler tests/valid/while.md
```

Example:

```bash
./compiler tests/valid/if_else.md
```

The compiler outputs:

* Token Stream
* Abstract Syntax Tree (AST)
* Symbol Table
* Semantic Analysis Result
* Three-Address Code (TAC)

If an error occurs during lexical, syntax, or semantic analysis, the compiler reports the error and terminates before proceeding to the next phase.

---

# Test Cases

The project contains both successful and erroneous test programs.

## Valid Tests

* Variable declarations
* Assignment statements
* if statements
* while loops
* Nested blocks

## Invalid Tests

* Undeclared variables
* Variable redeclarations
* Type mismatch errors

---

# Project Structure

```text
.
├── Makefile
├── README.md
├── src/
│   ├── lexer/
│   │   └── lexer.l
│   ├── parser/
│   │   └── parser.y
│   ├── ast/
│   │   ├── ast.h
│   │   └── ast.c
│   ├── symbol_table/
│   │   ├── symbol_table.h
│   │   └── symbol_table.c
│   ├── semantic/
│   │   ├── semantic.h
│   │   └── semantic.c
│   ├── codegen/
│   │   ├── tac.h
│   │   └── tac.c
│   └── main.c
├── tests/
│   ├── valid/couple of valid tests
|   |
│   └── invalid/couple of invalid tests
│       
└── docs/
    └── Compiler Construction Lab Project Manual.pdf

```

---

# Design Notes

* The compiler is implemented using **Flex** and **Bison**.
* The parser builds an Abstract Syntax Tree before semantic analysis.
* Every block introduces a new scope managed through the symbol table.
* Semantic analysis verifies declarations, scopes, and type correctness before code generation.
* Three-Address Code (TAC) generation uses temporary variables (`t1`, `t2`, ...) for intermediate expressions.

---

# Technologies Used

* C Programming
* Flex (Lex)
* Bison (Yacc)
* GCC
* Make
* Ubuntu Linux

---

# Documentation

Additional project documentation is available in:

```
docs/Compiler Construction Lab Project Manual.pdf
```

---

# Team Contributions

The project was developed collaboratively by **Team NASA**. Development responsibilities were divided across compiler phases, including lexical analysis, parsing, semantic analysis, symbol table implementation, AST construction, intermediate code generation, testing, and documentation. Git commit history reflects the individual contributions of all team members.

---

# AI Usage

AI tools were used during development to assist with debugging, documentation, code review, and implementation guidance. Every member understands the architecture, implementation details, and functionality of the compiler and can explain each module independently.

---

