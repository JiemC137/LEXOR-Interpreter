# Increment 1 Line-by-Line Implementation Plan

Scope: Structure, comments, reserved words, concatenation, declarations, assignment, PRINT.
Skip: SCAN, IF/ELSE, loops, arithmetic/logical ops beyond basic literals.

## 0) Create Project Skeleton
Create these folders (if not already):
- include/
- src/
- examples/

## 1) Token Definitions (include/token.h)
Implement only the needed token types.

Token types to include:
- Keywords: SCRIPT, AREA, START, END, DECLARE, INT, CHAR, BOOL, FLOAT, PRINT
- Literals: NUMBER, STRING, CHAR_LIT, TRUE, FALSE
- Identifiers: IDENTIFIER
- Operators/punctuation: ASSIGN (=), CONCAT (&), NEWLINE ($), COMMA (,), COLON (:), LPAREN, RPAREN
- Special: EOF, ERROR

Keep Token fields:
- type
- value
- line
- column

## 2) Lexer (include/lexer.h, src/lexer.cpp)
Goal: tokenize source; skip comments; recognize reserved words; parse literals.

### 2.1 Lexer interface
Expose:
- Lexer(string source)
- Token nextToken()
- vector<Token> tokenize()

### 2.2 Lexer core behavior
Implement these functions in this order:
1) advance()        -> moves position, updates line/column
2) peek()           -> lookahead
3) skipWhitespace() -> skip spaces/tabs
4) skipComment()    -> if '%%', skip to end of line
5) isDigit(), isAlpha(), isAlphaNumeric()
6) readNumber()     -> integer or float token
7) readString()     -> handle "..."
8) readCharLiteral()-> handle 'c'
9) getKeywordType() -> map reserved words
10) readIdentifierOrKeyword()
11) nextToken()     -> main tokenizer
12) tokenize()      -> collect all tokens

### 2.3 Minimum Lexing Rules
- Comments: if current == '%' and peek() == '%', skip to end of line
- Identifiers: start with letter/_ then letters/digits/_
- Keywords: match uppercase reserved words only
- Strings: "..." (no multiline)
- Chars: 'c' only
- Operators: =, &, $, :, ,

## 3) AST (include/ast.h)
Keep only what Increment 1 needs.

### 3.1 Expressions
- Literal: NumberLiteral, StringLiteral, CharLiteral, BoolLiteral
- Identifier

### 3.2 Statements
- Declaration
- Assignment
- PrintStatement

### 3.3 Program
- declarations: vector<Declaration>
- statements: vector<Statement>

## 4) Parser (include/parser.h, src/parser.cpp)
Goal: parse program structure, declarations, assignment, print.

### 4.1 Parser helpers
Implement:
- peek(), peekNext(), advance()
- check(), match()
- consume(type, message)
- error(message)

### 4.2 Grammar for Increment 1
Program:
SCRIPT AREA START SCRIPT
  Declarations
  Statements
END SCRIPT

Declarations:
DECLARE <TYPE> id [= literal] (, id [= literal])*

Statements:
- Assignment
- Print

Assignment:
id (= id)* = expression

Print:
PRINT : expression (& expression)*

Expression (Increment 1 only):
- literal
- identifier

### 4.3 Parsing Order
Implement in this order:
1) parse() -> program()
2) program()
3) declaration()
4) statement()
5) assignment()
6) printStatement()
7) primary()

## 5) Interpreter (include/interpreter.h, src/interpreter.cpp)
Goal: store variables, assign, print, concat.

### 5.1 Value class (include/value.h)
Support only:
- INT
- FLOAT
- CHAR
- BOOL
- STRING

Methods:
- toString()

### 5.2 Symbol table
Map:
- name -> (type, Value)

### 5.3 Execution
Implement:
- execute(program)
- executeStatement(decl/assign/print)
- evaluateExpression(literal/identifier)

### 5.4 Print behavior
- & concatenates string representations
- $ outputs newline

## 6) main.cpp (src/main.cpp)
Read file input, tokenize, parse, execute, output.

Flow:
1) read source code
2) Lexer -> tokens
3) Parser -> AST
4) Interpreter -> output

## 7) Test Program (examples/increment1.lexor)
Use this to validate:

%% Sample program
SCRIPT AREA
START SCRIPT
DECLARE INT x, y, z=5
DECLARE CHAR a_1='n'
DECLARE BOOL t="TRUE"
x=y=4
a_1='c'
%% comment
PRINT: x & t & z & $ & a_1 & [#] & "last"
END SCRIPT

Expected:
4TRUE5
c#last

## 8) Minimal Pass Criteria
You pass Increment 1 when:
- Comments are ignored
- Reserved words recognized
- Declarations and assignments work
- PRINT with concatenation works
- Sample program output matches

## 9) What to Skip for Now
Do not implement:
- SCAN
- IF/ELSE
- FOR / REPEAT
- Full arithmetic/logical precedence
- NOT, AND, OR operators

## 10) Implementation Sequence (Suggested)
1) token.h
2) lexer.h + lexer.cpp
3) ast.h
4) parser.h + parser.cpp
5) value.h
6) interpreter.h + interpreter.cpp
7) main.cpp
8) example program + test
