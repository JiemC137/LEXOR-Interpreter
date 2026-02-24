# LEXOR Interpreter in C++ - Complete Implementation Guide

A comprehensive tutorial series for building a complete LEXOR programming language interpreter in C++, from scratch.

## 📖 What Is This?

This is a **complete, structured guide** for implementing a working LEXOR interpreter. It includes:

- ✅ **Detailed architecture documentation**
- ✅ **Step-by-step implementation tutorials**
- ✅ **Complete C++ code examples**
- ✅ **15 test programs with expected outputs**
- ✅ **Build instructions for all platforms**
- ✅ **Troubleshooting and debugging tips**
- ✅ **Enhancement suggestions for advanced features**

## 🚀 Quick Start (5 minutes)

### Prerequisites
- C++17 compiler (GCC, Clang, MSVC)
- CMake 3.10+ or make/Visual Studio

### Setup and Build
```bash
# Navigate to project folder
cd "C:\Users\Maykel\Desktop\C++ cit\Interpreter"

# CMake build (recommended)
mkdir build
cd build
cmake ..
cmake --build . --config Release

# Or direct compilation (Windows)
cl /std:c++17 /O2 /I.\include src\main.cpp src\lexer.cpp src\parser.cpp src\interpreter.cpp /Fe:interpreter.exe

# Or g++/clang (Linux/Mac/MinGW)
g++ -std=c++17 -O2 -I./include src/main.cpp src/lexer.cpp src/parser.cpp src/interpreter.cpp -o interpreter

# Run test
./interpreter examples/hello.lexor
# Expected output: Hello\nWorld
```

## 📚 Documentation Files

### Essential Reading (Start with these!)

| File | Description | Time |
|------|-------------|------|
| **10_INDEX.md** | Navigation guide & learning path | 5 min |
| **01_ARCHITECTURE_GUIDE.md** | System design overview | 20 min |
| **08_QUICK_REFERENCE.md** | Quick lookup tables | 10 min |

### Implementation Tutorials (Step-by-step coding)

| File | Topic | Time |
|------|-------|------|
| **02_LEXER_TUTORIAL.md** | Token scanning (Lexer phase) | 60 min |
| **03_PARSER_TUTORIAL.md** | AST building (Parser phase) | 75 min |
| **04_INTERPRETER_TUTORIAL.md** | Program execution (Interpreter phase) | 75 min |

### Examples & Testing

| File | Description | Time |
|------|-------------|------|
| **05_COMPLETE_EXAMPLES.md** | 15 test programs | 30 min |
| **06_BUILD_GUIDE.md** | Compilation on all platforms | 20 min |

### Advanced Topics

| File | Description | Time |
|------|-------------|------|
| **07_ENHANCEMENT_GUIDE.md** | Advanced features & optimizations | 40 min |
| **09_TROUBLESHOOTING_GUIDE.md** | Debugging & error fixes | 30 min |

**Total Documentation: ~365 minutes (6 hours) of reading**
**Estimated Implementation Time: 40-60 hours**
**Total Project Time: 46-66 hours (1-2 weeks)**

## 🎯 The Three Phases

### Phase 1: Lexer (Tokenizer)
Converts text → Tokens
```
Input:  "DECLARE INT x=5"
Output: [DECLARE] [INT] [IDENTIFIER:x] [=] [NUMBER:5]
```
**File**: 02_LEXER_TUTORIAL.md

### Phase 2: Parser
Converts Tokens → AST
```
Input:  [DECLARE] [INT] [ID:x] [=] [5]
Output: Declaration(type=INT, var=x, value=5)
```
**File**: 03_PARSER_TUTORIAL.md

### Phase 3: Interpreter
Converts AST → Result
```
Input:  Declaration(type=INT, var=x, value=5)
Effect: Store x=5 in memory
```
**File**: 04_INTERPRETER_TUTORIAL.md

## 📁 Project Structure

```
C:\Users\Maykel\Desktop\C++ cit\Interpreter\
├── Documentation Files
│   ├── 01_ARCHITECTURE_GUIDE.md
│   ├── 02_LEXER_TUTORIAL.md
│   ├── 03_PARSER_TUTORIAL.md
│   ├── 04_INTERPRETER_TUTORIAL.md
│   ├── 05_COMPLETE_EXAMPLES.md
│   ├── 06_BUILD_GUIDE.md
│   ├── 07_ENHANCEMENT_GUIDE.md
│   ├── 08_QUICK_REFERENCE.md
│   ├── 09_TROUBLESHOOTING_GUIDE.md
│   ├── 10_INDEX.md (Navigator)
│   └── README.md (This file)
│
├── Build & Configuration
│   ├── CMakeLists.txt
│   └── build/ (generated after compilation)
│
├── Source Code (You'll create these files from tutorials)
│   ├── include/
│   │   ├── token.h
│   │   ├── ast.h
│   │   ├── value.h
│   │   ├── lexer.h
│   │   ├── parser.h
│   │   └── interpreter.h
│   │
│   └── src/
│       ├── main.cpp
│       ├── lexer.cpp
│       ├── parser.cpp
│       └── interpreter.cpp
│
└── Examples
    ├── hello.lexor
    ├── arithmetic.lexor
    ├── logical.lexor
    ├── ifstatement.lexor
    ├── repeatloop.lexor
    └── ... (15 total examples)
```

## 🎓 Learning Path

### Day 1: Architecture
- Read: 01_ARCHITECTURE_GUIDE.md (20 min)
- Read: 08_QUICK_REFERENCE.md (10 min)
- Understand three-phase flow
- **Time: 0.5 hour**

### Day 2-3: Lexer Implementation
- Read: 02_LEXER_TUTORIAL.md (60 min)
- Code: Implement token.h (30 min)
- Code: Implement lexer.h/lexer.cpp (90 min)
- Test: Run first lexer test (30 min)
- **Time: 6 hours**

### Day 4-5: Parser Implementation
- Read: 03_PARSER_TUTORIAL.md (75 min)
- Code: Implement ast.h (30 min)
- Code: Implement parser.h/parser.cpp (120 min)
- Test: Parse simple program (30 min)
- **Time: 7 hours**

### Day 6-7: Interpreter Implementation
- Read: 04_INTERPRETER_TUTORIAL.md (75 min)
- Code: Implement value.h (30 min)
- Code: Implement interpreter.h/interpreter.cpp (120 min)
- Code: Create main.cpp (30 min)
- **Time: 5 hours**

### Day 8: Testing & Debugging
- Read: 05_COMPLETE_EXAMPLES.md (30 min)
- Test: Run all 15 examples (60 min)
- Debug: Fix failing tests (120 min)
- **Time: 4 hours**

### Day 9: Polish & Optimization
- Read: 07_ENHANCEMENT_GUIDE.md (30 min)
- Refactor: Code cleanup (90 min)
- Add: Error handling improvements (60 min)
- Test: Verify nothing broke (30 min)
- **Time: 3.5 hours**

**Total: ~26 hours of work**

## 💻 Language Features

LEXOR supports:

### Data Types
- `INT` - 32-bit integers
- `FLOAT` - Floating point numbers
- `CHAR` - Single characters
- `BOOL` - Boolean values

### Operators
- **Arithmetic**: `+`, `-`, `*`, `/`, `%`
- **Comparison**: `<`, `>`, `<=`, `>=`, `==`, `<>`
- **Logical**: `AND`, `OR`, `NOT`
- **String**: `&` (concatenation), `$` (newline)

### Control Flow
```lexor
IF (condition)
START IF
  statements
END IF
ELSE
START IF
  statements
END IF

REPEAT WHEN (condition)
START REPEAT
  statements
END REPEAT
```

### I/O
```lexor
PRINT: value1 & value2 & $
SCAN: variable1, variable2
```

### Example Program
```lexor
%% Calculate factorial
SCRIPT AREA
START SCRIPT
DECLARE INT n=5, result=1, i=1

REPEAT WHEN (i <= n)
START REPEAT
  result = result * i
  i = i + 1
END REPEAT

PRINT: "Factorial of " & n & " is " & result
END SCRIPT
```

Output:
```
Factorial of 5 is 120
```

## ✅ Success Criteria

Your interpreter is complete when:

- ✓ **Lexer Phase**
  - All keywords recognized
  - Comments skipped properly
  - All operators tokenized
  - Numbers/strings parsed correctly

- ✓ **Parser Phase**
  - Correct AST structure built
  - Operator precedence correct
  - All statements parsed
  - Error messages helpful

- ✓ **Interpreter Phase**
  - Variables stored and retrieved
  - Expressions evaluated correctly
  - All control structures work
  - Output formatted properly

- ✓ **Testing**
  - All 15 examples pass
  - Original sample program works
  - Edge cases handled gracefully
  - No crashes on invalid input

## 🔧 Building the Interpreter

### Option 1: CMake (Recommended)
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
./bin/interpreter.exe ../examples/hello.lexor
```

### Option 2: Visual Studio (Windows)
```
1. Create new C++ Console Project
2. Copy header files to include/
3. Copy source files to src/
4. Add include directory path
5. Build → Build Solution
```

### Option 3: Command Line (Windows MSVC)
```bash
cl /std:c++17 /O2 /I.\include src\*.cpp /Fe:interpreter.exe
interpreter.exe examples\hello.lexor
```

### Option 4: GCC/Clang (Linux/Mac)
```bash
g++ -std=c++17 -O2 -I./include src/*.cpp -o interpreter
./interpreter examples/hello.lexor
```

**See 06_BUILD_GUIDE.md for detailed platform-specific instructions**

## 🧪 Testing

### Run Individual Example
```bash
interpreter examples/arithmetic.lexor
```

### Run All Examples
```bash
# On Linux/Mac
for f in examples/*.lexor; do
  echo "Testing $(basename $f)..."
  ./interpreter "$f"
done

# On Windows
for %%f in (examples\*.lexor) do (
  echo Testing %%~nf
  interpreter.exe "%%f"
)
```

### Expected Test Results
- Example 1 (Hello): `Hello\nWorld`
- Example 2 (Arithmetic): `5 lines of calculation output`
- Example 3 (Comparison): `TRUE FALSE TRUE TRUE`
- Example 4 (Logical): `FALSE TRUE FALSE`
- Example 5 (If): `You are an adult`
- ...see 05_COMPLETE_EXAMPLES.md for all 15

## 🐛 Debugging

### Enable Debug Output
Add to your code:
```cpp
cerr << "[DEBUG] Variable x = " << symbolTable["x"].second.toString() << endl;
```

### Run with GDB
```bash
gdb ./interpreter
(gdb) break parser.cpp:150
(gdb) run examples/hello.lexor
(gdb) step
(gdb) print symbolTable
```

**See 09_TROUBLESHOOTING_GUIDE.md for more debugging techniques**

## 📈 Performance

Expected performance:
- Parse a 100-line program: < 1ms
- Execute simple loop 1000 times: < 10ms
- Handle 1000 variables: < 100ms

**See 07_ENHANCEMENT_GUIDE.md for optimization techniques**

## 🎓 What You'll Learn

By completing this project, you will understand:

- **Interpreter Architecture**
  - Lexical analysis
  - Syntax analysis
  - Semantic analysis

- **C++ Programming**
  - Object-oriented design
  - Memory management (smart pointers)
  - Standard containers
  - Exception handling

- **Compiler Techniques**
  - Tokenization
  - Parsing (recursive descent)
  - AST representation and traversal
  - Symbol table management
  - Expression evaluation

- **Language Implementation**
  - Type systems
  - Scope and binding
  - Control flow
  - Runtime behavior

## 🚀 Next Steps After Completion

### Enhancement Ideas
1. Add function declarations and calls
2. Implement arrays and array indexing
3. Add file I/O operations
4. Implement string built-in functions
5. Add break/continue statements
6. Create interactive REPL mode
7. Add performance profiling
8. Implement unit testing framework

### Real-World Applications
- Create domain-specific languages
- Build configuration file parsers
- Develop game scripting engines
- Create data processing pipelines
- Implement educational tools

## 🤝 Contributing

Found an issue or improvement?

1. Test thoroughly
2. Document the problem
3. Provide a test case
4. Suggest a fix
5. Include code example

## 📝 License

This documentation and code examples are provided for educational purposes.
Use freely for learning and personal projects.

## 💬 Getting Help

If you get stuck:

1. **Check 09_TROUBLESHOOTING_GUIDE.md** for your specific error
2. **Look at 05_COMPLETE_EXAMPLES.md** for working code
3. **Review the relevant tutorial** for that phase
4. **Enable debug output** to trace execution
5. **Compare with tutorial code** line by line

## 🎯 Success Story

With this guide, you should be able to:
- [ ] Day 1: Understand architecture
- [ ] Day 2-3: Complete lexer
- [ ] Day 4-5: Complete parser
- [ ] Day 6-7: Complete interpreter
- [ ] Day 8: Pass all tests
- [ ] Day 9: Optimize and polish
- [ ] **Result: A working LEXOR interpreter!**

---

## 📞 Quick Links

- **Start Here**: [10_INDEX.md](10_INDEX.md)
- **Architecture**: [01_ARCHITECTURE_GUIDE.md](01_ARCHITECTURE_GUIDE.md)
- **Quick Reference**: [08_QUICK_REFERENCE.md](08_QUICK_REFERENCE.md)
- **Lexer Tutorial**: [02_LEXER_TUTORIAL.md](02_LEXER_TUTORIAL.md)
- **Parser Tutorial**: [03_PARSER_TUTORIAL.md](03_PARSER_TUTORIAL.md)
- **Interpreter Tutorial**: [04_INTERPRETER_TUTORIAL.md](04_INTERPRETER_TUTORIAL.md)
- **Examples**: [05_COMPLETE_EXAMPLES.md](05_COMPLETE_EXAMPLES.md)
- **Build Guide**: [06_BUILD_GUIDE.md](06_BUILD_GUIDE.md)
- **Troubleshooting**: [09_TROUBLESHOOTING_GUIDE.md](09_TROUBLESHOOTING_GUIDE.md)

---

**Ready to build a programming language?**

Begin with [10_INDEX.md](10_INDEX.md) for the complete learning roadmap.

Good luck! 🚀
