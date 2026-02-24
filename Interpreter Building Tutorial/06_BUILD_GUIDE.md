# Build and Compilation Guide

## Project Setup

### Directory Structure

```
Interpreter/
├── CMakeLists.txt
├── include/
│   ├── token.h
│   ├── ast.h
│   ├── value.h
│   ├── lexer.h
│   ├── parser.h
│   └── interpreter.h
├── src/
│   ├── main.cpp
│   ├── lexer.cpp
│   ├── parser.cpp
│   └── interpreter.cpp
├── examples/
│   ├── hello.lexor
│   ├── arithmetic.lexor
│   └── ... more examples
└── build/
    └── (generated build files)
```

---

## Option 1: CMake Build System

### Step 1: Create CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)
project(LexorInterpreter)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/interpreter.cpp
)

# Create executable
add_executable(interpreter ${SOURCES})

# Optional: Add compiler flags for optimization
if(MSVC)
    target_compile_options(interpreter PRIVATE /W4)
else()
    target_compile_options(interpreter PRIVATE -Wall -Wextra -Wpedantic)
endif()
```

### Step 2: Build with CMake (Visual Studio)

```bash
# Create build directory
mkdir build
cd build

# Generate Visual Studio project files
cmake -G "Visual Studio 16 2019" ..

# Build the project
cmake --build . --config Release

# Run the interpreter
./Release/interpreter.exe ../examples/hello.lexor
```

### Step 3: Build with CMake (MinGW/Linux)

```bash
mkdir build
cd build

# Generate Makefiles
cmake -G "Unix Makefiles" ..

# Build
make

# Run
./interpreter ../examples/hello.lexor
```

---

## Option 2: Visual Studio (Direct Project Setup)

### Step 1: Create Visual Studio Project

1. Open Visual Studio
2. Create new project: C++ Console Application
3. Name: `LexorInterpreter`

### Step 2: Add Header Files

- Right-click project → Add → New Item → Header File (.h)
- Create:
  - token.h
  - ast.h
  - value.h
  - lexer.h
  - parser.h
  - interpreter.h

### Step 3: Add Source Files

- Right-click project → Add → New Item → C++ File (.cpp)
- Create:
  - main.cpp
  - lexer.cpp
  - parser.cpp
  - interpreter.cpp

### Step 4: Configure Include Paths

1. Right-click project → Properties
2. VC++ Directories → Include Directories
3. Add: `./include` or `$(ProjectDir)include`

### Step 5: Build

Press F7 or Build → Build Solution

---

## Option 3: Command Line Compilation (Windows MSVC)

```bash
# Compile all files together
cl /std:c++17 /I.\include src\main.cpp src\lexer.cpp src\parser.cpp src\interpreter.cpp /Fe:interpreter.exe

# Or with optimization
cl /std:c++17 /O2 /I.\include src\main.cpp src\lexer.cpp src\parser.cpp src\interpreter.cpp /Fe:interpreter.exe

# Run
interpreter.exe examples\hello.lexor
```

---

## Option 4: GCC/Clang (Linux/macOS)

```bash
# Compile
g++ -std=c++17 -o interpreter -I./include src/main.cpp src/lexer.cpp src/parser.cpp src/interpreter.cpp

# Or with optimizations
g++ -std=c++17 -O2 -Wall -Wextra -o interpreter -I./include src/main.cpp src/lexer.cpp src/parser.cpp src/interpreter.cpp

# Run
./interpreter examples/hello.lexor
```

---

## Option 5: Makefile (Unix-like systems)

Create `Makefile`:

```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./include
LDFLAGS = 

SOURCES = src/main.cpp src/lexer.cpp src/parser.cpp src/interpreter.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = interpreter

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE) examples/hello.lexor

.PHONY: all clean run
```

Build with:
```bash
make              # Build
make run          # Build and run
make clean        # Clean
```

---

## Option 6: Visual Studio Code with C++

### Install Extensions

1. C/C++ (by Microsoft)
2. CMake Tools (by Microsoft)
3. Code Runner (optional)

### Create `.vscode/tasks.json`

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build",
            "command": "cl",
            "args": [
                "/std:c++17",
                "/O2",
                "/I.\\include",
                "src\\main.cpp",
                "src\\lexer.cpp",
                "src\\parser.cpp",
                "src\\interpreter.cpp",
                "/Fe:interpreter.exe"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": ["$msCompile"]
        },
        {
            "label": "run",
            "command": "${workspaceFolder}\\interpreter.exe",
            "args": ["${workspaceFolder}\\examples\\hello.lexor"],
            "group": {
                "kind": "test"
            }
        }
    ]
}
```

Build: Ctrl+Shift+B
Run: Ctrl+Shift+D

### Create `.vscode/launch.json`

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(gdb) Launch",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/interpreter.exe",
            "args": ["${workspaceFolder}/examples/hello.lexor"],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "miDebuggerPath": "gdb.exe",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ]
        }
    ]
}
```

Debug: F5

---

## Compilation Troubleshooting

| Error | Solution |
|-------|----------|
| "fatal error: ast.h: No such file or directory" | Add `-I./include` to include path |
| "undefined reference to `Lexer::nextToken()'`" | Make sure all `.cpp` files are compiled |
| "stray '\342' in program" | Encoding issue in source files, use UTF-8 |
| "'class Lexer' has no member named 'tokenize'" | Check header/implementation match |

---

## Running the Interpreter

### From LEXOR File
```bash
interpreter example.lexor
interpreter < example.lexor
```

### Interactive
```bash
interpreter
%% Type your program
SCRIPT AREA
...
END SCRIPT
%% Press Ctrl+D (EOF)
```

### Output Redirection
```bash
interpreter example.lexor > output.txt
```

---

## Testing Framework Setup

### Create `test.sh` (Unix) or `test.bat` (Windows)

**test.sh (Linux/macOS):**
```bash
#!/bin/bash

INTERPRETER="./interpreter"
EXAMPLES_DIR="./examples"

echo "Building..."
make clean
make

if [ $? -ne 0 ]; then
    echo "Build failed"
    exit 1
fi

echo -e "\n=== Running Tests ===\n"

for file in $EXAMPLES_DIR/*.lexor; do
    echo "Testing $(basename $file)..."
    $INTERPRETER "$file"
    echo "---"
done

echo -e "\nAll tests completed!"
```

**test.bat (Windows):**
```batch
@echo off

set INTERPRETER=interpreter.exe
set EXAMPLES_DIR=examples

echo Building...
cl /std:c++17 /O2 /I.\include src\main.cpp src\lexer.cpp src\parser.cpp src\interpreter.cpp /Fe:%INTERPRETER%

if errorlevel 1 (
    echo Build failed
    exit /b 1
)

echo.
echo === Running Tests ===
echo.

for %%f in (%EXAMPLES_DIR%\*.lexor) do (
    echo Testing %%~nf...
    %INTERPRETER% "%%f"
    echo ---
)

echo.
echo All tests completed!
```

Run:
```bash
chmod +x test.sh
./test.sh
```

---

## Performance Optimization

### Compiler Flags

**MSVC:**
```bash
/O2          # Optimize for speed
/Oi          # Generate intrinsic functions
/Ot          # Favor speed over size
```

**GCC/Clang:**
```bash
-O2          # Optimize for speed (recommended)
-O3          # More aggressive optimization
-march=native # Optimize for current CPU
```

### In Code:
- Use `const` for immutable values
- Pass large objects by reference
- Use `std::move` for temporary objects
- Consider pre-allocating containers

---

## Debugging Tips

### MSVC Debugger
- Set breakpoints: Click line number
- Step: F10 (over), F11 (into)
- Continue: F5
- Watch variables: Watch window
- Immediate window: Debug → Windows → Immediate

### GDB (Command Line)
```bash
gdb ./interpreter
(gdb) break parser.cpp:150
(gdb) run examples/hello.lexor
(gdb) step
(gdb) print symbolTable
(gdb) continue
```

### Debug Output
Add to code:
```cpp
#define DEBUG 1

#if DEBUG
    cerr << "Debug: " << variable << endl;
#endif
```

---

## Next Steps

1. ✓ Set up project structure
2. ✓ Implement and compile
3. ✓ Run examples
4. ✓ Debug and optimize
5. → Enhance with additional features

See: 07_ENHANCEMENT_GUIDE.md for advanced features
