# SimpleLang Compiler

A simple compiler for a C-like language that compiles to assembly code. This project demonstrates basic lexing, parsing, abstract syntax tree (AST) construction, and assembly code generation for 8 bit CPU.

## Features

- Variable declaration and assignment
- Integer literals
- Basic arithmetic operations (+, -)
- Conditional statements (if-else)
- Block statements with braces
- Generation of pseudo-assembly code

## Compiler Architecture

The compiler consists of three main components:

1. Lexical Analysis (Lexer) - Tokenizes the source code.
2. Parsing (Parser) - Constructs an Abstract Syntax Tree (AST).
3. Code Generation - Converts AST into assembly code.

## Implementation Details

### 1. Lexical Analysis

- The lexer scans the input source code and converts it into tokens.
- It handles keywords (int, if, else), identifiers, numbers, operators (+, -, =), and delimiters (;, {, }).

### 2. Parsing (Parser)

- The parser receives tokens from the lexer and generates an Abstract Syntax Tree (AST).
- The AST is represented using multiple node types: NumberNode, VariableNode, BinaryOpNode, ConditionalNode, and BlockNode.

### 3. Code Generation

- The AST is traversed, and corresponding assembly instructions are written to an output file (output.asm).
- Each AST node generates specific assembly instructions.
- Implemented using virtual functions in ASTNode and derived classes.

## Language Syntax

### Variable Declaration
```c
int x;
int y = 10;
```

### Conditional Statements
```c
if (x == 5) {
    y = 10;
} else {
    y = 20;
}
```

### Assignment
```c
x = 5;
y = x + 3;
```

### Arithmetic Expressions
```c
int result = (x + y) - 15;
```




## Build the Project

To build this project, follow this steps:

```
git clone git@github.com:shubham-shinde-442/compiler.git
cd compiler
mkdir build
cmake -S . -B build
cmake --build build
```
And done.


