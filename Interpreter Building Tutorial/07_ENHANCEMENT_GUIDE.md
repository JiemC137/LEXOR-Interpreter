# Enhancement Guide for LEXOR Interpreter

## Features to Add After Basic Implementation

### 1. Error Handling and Recovery

#### Current State:
Simple exception throwing

#### Enhancement:
```cpp
// Enhanced error reporting
class LexorError : public exception {
public:
    int line, column;
    string message;
    
    LexorError(int l, int c, string msg) 
        : line(l), column(c), message(msg) {}
    
    const char* what() const noexcept {
        return message.c_str();
    }
    
    void printError() const {
        cerr << "Error at line " << line << ", column " << column 
             << ": " << message << endl;
    }
};

// Error recovery in parser
vector<Statement*> statements;
while (!check(TOKEN_END)) {
    try {
        statements.push_back(statement());
    } catch (const LexorError& e) {
        e.printError();
        synchronize();  // Skip to next statement
    }
}

void Parser::synchronize() {
    advance();
    while (!check(TOKEN_EOF)) {
        if (peek().type == TOKEN_PRINT || 
            peek().type == TOKEN_SCAN ||
            peek().type == TOKEN_IF) {
            return;
        }
        advance();
    }
}
```

---

### 2. Scoped Variables (Functions/Procedures)

#### Future LEXOR Extension:
```
SCRIPT AREA
START SCRIPT

DECLARE PROC int add(int a, int b)
START PROC
DECLARE int result = a + b
RETURN result
END PROC

DECLARE INT x=5, y=3
DECLARE INT sum
sum = add(x, y)
PRINT: sum
END SCRIPT
```

#### Implementation:
```cpp
class FunctionDeclaration : public Statement {
public:
    string name;
    string returnType;
    vector<pair<string, string>> parameters;  // name, type
    vector<StmtPtr> body;
};

class FunctionCall : public Expression {
public:
    string name;
    vector<ExprPtr> arguments;
};

// Symbol table with scopes
class SymbolTable {
private:
    vector<map<string, pair<string, Value>>> scopes;
    
public:
    void pushScope() {
        scopes.push_back({});
    }
    
    void popScope() {
        scopes.pop_back();
    }
    
    void declare(string name, string type, Value value) {
        scopes.back()[name] = {type, value};
    }
    
    Value get(string name) {
        for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
            if (it->find(name) != it->end()) {
                return it->at(name).second;
            }
        }
        throw runtime_error("Undefined variable: " + name);
    }
};
```

---

### 3. Arrays/Lists

#### Proposed Syntax:
```
DECLARE ARRAY INT numbers = [1, 2, 3, 4, 5]
DECLARE ARRAY CHAR letters = ['a', 'b', 'c']

PRINT: numbers[0]  %% Output: 1
```

#### Implementation:
```cpp
class ArrayValue : public Value {
public:
    vector<Value> elements;
    
    Value getElementAt(int index) {
        if (index < 0 || index >= elements.size()) {
            throw runtime_error("Array index out of bounds");
        }
        return elements[index];
    }
    
    void setElementAt(int index, Value val) {
        if (index < 0 || index >= elements.size()) {
            throw runtime_error("Array index out of bounds");
        }
        elements[index] = val;
    }
};

class ArrayAccess : public Expression {
public:
    string arrayName;
    ExprPtr index;
};
```

---

### 4. String Operations

#### Proposed Functions:
```
LENGTH(str)        %% Get string length
SUBSTR(str, pos, len)  %% Extract substring
INDEXOF(str, char)  %% Find character position
UPPER(str)         %% Convert to uppercase
LOWER(str)         %% Convert to lowercase
```

#### Implementation:
```cpp
class BuiltinFunction : public Expression {
public:
    string name;
    vector<ExprPtr> arguments;
};

Value callBuiltinFunction(string name, vector<ExprPtr>& args) {
    if (name == "LENGTH") {
        Value str = evaluateExpression(args[0]);
        return Value((int)str.toString().length());
    }
    
    if (name == "UPPER") {
        Value str = evaluateExpression(args[0]);
        string s = str.toString();
        for (char& c : s) {
            c = toupper(c);
        }
        return Value(s);
    }
    
    // ... more functions
}
```

---

### 5. File I/O Operations

#### Proposed Syntax:
```
FILE OPEN "data.txt" AS input_file
FILE READ input_file INTO line_var
FILE CLOSE input_file

FILE WRITE "output.txt" "Hello World"
```

#### Implementation:
```cpp
#include <fstream>

class FileHandle {
public:
    string filename;
    ifstream* inputFile;
    ofstream* outputFile;
    bool isInput;
};

map<string, FileHandle> openFiles;

void executeFileOpen(string filename, string mode, string handleName) {
    FileHandle handle;
    handle.filename = filename;
    handle.isInput = (mode == "input");
    
    if (handle.isInput) {
        handle.inputFile = new ifstream(filename);
        if (!handle.inputFile->is_open()) {
            throw runtime_error("Cannot open file: " + filename);
        }
    } else {
        handle.outputFile = new ofstream(filename);
        if (!handle.outputFile->is_open()) {
            throw runtime_error("Cannot create file: " + filename);
        }
    }
    
    openFiles[handleName] = handle;
}
```

---

### 6. Debug Mode

#### Feature: Verbose Execution Tracing

```cpp
class Interpreter {
private:
    bool debugMode;
    
public:
    void setDebugMode(bool debug) {
        debugMode = debug;
    }
    
    void executeStatement(shared_ptr<Statement> stmt) {
        if (debugMode) {
            cerr << "[DEBUG] Executing: " << stmt->type() << endl;
        }
        // ... rest of execution
    }
    
    void printSymbolTable() {
        cerr << "\n[DEBUG] Symbol Table:" << endl;
        for (auto& pair : symbolTable) {
            cerr << "  " << pair.first << " = " 
                 << pair.second.second.toString() << endl;
        }
    }
};
```

Usage:
```cpp
interpreter.setDebugMode(true);
interpreter.execute(ast);
```

---

### 7. Expression Optimization

#### Constant Folding:
```cpp
// Optimize: 5 + 3 → 8 (at parse time)
shared_ptr<Expression> optimizeExpression(shared_ptr<Expression> expr) {
    if (auto binop = dynamic_pointer_cast<BinaryOp>(expr)) {
        auto left = dynamic_pointer_cast<NumberLiteral>(binop->left);
        auto right = dynamic_pointer_cast<NumberLiteral>(binop->right);
        
        if (left && right) {
            Value result = evalBinaryOp(binop->op, 
                                       Value(left->value), 
                                       Value(right->value));
            return make_shared<NumberLiteral>(result.toNumber(), 
                                            result.type == Value::TYPE_FLOAT);
        }
    }
    return expr;
}
```

---

### 8. Type System Enhancement

#### Stricter Type Checking:
```cpp
bool isCompatible(string declaredType, Value value) {
    if (declaredType == "INT" && value.type == Value::TYPE_INT) 
        return true;
    if (declaredType == "FLOAT" && 
        (value.type == Value::TYPE_FLOAT || value.type == Value::TYPE_INT))
        return true;
    if (declaredType == "CHAR" && value.type == Value::TYPE_CHAR)
        return true;
    if (declaredType == "BOOL" && value.type == Value::TYPE_BOOL)
        return true;
    return false;
}

void Assignment::execute(Interpreter& interp) {
    Value val = interp.evaluateExpression(value);
    string varType = interp.getVariableType(var);
    
    if (!isCompatible(varType, val)) {
        throw TypeError("Cannot assign " + val.type 
                       + " to " + varType);
    }
    interp.setVariable(var, val);
}
```

---

### 9. Loop Break/Continue

#### Proposed Syntax:
```
REPEAT WHEN (count < 10)
START REPEAT
IF (count == 5)
START IF
BREAK
END IF
count = count + 1
END REPEAT
```

#### Implementation:
```cpp
enum LoopControl {
    NORMAL,
    BREAK,
    CONTINUE
};

class Interpreter {
private:
    LoopControl loopControl;
    
public:
    void setBreak() { loopControl = BREAK; }
    void setContinue() { loopControl = CONTINUE; }
    bool shouldBreak() { return loopControl == BREAK; }
    bool shouldContinue() { return loopControl == CONTINUE; }
    
    void executeRepeatStatement(shared_ptr<RepeatStatement> stmt) {
        while (evaluateExpression(stmt->condition).toBool()) {
            loopControl = NORMAL;
            for (auto& s : stmt->body) {
                executeStatement(s);
                if (loopControl != NORMAL) break;
            }
            if (loopControl == BREAK) {
                loopControl = NORMAL;
                break;
            }
        }
    }
};
```

---

### 10. Comments and Documentation

#### Inline Documentation:
```
%%! FUNCTION: calculateSum
%%! INPUT: a (INT), b (INT)
%%! OUTPUT: INT (sum of a and b)
DECLARE INT result = a + b

%%! Loop from 0 to 10
REPEAT WHEN (i < 10)
START REPEAT
...
END REPEAT
```

#### Implementation:
Extract comments and generate documentation automatically

---

### 11. Performance Metrics

```cpp
class PerformanceMonitor {
private:
    chrono::high_resolution_clock::time_point startTime;
    map<string, long long> executionTimes;
    
public:
    void start() {
        startTime = chrono::high_resolution_clock::now();
    }
    
    void recordExecution(string statementType) {
        auto endTime = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>
                       (endTime - startTime).count();
        executionTimes[statementType] += duration;
    }
    
    void printReport() {
        cout << "\n=== Performance Report ===" << endl;
        for (auto& pair : executionTimes) {
            cout << pair.first << ": " << pair.second << " µs" << endl;
        }
    }
};
```

---

### 12. REPL (Read-Eval-Print Loop)

```cpp
class REPL {
public:
    void run() {
        string line;
        while (true) {
            cout << "lexor> ";
            getline(cin, line);
            
            if (line == "exit") break;
            if (line == "vars") {
                printSymbolTable();
                continue;
            }
            
            try {
                Lexer lexer(line);
                Parser parser(lexer.tokenize());
                // Execute single statement
            } catch (exception& e) {
                cerr << "Error: " << e.what() << endl;
            }
        }
    }
};
```

---

### 13. Inline Assembly (Advanced)

For performance-critical operations:
```cpp
// x86 inline assembly for speed-critical path
int fastAdd(int a, int b) {
    int result;
    asm("addl %%ebx,%%eax;" : "=a" (result) : "a" (a), "b" (b));
    return result;
}
```

---

### 14. Unit Testing Framework

```cpp
class TestCase {
public:
    string name;
    string sourceCode;
    string expectedOutput;
    
    bool run() {
        try {
            Lexer lexer(sourceCode);
            Parser parser(lexer.tokenize());
            Interpreter interp;
            interp.execute(parser.parse());
            
            string output = interp.getOutput();
            if (output == expectedOutput) {
                cout << "✓ " << name << endl;
                return true;
            } else {
                cout << "✗ " << name << endl;
                cout << "  Expected: " << expectedOutput << endl;
                cout << "  Got: " << output << endl;
                return false;
            }
        } catch (exception& e) {
            cout << "✗ " << name << " (Exception)" << endl;
            cout << "  " << e.what() << endl;
            return false;
        }
    }
};

int main() {
    vector<TestCase> tests = {
        {"Simple addition", "SCRIPT AREA START SCRIPT DECLARE INT x=5 END SCRIPT", ""},
        // ... more tests
    };
    
    int passed = 0;
    for (auto& test : tests) {
        if (test.run()) passed++;
    }
    
    cout << "\n" << passed << "/" << tests.size() << " tests passed" << endl;
}
```

---

### 15. Plugin System (Advanced)

```cpp
// Load external C++ code as extensions
class Plugin {
public:
    virtual ~Plugin() = default;
    virtual void initialize(Interpreter& interp) = 0;
    virtual string getName() = 0;
};

class PluginManager {
private:
    map<string, Plugin*> plugins;
    
public:
    void loadPlugin(string pluginPath) {
        // Load .dll/.so dynamically
        // Call initialize on plugin
    }
    
    void callPluginFunction(string pluginName, string funcName) {
        // Invoke plugin function
    }
};
```

---

## Priority Implementation Order

1. **High Priority** (Before submission)
   - Basic error handling
   - All data types working
   - All operators implemented
   - All control flow structures

2. **Medium Priority** (Nice to have)
   - Enhanced error messages
   - Type checking
   - Debug mode
   - Multiple input files

3. **Low Priority** (Future enhancements)
   - Functions/Procedures
   - Arrays
   - File I/O
   - Performance optimization
   - Plugin system

---

## Testing Your Enhancements

```cpp
// Test template
void testFeature(string name, string code, string expected) {
    cout << "Testing: " << name << "... ";
    try {
        Lexer lexer(code);
        Parser parser(lexer.tokenize());
        Interpreter interp;
        interp.execute(parser.parse());
        
        if (interp.getOutput() == expected) {
            cout << "PASS" << endl;
        } else {
            cout << "FAIL" << endl;
            cout << "  Expected: [" << expected << "]" << endl;
            cout << "  Got: [" << interp.getOutput() << "]" << endl;
        }
    } catch (exception& e) {
        cout << "ERROR: " << e.what() << endl;
    }
}
```

---

See: 07_ADVANCED_TECHNIQUES.md for more implementation patterns
