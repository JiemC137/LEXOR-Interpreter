# LEXOR Interpreter Development - Quick Reference

## Lexer Quick Reference

### Token Types
```
Keywords:    SCRIPT, AREA, START, END, DECLARE, INT, CHAR, BOOL, FLOAT, PRINT, SCAN, IF, ELSE, FOR, REPEAT, WHEN, AND, OR, NOT
Numbers:     123, 45.67
Strings:     "hello"
Characters:  'a'
Identifiers: x, y, var_name
Operators:   +, -, *, /, %, <, >, <=, >=, ==, <>, &, $, =, (, ), [, ], :, ,
```

### Lexer Class Methods
```cpp
Lexer(string source);              // Constructor
Token nextToken();                 // Get one token
vector<Token> tokenize();          // Get all tokens
```

### Common Lexer Errors
- Unterminated string: `"hello` 
- Invalid escape: Unknown character after `[`
- Unexpected character: `@`, `#` (outside escape)

---

## Parser Quick Reference

### Grammar Summary
```
Program = SCRIPT AREA START SCRIPT Declarations Statements END SCRIPT

Declarations = (DECLARE Type Vars)*

Type = INT | CHAR | BOOL | FLOAT

Vars = ID ('=' Value)? (',' ID ('=' Value)?)*

Statement = Declaration | Assignment | PrintStmt | ScanStmt | IfStmt | ForStmt | RepeatStmt

Expression = LogicalAnd ((OR | AND) LogicalAnd)*
LogicalAnd = Comparison ((AND) Comparison)*
Comparison = Arithmetic ((==|<>|<|>|<=|>=) Arithmetic)*
Arithmetic = Term ((+|-) Term)*
Term = Factor ((*|/|%) Factor)*
Factor = Unary
Unary = (+|-|NOT) Primary | Primary
Primary = NUMBER | STRING | CHAR | ID | ( Expression )
```

### AST Node Classes
- `Program` - Root node
- `Declaration` - Variable declaration
- `Assignment` - Variable assignment
- `PrintStatement` - Output
- `ScanStatement` - Input
- `IfStatement` - Conditional
- `ForStatement` - Loop (START FOR)
- `RepeatStatement` - Loop (REPEAT WHEN)

### Parser Class Methods
```cpp
Parser(vector<Token> tokens);      // Constructor
shared_ptr<Program> parse();       // Parse program
```

---

## Interpreter Quick Reference

### Value Types
```cpp
Value::TYPE_INT      // 32-bit integer
Value::TYPE_FLOAT    // Floating point
Value::TYPE_CHAR     // Single character
Value::TYPE_BOOL     // true/false
Value::TYPE_STRING   // Text
```

### Symbol Table Operations
```cpp
symbolTable[varName] = {type, value};           // Declare/set
Value val = symbolTable[varName].second;        // Get value
string type = symbolTable[varName].first;       // Get type
```

### Value Conversions
```cpp
value.toString();    // Convert to string
value.toNumber();    // Convert to number
value.toBool();      // Convert to boolean
```

### Interpreter Class Methods
```cpp
Interpreter();                                   // Constructor
void execute(shared_ptr<Program> program);      // Execute program
string getOutput() const;                       // Get output
```

---

## Common Operations

### Print with Concatenation
```lexor
PRINT: a & " + " & b & " = " & (a + b)
```
Outputs: `5 + 10 = 15`

### New Line in Output
```lexor
PRINT: "Line 1" & $ & "Line 2"
```
Outputs:
```
Line 1
Line 2
```

### Escape Characters
```lexor
PRINT: "Hash: " & [#]         %% Outputs: Hash: #
PRINT: "Dollar: " & [$]       %% Outputs: Dollar: $
PRINT: "[Bracket" & [[]       %% Outputs: [Bracket[
```

### Chained Assignment
```lexor
x = y = z = 5     %% All three get value 5
```

### Boolean Expressions
```lexor
DECLARE BOOL result
result = (a > b AND c < d)
result = (x == 5 OR y <> 3)
result = NOT (flag)
```

---

## Implementation Checklist

### Phase 1: Lexer
- [ ] Token enumeration
- [ ] Lexer class structure
- [ ] Character scanning
- [ ] Comment skipping
- [ ] Number parsing
- [ ] String parsing
- [ ] Identifier/keyword recognition
- [ ] Two-character operators

### Phase 2: Parser
- [ ] AST node classes
- [ ] Token consumption
- [ ] Expression parsing (precedence)
- [ ] Statement parsing
- [ ] Error handling
- [ ] Program parsing

### Phase 3: Interpreter
- [ ] Value class
- [ ] Symbol table
- [ ] Expression evaluation
- [ ] Statement execution
- [ ] I/O operations
- [ ] Control flow

### Testing
- [ ] Simple expressions
- [ ] Variable declarations
- [ ] Arithmetic operations
- [ ] String concatenation
- [ ] Conditionals
- [ ] Loops
- [ ] Complex programs

---

## Operator Precedence (Lowest to Highest)

```
1. OR
2. AND
3. NOT
4. ==, <>, <, >, <=, >=
5. +, -
6. *, /, %
7. Unary +, -, NOT
8. Primary (literals, variables, parentheses)
```

---

## String Concatenation Rules

- `&` joins values as strings
- INT `5` becomes `"5"`
- BOOL `TRUE` becomes `"TRUE"`
- CHAR `'c'` becomes `"c"`
- Result is always STRING type

---

## Type Coercion Rules

| Source | to INT | to FLOAT | to CHAR | to BOOL |
|--------|--------|----------|---------|---------|
| INT | Same | Float conversion | Char(value) | value != 0 |
| FLOAT | (int) truncate | Same | Char(int) | value != 0 |
| CHAR | ASCII value | ASCII float | Same | value != '\0' |
| BOOL | 1 or 0 | 1.0 or 0.0 | '1' or '0' | Same |
| STRING | stod(str) | stod(str) | str[0] | str != "" |

---

## Debug Output Format

System should print line/column for errors:

```
Parse Error at line 5, column 12: Expected END
Undefined variable 'x' at line 8
Type mismatch: INT expected, BOOL found at line 10
```

---

## File Format

LEXOR source files can use any extension, commonly:
- `.lexor`
- `.lex`
- `.lxr`

---

## Command Line Usage

```bash
# File input
interpreter program.lexor

# Standard input
interpreter < program.lexor
cat program.lexor | interpreter

# With error output
interpreter program.lexor 2> errors.txt

# Piping
interpreter program.lexor | tee output.txt
```

---

## Performance Tips

1. **Parsing**: Use recursive descent (already efficient)
2. **Interpretation**: 
   - Cache symbol table lookups
   - Avoid unnecessary type conversions
   - Use references for large objects
3. **Memory**: 
   - Use `shared_ptr` carefully
   - Clear symbol table after programs
4. **Expression Evaluation**: 
   - Short-circuit AND/OR
   - Avoid deep recursion in loops

---

## Common Bugs and Fixes

| Bug | Fix |
|-----|-----|
| Variable undefined | Check declaration matches case |
| Wrong output | Check & for concatenation, $ for newline |
| Infinite loop | Add loop condition increment, debug output |
| Crashes on input | Check variable exists, handle type mismatch |
| Operator precedence wrong | Verify expression tree structure |
| String not printing | Check escape sequences `[]` |

---

## Code Structure Template

```cpp
// Token stream
Lexer lexer(sourceCode);
vector<Token> tokens = lexer.tokenize();

// Build AST
Parser parser(tokens);
shared_ptr<Program> ast = parser.parse();

// Execute
Interpreter interpreter;
interpreter.execute(ast);

// Get output
cout << interpreter.getOutput();
```

---

## Key Classes Relationships

```
Lexer (tokens)
  ↓
Parser (AST)
  ├─→ Token
  ├─→ ASTNode
  │    ├─→ Expression
  │    │    ├─→ BinaryOp
  │    │    ├─→ UnaryOp
  │    │    ├─→ Literal
  │    │    └─→ Identifier
  │    └─→ Statement
  │         ├─→ Declaration
  │         ├─→ Assignment
  │         ├─→ PrintStatement
  │         ├─→ IfStatement
  │         └─→ Loop
  ↓
Interpreter (execution)
  ├─→ SymbolTable (variables)
  ├─→ Value (runtime values)
  └─→ Output (results)
```

---

## Testing One-Liners

```cpp
// Quick test in main.cpp
string test = "SCRIPT AREA START SCRIPT DECLARE INT x=5 PRINT: x END SCRIPT";
Lexer l(test);
Parser p(l.tokenize());
Interpreter i;
i.execute(p.parse());
cout << i.getOutput();  // Output: 5
```

---

## Resources in This Guide

1. **01_ARCHITECTURE_GUIDE.md** - System design overview
2. **02_LEXER_TUTORIAL.md** - Token scanning implementation
3. **03_PARSER_TUTORIAL.md** - AST construction
4. **04_INTERPRETER_TUTORIAL.md** - Program execution
5. **05_COMPLETE_EXAMPLES.md** - Sample programs
6. **06_BUILD_GUIDE.md** - Compilation setup
7. **07_ENHANCEMENT_GUIDE.md** - Advanced features
8. **08_QUICK_REFERENCE.md** - This file

---

## Quick Start Commands

```bash
# Windows
cl /std:c++17 /I.\include src\*.cpp /Fe:interpreter.exe
interpreter.exe examples\hello.lexor

# Linux/Mac
g++ -std=c++17 -I./include src/*.cpp -o interpreter
./interpreter examples/hello.lexor

# CMake (All platforms)
mkdir build && cd build
cmake ..
cmake --build .
./interpreter ../examples/hello.lexor
```

---

## Success Criteria

Your interpreter should:
- ✓ Parse all LEXOR syntax without errors
- ✓ Execute all control flow structures
- ✓ Handle all data types (INT, FLOAT, CHAR, BOOL)
- ✓ Support variable declarations and assignments
- ✓ Implement all operators (arithmetic, logical, comparison)
- ✓ Handle I/O (PRINT, SCAN)
- ✓ Process comments (skip %%)
- ✓ Handle escape sequences ([#], [$], etc.)
- ✓ Report meaningful errors
- ✓ Run all example programs correctly

---

Next: See 09_TROUBLESHOOTING_GUIDE.md for problem solving
