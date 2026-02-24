# LEXOR Interpreter - Troubleshooting Guide

## Compilation Errors

### Error: "fatal error: ast.h: No such file or directory"

**Cause**: Include path not set correctly

**Solution**:
```bash
# Make sure you're in the project root directory
# Compile with -I flag pointing to include directory
g++ -std=c++17 -I./include src/main.cpp src/*.cpp -o interpreter

# Or check your project settings
# Visual Studio: Project → Properties → VC++ Directories → Include Directories
# Add: $(ProjectDir)include
```

---

### Error: "undefined reference to `Lexer::nextToken()'"

**Cause**: Linking error - .cpp file not compiled

**Solution**:
```bash
# Make sure ALL .cpp files are included
g++ -std=c++17 -I./include src/main.cpp src/lexer.cpp src/parser.cpp src/interpreter.cpp -o interpreter

# Or check CMakeLists.txt includes all source files
set(SOURCES
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/interpreter.cpp
)
```

---

### Error: "'class Token' has no member named 'line'"

**Cause**: Token.h definition doesn't match usage

**Solution**:
Ensure token.h has the line field:
```cpp
struct Token {
    TokenType type;
    string value;
    int line;      // ← Must be included
    int column;
};
```

---

### Error: "stray '\342' in program"

**Cause**: File encoding issue (non-ASCII characters)

**Solution**:
1. Save all .h and .cpp files as **UTF-8 without BOM**
2. In VS Code: Bottom right → Select Encoding → UTF-8
3. In Visual Studio: File → Advanced Save Options → UTF-8

---

### Error: "error C2468: cannot initialize a member of const type"

**Cause**: Trying to modify const member in Value constructor

**Solution**:
```cpp
// Wrong - trying to modify after construction
struct Value {
    const Type type;  // Can't modify after construction
};

// Right - use member initializer list
struct Value {
    Type type;  // Not const, or
    Value(Type t) : type(t) {}  // Use initializer list
};
```

---

## Runtime Errors

### Error: "Undefined variable: x"

**Cause**: Variable used before declaration or typo

**Solution**:
1. Check variable is declared before use
2. Check variable name matches (case-sensitive)
3. Add debug output:
```cpp
// In interpreter
cout << "[DEBUG] Accessing variable: " << id->name << endl;
cout << "[DEBUG] Symbol table size: " << symbolTable.size() << endl;
```

---

### Error: "Division by zero"

**Cause**: Attempting `a / 0` or `a % 0`

**Solution**:
Add runtime check:
```cpp
if (op == "/") {
    double rightNum = right.toNumber();
    if (rightNum == 0) {
        throw runtime_error("Division by zero at line " + to_string(line));
    }
    return Value(left.toNumber() / rightNum);
}
```

---

### Error: "Array index out of bounds" (if implementing arrays)

**Cause**: Accessing array element beyond size

**Solution**:
```cpp
Value Vector::getElementAt(int index) {
    if (index < 0 || index >= elements.size()) {
        throw runtime_error("Array index out of bounds: " + to_string(index) + 
                          " for array of size " + to_string(elements.size()));
    }
    return elements[index];
}
```

---

### Error: "Type mismatch: Trying to assign STRING to INT"

**Cause**: Strict type checking rejecting assignment

**Solution**:
Either:
1. Add type coercion:
```cpp
if (declaredType == "INT" && value.type == Value::TYPE_STRING) {
    value = Value((int)stod(value.stringValue));
}
```

2. Or relax checking:
```cpp
// Allow implicit conversion
symbolTable[varName].second = value;  // Auto-convert
```

---

## Lexer Issues

### Problem: Comments not skipped

**Symptoms**: Error tokens for `%%` text

**Debug**:
```cpp
// In lexer.cpp, add debug output
void Lexer::skipComment() {
    if (current == '%' && peek() == '%') {
        cout << "[DEBUG] Skipping comment at line " << line << endl;
        advance(); // skip first %
        advance(); // skip second %
        while (current != '\n' && current != '\0') {
            advance();
        }
    }
}
```

**Fix**:
```cpp
// Ensure skipComment is called in nextToken()
Token Lexer::nextToken() {
    skipWhitespace();
    
    // Skip multiple comments in a row
    while (current == '%' && peek() == '%') {
        skipComment();
        skipWhitespace();
    }
    // ... rest of nextToken
}
```

---

### Problem: String parsing stops early

**Symptoms**: String truncated at first quote

**Code**:
```cpp
Token Lexer::readString() {
    advance(); // skip opening quote
    string value = "";
    
    while (current != '"' && current != '\0') {   // ← Check for end quote
        value += current;
        advance();
    }
    
    if (current == '"') advance(); // skip closing quote
    return Token(TOKEN_STRING, value, line, column);
}
```

---

### Problem: Number parsing doesn't handle decimals

**Symptoms**: `45.67` parsed as two tokens

**Code**:
```cpp
Token Lexer::readNumber() {
    string value = "";
    bool hasDecimal = false;
    
    while (isDigit(current) || (current == '.' && !hasDecimal)) {   // ← Allow one decimal
        if (current == '.') hasDecimal = true;
        value += current;
        advance();
    }
    
    return Token(TOKEN_NUMBER, value, line, column);
}
```

---

### Problem: Keywords not recognized

**Symptoms**: `START` recognized as identifier, not keyword

**Check**:
1. Make sure keyword string matches exactly (case-sensitive)
2. getKeywordType returns TOKEN_IDENTIFIER for unknown keywords ✓
3. Reserved words must be uppercase in LEXOR

```cpp
TokenType Lexer::getKeywordType(string s) {
    // Must match exactly - LEXOR uses UPPERCASE
    if (s == "SCRIPT") return TOKEN_SCRIPT;
    if (s == "START") return TOKEN_START;
    // ... etc
    return TOKEN_IDENTIFIER;  // If not found, it's an identifier
}
```

---

## Parser Issues

### Problem: "Expected END" error

**Symptoms**: Parse error on valid code

**Debug**:
```cpp
// Add debug output in parser
if (check(TOKEN_END)) {
    cout << "[DEBUG] Found END token" << endl;
} else {
    cout << "[DEBUG] Current token: " << peek().value 
         << " (expected END)" << endl;
}
```

**Common Causes**:
1. Missing START SCRIPT after SCRIPT AREA
2. Missing END IF/END FOR/END REPEAT
3. Case sensitivity (END vs end)

---

### Problem: Operator precedence wrong

**Symptoms**: `2 + 3 * 4` gives `20` instead of `14`

**Check operator precedence in parser**:
```
✓ Logical OR (lowest)
✓ Logical AND
✓ Comparison
✓ Addition/Subtraction
✓ Multiplication/Division/Modulo (highest)

// Correct order in parser:
expression()  // OR
  → logicalAnd()  // AND
    → comparison()  // ==, <>, <, >, <=, >=
      → additive()  // +, -
        → multiplicative()  // *, /, %
          → unary()  // +, -, NOT
            → primary()  // literals, ()
```

**Fix**: Ensure each level calls the correct next level

---

### Problem: Expression parsing hangs (infinite loop)

**Symptoms**: Program freezes

**Cause**: Missing advance() in operator parsing

**Debug**:
```cpp
shared_ptr<Expression> Parser::additive() {
    auto left = multiplicative();
    
    while (true) {
        if (match(TOKEN_PLUS)) {  // ← match() includes advance()
            auto right = multiplicative();
            left = make_shared<BinaryOp>("+", left, right);
        } else {
            break;  // ← Must have break to exit loop
        }
    }
    return left;
}
```

**Fix**: Always call `match()` or `advance()` and ensure loop termination

---

### Problem: Variables not being stored in declarations

**Symptoms**: Declared variables undefined at runtime

**Check**:
```cpp
// Make sure both passes happen:
// First pass: declarations
for (auto& decl : program->declarations) {
    executeStatement(decl);
}

// Second pass: statements  
for (auto& stmt : program->statements) {
    executeStatement(stmt);
}
```

---

## Interpreter Issues

### Problem: Variables not retaining values

**Symptoms**: Print statement shows 0 or old value

**Check symbol table**:
```cpp
// Add debug function
void Interpreter::debugPrintSymbolTable() {
    cout << "\n[DEBUG] Symbol Table:" << endl;
    for (auto& pair : symbolTable) {
        cout << "  " << pair.first << " = " 
             << pair.second.second.toString() 
             << " (" << pair.first << ")" << endl;
    }
    cout << endl;
}

// Call after each statement
executeStatement(stmt);
debugPrintSymbolTable();
```

**Common Causes**:
1. New symbol table for each statement (should be persistent)
2. Assignment not updating symbol table
3. Type mismatch causing exception

---

### Problem: Print output wrong

**Symptoms**: Output missing newlines or concatenation

**Check**:
```cpp
// PRINT: a & $ & b should output: a[newline]b
// 
// Debug each expression:
if (auto print = dynamic_pointer_cast<PrintStatement>(stmt)) {
    for (auto& expr : print->expressions) {
        Value val = evaluateExpression(expr);
        cout << "[DEBUG] Print expr: '" << val.toString() << "'" << endl;
    }
}
```

**Common Mistakes**:
1. `$` should output newline, not stored in output string
2. Escape sequences like `[#]` should print as `#`
3. String concatenation with `&` not working

---

### Problem: Loop executes wrong number of times

**Symptoms**: REPEAT WHEN looping forever or not at all

**Check loop condition**:
```cpp
// REPEAT WHEN loops WHILE condition is true
if (auto repStmt = dynamic_pointer_cast<RepeatStatement>(stmt)) {
    cout << "[DEBUG] Initial condition: " 
         << evaluateExpression(repStmt->condition).toString() << endl;
    
    int iterations = 0;
    while (evaluateExpression(repStmt->condition).toBool()) {
        // Execute body
        iterations++;
        if (iterations > 10000) {  // Safety check
            throw runtime_error("Infinite loop detected");
        }
    }
    
    cout << "[DEBUG] Loop executed " << iterations << " times" << endl;
}
```

---

### Problem: Input (SCAN) not working

**Symptoms**: Program hangs or crashes on SCAN

**Check**:
```cpp
if (auto scan = dynamic_pointer_cast<ScanStatement>(stmt)) {
    cout << "[DEBUG] Scanning " << scan->variables.size() 
         << " variables" << endl;
    
    string input;
    cout << "Enter values separated by commas: ";
    getline(cin, input);
    
    cout << "[DEBUG] Received input: '" << input << "'" << endl;
    
    // Parse and store values
}
```

**Common Issues**:
1. Variable doesn't exist (must be declared first)
2. Type mismatch when converting input
3. Input parsing fails (spaces, wrong format)

---

## Output Issues

### Problem: Output appears in wrong order

**Symptoms**: Debug output mixed with program output

**Solution**:
Use separate streams:
```cpp
// Debug to cerr
cerr << "[DEBUG] Value: " << value << endl;

// Output to cout or output buffer
output << value;
```

---

### Problem: Output has extra spaces or newlines

**Symptoms**: Output "5 " instead of "5"

**Check**:
```cpp
// Don't add extra spaces
return value;  // Right
return value + " ";  // Wrong - creates extra space

// Check $ handling
if (strVal == "$") {
    output <<  "\n";  // Just newline
} else {
    output << strVal;  // No extra characters
}
```

---

## Testing Your Fixes

### Minimal Test Case

```cpp
#include <iostream>
using namespace std;

int main() {
    // Test a single feature in isolation
    string code = R"(
SCRIPT AREA
START SCRIPT
DECLARE INT x=5
PRINT: x
END SCRIPT
)";

    try {
        Lexer lexer(code);
        auto tokens = lexer.tokenize();
        
        // Print tokens for debugging
        cout << "Tokens:" << endl;
        for (auto& t : tokens) {
            cout << "  " << t.value << endl;
        }
        
        Parser parser(tokens);
        auto ast = parser.parse();
        
        Interpreter interp;
        interp.execute(ast);
        
        cout << "Output: [" << interp.getOutput() << "]" << endl;
    } catch (exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    
    return 0;
}
```

---

## Build Verification Checklist

Before debugging runtime issues, verify:
- [ ] All .cpp files compile without errors
- [ ] All .cpp files link (no undefined references)
- [ ] Include paths are correct
- [ ] Header guards are in place
- [ ] No circular includes
- [ ] Headers match implementation

---

## Common Implementation Mistakes

| Mistake | Symptom | Fix |
|---------|---------|-----|
| forget `advance()` in lexer | Infinite loop | Call `advance()` after reading tokens |
| forget `match()` return | Parser stuck | Use `match()` not just `check()` |
| Symbol table reset each stmt | Variables lost | Use single persistent map |
| `toBool()` on non-bool | Type error | Add proper type conversion |
| String concatenation order | Wrong output | Evaluate left-to-right |
| Missing `(` `)` in expressions | Parse error | Check token flow |
| `==` vs `=` | Logic error | Use `==` for comparison, `=` for assignment |

---

## Debug Output Template

Add this to any class:

```cpp
#ifdef DEBUG
    #define DPRINT(x) cerr << "[DEBUG] " << __FUNCTION__ << ": " << x << endl;
#else
    #define DPRINT(x)
#endif

// Usage:
DPRINT("Evaluating expression");
DPRINT("Variable: " << varName << " = " << value.toString());
```

Compile with:
```bash
g++ -DDEBUG -std=c++17 ...   # Debug on
g++ -std=c++17 ...           # Debug off
```

---

## Performance Troubleshooting

### Issue: Program runs slowly

**Profile with timing**:
```cpp
#include <chrono>

auto start = chrono::high_resolution_clock::now();

// ... code to measure

auto end = chrono::high_resolution_clock::now();
auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
cout << "Time: " << duration.count() << " microseconds" << endl;
```

**Common bottlenecks**:
1. String operations in loops - use references
2. Repeated map lookups - cache value
3. Deep recursion - unroll recursive calls

---

## Getting Help

When stuck, provide:
1. The LEXOR source code that fails
2. Error message (copy exact text)
3. Expected vs actual output
4. Steps to reproduce

---

Next: Contact your instructor or see documentation examples for working code.
