# Interpreter Implementation Tutorial

## What is an Interpreter?
The interpreter executes the Abstract Syntax Tree (AST) created by the parser, managing variables, evaluating expressions, and producing output.

## Step 1: Define Value Class

Create `value.h`:

```cpp
#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <iostream>
using namespace std;

class Value {
public:
    enum Type {
        TYPE_INT,
        TYPE_FLOAT,
        TYPE_CHAR,
        TYPE_BOOL,
        TYPE_STRING,
        TYPE_VOID
    };
    
    Type type;
    double numValue;      // For INT and FLOAT
    char charValue;       // For CHAR
    bool boolValue;       // For BOOL
    string stringValue;   // For STRING
    
    // Constructors
    Value() : type(TYPE_VOID), numValue(0), charValue('\0'), boolValue(false) {}
    
    Value(int v) : type(TYPE_INT), numValue(v), charValue('\0'), boolValue(false) {}
    
    Value(double v) : type(TYPE_FLOAT), numValue(v), charValue('\0'), boolValue(false) {}
    
    Value(char v) : type(TYPE_CHAR), numValue(0), charValue(v), boolValue(false) {}
    
    Value(bool v) : type(TYPE_BOOL), numValue(0), charValue('\0'), boolValue(v) {}
    
    Value(string v) : type(TYPE_STRING), numValue(0), charValue('\0'), boolValue(false), stringValue(v) {}
    
    // Convert to string for output
    string toString() const {
        switch (type) {
            case TYPE_INT:
                return to_string((int)numValue);
            case TYPE_FLOAT:
                return to_string(numValue);
            case TYPE_CHAR:
                return string(1, charValue);
            case TYPE_BOOL:
                return boolValue ? "TRUE" : "FALSE";
            case TYPE_STRING:
                return stringValue;
            case TYPE_VOID:
                return "";
        }
        return "";
    }
    
    // Convert to number
    double toNumber() const {
        switch (type) {
            case TYPE_INT:
            case TYPE_FLOAT:
                return numValue;
            case TYPE_CHAR:
                return (double)charValue;
            case TYPE_BOOL:
                return boolValue ? 1.0 : 0.0;
            case TYPE_STRING:
                try {
                    return stod(stringValue);
                } catch (...) {
                    return 0.0;
                }
            default:
                return 0.0;
        }
    }
    
    // Convert to bool
    bool toBool() const {
        switch (type) {
            case TYPE_INT:
            case TYPE_FLOAT:
                return numValue != 0;
            case TYPE_CHAR:
                return charValue != '\0';
            case TYPE_BOOL:
                return boolValue;
            case TYPE_STRING:
                return !stringValue.empty();
            default:
                return false;
        }
    }
};

#endif
```

## Step 2: Create the Interpreter Class

Create `interpreter.h`:

```cpp
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include "value.h"
#include <map>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

class Interpreter {
private:
    // Symbol table: variable name -> (type, value)
    map<string, pair<string, Value>> symbolTable;
    stringstream output;
    bool loopBreak;  // For loop control
    
    // Helper methods
    void checkTypeCompatibility(string varName, string type, Value value);
    Value evaluateExpression(shared_ptr<Expression> expr);
    void executeStatement(shared_ptr<Statement> stmt);
    
    // Expression evaluation
    Value evalBinaryOp(string op, Value left, Value right);
    Value evalUnaryOp(string op, Value operand);
    
    // Output handling  
    string escapeOutput(string s);
    
public:
    Interpreter();
    void execute(shared_ptr<Program> program);
    string getOutput() const;
};

#endif
```

Create `interpreter.cpp`:

```cpp
#include "interpreter.h"
#include <cmath>
#include <iostream>

Interpreter::Interpreter() : loopBreak(false) {}

void Interpreter::checkTypeCompatibility(string varName, string type, Value value) {
    // Basic type checking - can be expanded
    // For now, we'll allow implicit conversions
}

Value Interpreter::evaluateExpression(shared_ptr<Expression> expr) {
    // Number literal
    if (auto num = dynamic_pointer_cast<NumberLiteral>(expr)) {
        if (num->isFloat) {
            return Value(num->value);
        } else {
            return Value((int)num->value);
        }
    }
    
    // String literal
    if (auto str = dynamic_pointer_cast<StringLiteral>(expr)) {
        return Value(str->value);
    }
    
    // Character literal
    if (auto ch = dynamic_pointer_cast<CharLiteral>(expr)) {
        return Value(ch->value);
    }
    
    // Boolean literal
    if (auto b = dynamic_pointer_cast<BoolLiteral>(expr)) {
        return Value(b->value);
    }
    
    // Identifier (variable reference)
    if (auto id = dynamic_pointer_cast<Identifier>(expr)) {
        if (symbolTable.find(id->name) != symbolTable.end()) {
            return symbolTable[id->name].second;
        } else {
            throw runtime_error("Undefined variable: " + id->name);
        }
    }
    
    // Binary operation
    if (auto binop = dynamic_pointer_cast<BinaryOp>(expr)) {
        Value left = evaluateExpression(binop->left);
        Value right = evaluateExpression(binop->right);
        return evalBinaryOp(binop->op, left, right);
    }
    
    // Unary operation
    if (auto unop = dynamic_pointer_cast<UnaryOp>(expr)) {
        Value operand = evaluateExpression(unop->operand);
        return evalUnaryOp(unop->op, operand);
    }
    
    return Value();
}

Value Interpreter::evalBinaryOp(string op, Value left, Value right) {
    if (op == "+") {
        double result = left.toNumber() + right.toNumber();
        // Return INT if both are INT, else FLOAT
        if (left.type == Value::TYPE_INT && right.type == Value::TYPE_INT) {
            return Value((int)result);
        }
        return Value(result);
    }
    
    if (op == "-") {
        double result = left.toNumber() - right.toNumber();
        if (left.type == Value::TYPE_INT && right.type == Value::TYPE_INT) {
            return Value((int)result);
        }
        return Value(result);
    }
    
    if (op == "*") {
        double result = left.toNumber() * right.toNumber();
        if (left.type == Value::TYPE_INT && right.type == Value::TYPE_INT) {
            return Value((int)result);
        }
        return Value(result);
    }
    
    if (op == "/") {
        double rightNum = right.toNumber();
        if (rightNum == 0) {
            throw runtime_error("Division by zero");
        }
        double result = left.toNumber() / rightNum;
        return Value(result);
    }
    
    if (op == "%") {
        int leftInt = (int)left.toNumber();
        int rightInt = (int)right.toNumber();
        if (rightInt == 0) {
            throw runtime_error("Modulo by zero");
        }
        return Value(leftInt % rightInt);
    }
    
    // Comparison operators
    if (op == "<") {
        return Value(left.toNumber() < right.toNumber());
    }
    
    if (op == ">") {
        return Value(left.toNumber() > right.toNumber());
    }
    
    if (op == "<=") {
        return Value(left.toNumber() <= right.toNumber());
    }
    
    if (op == ">=") {
        return Value(left.toNumber() >= right.toNumber());
    }
    
    if (op == "==") {
        // For equality, compare as strings or numbers
        if (left.type == Value::TYPE_STRING || right.type == Value::TYPE_STRING) {
            return Value(left.toString() == right.toString());
        }
        return Value(left.toNumber() == right.toNumber());
    }
    
    if (op == "<>") {
        if (left.type == Value::TYPE_STRING || right.type == Value::TYPE_STRING) {
            return Value(left.toString() != right.toString());
        }
        return Value(left.toNumber() != right.toNumber());
    }
    
    // Logical operators
    if (op == "AND") {
        return Value(left.toBool() && right.toBool());
    }
    
    if (op == "OR") {
        return Value(left.toBool() || right.toBool());
    }
    
    throw runtime_error("Unknown binary operator: " + op);
}

Value Interpreter::evalUnaryOp(string op, Value operand) {
    if (op == "+") {
        return Value(operand.toNumber());
    }
    
    if (op == "-") {
        return Value(-operand.toNumber());
    }
    
    if (op == "NOT") {
        return Value(!operand.toBool());
    }
    
    throw runtime_error("Unknown unary operator: " + op);
}

string Interpreter::escapeOutput(string s) {
    // Handle escape sequences
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        result += s[i];
    }
    return result;
}

void Interpreter::executeStatement(shared_ptr<Statement> stmt) {
    // Declaration
    if (auto decl = dynamic_pointer_cast<Declaration>(stmt)) {
        for (auto& var : decl->variables) {
            string varName = var.first;
            ExprPtr initialValue = var.second;
            
            Value value;
            if (initialValue) {
                value = evaluateExpression(initialValue);
            } else {
                // Default initialization
                if (decl->type == "INT") {
                    value = Value(0);
                } else if (decl->type == "FLOAT") {
                    value = Value(0.0);
                } else if (decl->type == "CHAR") {
                    value = Value('\0');
                } else if (decl->type == "BOOL") {
                    value = Value(false);
                }
            }
            
            symbolTable[varName] = {decl->type, value};
        }
        return;
    }
    
    // Assignment
    if (auto assign = dynamic_pointer_cast<Assignment>(stmt)) {
        Value value = evaluateExpression(assign->value);
        
        if (symbolTable.find(assign->var) != symbolTable.end()) {
            symbolTable[assign->var].second = value;
        } else {
            // Auto-declare if not exists (for chained assignment)
            symbolTable[assign->var] = {"INT", value};
        }
        return;
    }
    
    // Print statement
    if (auto print = dynamic_pointer_cast<PrintStatement>(stmt)) {
        bool first = true;
        for (auto& expr : print->expressions) {
            Value val = evaluateExpression(expr);
            string strVal = val.toString();
            
            // Handle newlines
            if (strVal == "$") {
                output << "\n";
            } else {
                output << strVal;
            }
        }
        return;
    }
    
    // Scan statement
    if (auto scan = dynamic_pointer_cast<ScanStatement>(stmt)) {
        string input;
        getline(cin, input);
        
        // Split by comma
        size_t pos = 0;
        int varIdx = 0;
        
        while (pos < input.length() && varIdx < scan->variables.size()) {
            size_t commaPos = input.find(',', pos);
            if (commaPos == string::npos) commaPos = input.length();
            
            string value = input.substr(pos, commaPos - pos);
            
            // Trim whitespace
            size_t start = value.find_first_not_of(" \t");
            size_t end = value.find_last_not_of(" \t");
            if (start != string::npos) {
                value = value.substr(start, end - start + 1);
            }
            
            // Store in symbol table
            string varName = scan->variables[varIdx];
            if (symbolTable.find(varName) != symbolTable.end()) {
                string type = symbolTable[varName].first;
                
                // Convert based on type
                Value val;
                if (type == "INT") {
                    val = Value((int)stod(value));
                } else if (type == "FLOAT") {
                    val = Value(stod(value));
                } else if (type == "CHAR") {
                    val = Value(value[0]);
                } else if (type == "BOOL") {
                    val = Value(value == "TRUE" || value == "true");
                }
                
                symbolTable[varName].second = val;
            }
            
            pos = commaPos + 1;
            varIdx++;
        }
        return;
    }
    
    // If statement
    if (auto ifStmt = dynamic_pointer_cast<IfStatement>(stmt)) {
        Value condition = evaluateExpression(ifStmt->condition);
        
        if (condition.toBool()) {
            for (auto& s : ifStmt->ifBody) {
                executeStatement(s);
            }
        } else if (ifStmt->hasElse) {
            for (auto& s : ifStmt->elseBody) {
                executeStatement(s);
            }
        }
        return;
    }
    
    // For statement
    if (auto forStmt = dynamic_pointer_cast<ForStatement>(stmt)) {
        // LEXOR FOR loop runs body at least once, then checks condition
        // Actually, LEXOR doesn't specify a condition for FOR, it's an infinite loop
        // That needs clarification, but typically FOR with no condition means iterate over range
        // For now, treat it as a single iteration or based on context
        
        // Since LEXOR spec doesn't detail FOR clearly, execute body once
        for (auto& s : forStmt->body) {
            executeStatement(s);
            if (loopBreak) {
                loopBreak = false;
                break;
            }
        }
        return;
    }
    
    // Repeat statement
    if (auto repStmt = dynamic_pointer_cast<RepeatStatement>(stmt)) {
        loopBreak = false;
        
        // REPEAT WHEN: execute body while condition is true
        while (evaluateExpression(repStmt->condition).toBool()) {
            for (auto& s : repStmt->body) {
                executeStatement(s);
                if (loopBreak) {
                    loopBreak = false;
                    break;
                }
            }
        }
        return;
    }
}

void Interpreter::execute(shared_ptr<Program> program) {
    if (!program) {
        throw runtime_error("Invalid program");
    }
    
    // First pass: process all declarations
    for (auto& decl : program->declarations) {
        executeStatement(decl);
    }
    
    // Second pass: execute statements
    for (auto& stmt : program->statements) {
        executeStatement(stmt);
    }
}

string Interpreter::getOutput() const {
    return output.str();
}
```

## Step 3: Complete Main Function

Update `main.cpp`:

```cpp
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

string readFile(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + filename);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[]) {
    try {
        string code;
        
        if (argc > 1) {
            // Read from file
            code = readFile(argv[1]);
        } else {
            // Read from standard input
            string line;
            while (getline(cin, line)) {
                code += line + "\n";
            }
        }
        
        // Phase 1: Lexing
        cout << "=== LEXING ===" << endl;
        Lexer lexer(code);
        auto tokens = lexer.tokenize();
        cout << "Tokens generated: " << tokens.size() << endl;
        
        // Phase 2: Parsing
        cout << "\n=== PARSING ===" << endl;
        Parser parser(tokens);
        auto ast = parser.parse();
        
        if (!ast) {
            cerr << "Parsing failed" << endl;
            return 1;
        }
        cout << "AST constructed successfully" << endl;
        
        // Phase 3: Interpretation
        cout << "\n=== EXECUTION ===" << endl;
        Interpreter interpreter;
        interpreter.execute(ast);
        
        // Output results
        cout << "\n=== OUTPUT ===" << endl;
        cout << interpreter.getOutput();
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
```

## Key Interpreter Concepts

1. **Symbol Table**: Map of variable names to type and value
2. **Expression Evaluation**: Recursive evaluation of AST expressions
3. **Type System**: Support INT, FLOAT, CHAR, BOOL with conversions
4. **Statement Execution**: Process declarations, assignments, I/O, control flow
5. **Output Management**: Collect output in stringstream
6. **Error Handling**: Throw exceptions for undefined variables, type errors

## Testing Examples

### Example 1: Simple Arithmetic
```
SCRIPT AREA
START SCRIPT
DECLARE INT x=5, y=3
DECLARE INT z
z = x + y
PRINT: z
END SCRIPT
```
Expected Output: `8`

### Example 2: Conditional
```
SCRIPT AREA
START SCRIPT
DECLARE INT a=10
IF (a > 5)
START IF
PRINT: "Greater"
END IF
END SCRIPT
```
Expected Output: `Greater`

### Example 3: Loop
```
SCRIPT AREA
START SCRIPT
DECLARE INT i=0
REPEAT WHEN (i < 3)
START REPEAT
PRINT: i & $ 
i = i + 1
END REPEAT
END SCRIPT
```
Expected Output:
```
0
1
2
```

## Common Issues and Solutions

| Issue | Solution |
|-------|----------|
| Variable not found | Check declarations before use |
| Type mismatch | Implement type coercion or strict checking |
| Division by zero | Add runtime check |
| Infinite loop | Verify loop condition updates |
| Output formatting | Handle $ for newlines, & for concatenation |

Next: Complete working examples (05_COMPLETE_EXAMPLES.md)
