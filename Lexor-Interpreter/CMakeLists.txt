cmake_minimum_required(VERSION 3.10)
project(LexorInterpreter)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Source files
set(SOURCES
    main.cpp
    lexer.cpp
    parser.cpp
    interpreter.cpp
)

# Create executable
add_executable(interpreter ${SOURCES})

# Compiler-specific flags
if(MSVC)
    # MSVC (Visual Studio)
    target_compile_options(interpreter PRIVATE /W4 /O2)
else()
    # GCC/Clang (Linux, macOS, MinGW)
    target_compile_options(interpreter PRIVATE -Wall -Wextra -Wpedantic -O2)
endif()

# Optional: Enable debugging symbols
if(CMAKE_BUILD_TYPE MATCHES Debug)
    if(MSVC)
        target_compile_options(interpreter PRIVATE /Zi)
    else()
        target_compile_options(interpreter PRIVATE -g)
    endif()
endif()

# Print configuration
message(STATUS "LEXOR Interpreter Build Configuration")
message(STATUS "====================================")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")