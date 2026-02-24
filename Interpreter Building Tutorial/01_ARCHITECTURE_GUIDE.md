# LEXOR Interpreter in C++ - Complete Architecture Guide

## Project Overview
Build a complete LEXOR interpreter in C++ with three main phases: Lexer → Parser → Interpreter

## Architecture Overview

```
Source Code (.lexor)
       ↓
   [LEXER]
    Tokenization
       ↓
   [PARSER]
    AST Construction
       ↓
[INTERPRETER]
    Execution & Output
```

## Phase 1: Lexer (Tokenization)
Converts source code into tokens.

### Responsibilities:
- Scan character by character
- Skip comments (`%%`)
- Recognize keywords, operators, literals, identifiers
- Handle escape sequences `[]`
- Report line numbers for errors

### Token Types:
```
KEYWORD:     START, END, SCRIPT, DECLARE, INT, CHAR, BOOL, FLOAT, PRINT, SCAN, IF, ELSE, FOR, REPEAT, WHEN, AND, OR, NOT
NUMBER:      123, 456.78
STRING:      "hello world"
CHAR_LIT:    'a', 'c'
IDENTIFIER:  x, y, a_1
OPERATOR:    +, -, *, /, %, <, >, <=, >=, ==, <>, &, $, =
PUNCTUATION: (, ), {, }, [, ], :, ,
EOF:         End of file
```

## Phase 2: Parser (Syntax Analysis)
Builds an Abstract Syntax Tree (AST).

### Grammar Rules:
```
Program → SCRIPT AREA START SCRIPT Declarations Statements END SCRIPT

Declarations → (DECLARE Type Vars)*

Type → INT | CHAR | BOOL | FLOAT

Vars → ID ('=' Value)? (',' ID ('=' Value)?)*

Statements → (Statement)*

Statement → Assignment
          | PrintStmt
          | ScanStmt
          | IfStmt
          | ForStmt
          | RepeatStmt

Assignment → ID '=' Expr

PrintStmt → PRINT ':' PrintExpr ('&' PrintExpr)*

ScanStmt → SCAN ':' ID (',' ID)*

IfStmt → IF '(' BoolExpr ')' START IF Statements END IF (ELSE IF ... | ELSE ... )?

ForStmt → START FOR Statements END FOR

RepeatStmt → REPEAT WHEN '(' BoolExpr ')' START REPEAT Statements END REPEAT

Expr → BoolExpr

BoolExpr → LogicalExpr (('AND' | 'OR') LogicalExpr)*

LogicalExpr → CompExpr | 'NOT' CompExpr

CompExpr → ArithExpr (('>=' | '<=' | '>' | '<' | '==' | '<>') ArithExpr)*

ArithExpr → Term (('+' | '-') Term)*

Term → Factor (('*' | '/' | '%') Factor)*

Factor → ('+'|'-')? Primary

Primary → NUMBER | STRING | CHAR | ID | '(' Expr ')'
```

## Phase 3: Interpreter (Execution)
Traverses AST and executes the program.

### Responsibilities:
- Maintain symbol table (variables and values)
- Evaluate expressions recursively
- Execute statements sequentially
- Handle control flow (if, for, while)
- Manage I/O operations
- Type checking and conversions

### Symbol Table Structure:
```cpp
struct Variable {
    string name;
    string type;  // "INT", "CHAR", "BOOL", "FLOAT"
    Value value;  // Stores actual value
};

map<string, Variable> symbolTable;
```

## C++ Project Structure

```
Interpreter/
├── Project Files
│   ├── main.cpp
│   ├── token.h
│   ├── ast.h
│   ├── value.h
│   ├── lexer.h / lexer.cpp
│   ├── parser.h / parser.cpp
│   └── interpreter.h / interpreter.cpp
│
└── Example Programs
    ├── sample.lexor
    ├── arithmetic.lexor
    ├── logical.lexor
    └── control_flow.lexor
```

## Special Cases to Handle

1. **String Concatenation**: `&` operator joins values as strings
   - `PRINT: x & "=" & y` outputs: `5=10`

2. **Escape Codes**: `[]` represents special characters
   - `[#]` → `#` character
   - `[$]` → `$` character
   - `[[]` → `[` character
   - `[]]` → `]` character

3. **Line Breaks**: `$` represents newline
   - Values before `$` print on one line
   - Values after print on next line

4. **Type Coercion**: When concatenating, convert all to strings
   - INT `5` → `"5"`
   - BOOL `TRUE` → `"TRUE"`
   - CHAR `'c'` → `"c"`

5. **Boolean Representations**:
   - Input: `"TRUE"` or `"FALSE"` (string literals)
   - Output: `TRUE` or `FALSE` (no quotes)

## Development Strategy

1. **Start with Token types** - Define all possible tokens
2. **Build Lexer** - Convert source to tokens
3. **Build Parser** - Convert tokens to AST
4. **Build Interpreter** - Execute AST
5. **Test incrementally** - Test each phase with simple examples
6. **Add features gradually** - Basic expressions → statements → control flow

## Key Implementation Notes

- Use **recursive descent parsing** for simplicity
- Use **tree-walking interpreter** for execution
- Use **map/unordered_map** for symbol tables
- Handle **comments** before any other processing
- Validate **syntax errors** with clear error messages
- Support **multiple variable declarations** on one line
- Support **chained assignments**: `x=y=z=5`

Next: Start with the Lexer Implementation (02_LEXER_TUTORIAL.md)
