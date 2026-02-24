# Parser Implementation Tutorial

## What is a Parser?
The parser takes tokens from the lexer and builds an Abstract Syntax Tree (AST) that represents the program structure.

## Step 1: Define AST Node Types

Create `ast.h`:

```cpp
#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>
using namespace std;

// Forward declarations
class ASTNode;
class Program;
class Declaration;
class Statement;
class Expression;
class Variable;

// Smart pointer types
using NodePtr = shared_ptr<ASTNode>;
using StmtPtr = shared_ptr<Statement>;
using ExprPtr = shared_ptr<Expression>;
using DeclPtr = shared_ptr<Declaration>;

// Base class for all AST nodes
class ASTNode {
public:
    virtual ~ASTNode() = default;
};

// Base class for expressions
class Expression : public ASTNode {
public:
    virtual ~Expression() = default;
};

// Literal values
class NumberLiteral : public Expression {
public:
    double value;
    bool isFloat;
    NumberLiteral(double v, bool f) : value(v), isFloat(f) {}
};

class StringLiteral : public Expression {
public:
    string value;
    StringLiteral(string v) : value(v) {}
};

class CharLiteral : public Expression {
public:
    char value;
    CharLiteral(char v) : value(v) {}
};

class BoolLiteral : public Expression {
public:
    bool value;
    BoolLiteral(bool v) : value(v) {}
};

// Variable reference
class Identifier : public Expression {
public:
    string name;
    Identifier(string n) : name(n) {}
};

// Binary operations
class BinaryOp : public Expression {
public:
    string op;  // "+", "-", "*", "/", "%", "<", ">", "<=", ">=", "==", "<>", "AND", "OR"
    ExprPtr left;
    ExprPtr right;
    
    BinaryOp(string o, ExprPtr l, ExprPtr r) : op(o), left(l), right(r) {}
};

// Unary operations
class UnaryOp : public Expression {
public:
    string op;  // "+", "-", "NOT"
    ExprPtr operand;
    
    UnaryOp(string o, ExprPtr e) : op(o), operand(e) {}
};

// Base class for statements
class Statement : public ASTNode {
public:
    virtual ~Statement() = default;
};

// Variable declaration
class Declaration : public Statement {
public:
    string type;  // "INT", "CHAR", "BOOL", "FLOAT"
    vector<pair<string, ExprPtr>> variables;  // name and optional initial value
    
    Declaration(string t) : type(t) {}
};

// Assignment statement
class Assignment : public Statement {
public:
    string var;
    ExprPtr value;
    
    Assignment(string v, ExprPtr e) : var(v), value(e) {}
};

// Print statement
class PrintStatement : public Statement {
public:
    vector<ExprPtr> expressions;
    PrintStatement() {}
};

// Scan statement
class ScanStatement : public Statement {
public:
    vector<string> variables;
    ScanStatement() {}
};

// If statement
class IfStatement : public Statement {
public:
    ExprPtr condition;
    vector<StmtPtr> ifBody;
    vector<StmtPtr> elseBody;
    bool hasElse;
    
    IfStatement(ExprPtr cond) : condition(cond), hasElse(false) {}
};

// For loop
class ForStatement : public Statement {
public:
    vector<StmtPtr> body;
    
    ForStatement() {}
};

// Repeat-When loop
class RepeatStatement : public Statement {
public:
    ExprPtr condition;
    vector<StmtPtr> body;
    
    RepeatStatement(ExprPtr cond) : condition(cond) {}
};

// Program (root node)
class Program : public ASTNode {
public:
    vector<DeclPtr> declarations;
    vector<StmtPtr> statements;
    
    Program() {}
};

#endif
```

## Step 2: Create the Parser Class

Create `parser.h`:

```cpp
#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ast.h"
#include <vector>
#include <stdexcept>
using namespace std;

class Parser {
private:
    vector<Token> tokens;
    int current;  // Current token position
    
    // Helper functions
    Token peek();                          // Look at current token
    Token peekNext();                      // Look at next token
    Token advance();                       // Move to next token
    bool check(TokenType type);            // Check if current token matches type
    bool match(TokenType type);            // Check and advance if match
    bool match(vector<TokenType> types);   // Match any in list
    Token consume(TokenType type, string msg);  // Consume or error
    void error(string message);            // Report error
    
    // Parsing functions (recursive descent)
    shared_ptr<Program> program();
    void declarations();
    shared_ptr<Declaration> declaration();
    void statements(shared_ptr<Program> prog);
    shared_ptr<Statement> statement();
    shared_ptr<Assignment> assignment();
    shared_ptr<PrintStatement> printStatement();
    shared_ptr<ScanStatement> scanStatement();
    shared_ptr<IfStatement> ifStatement();
    shared_ptr<ForStatement> forStatement();
    shared_ptr<RepeatStatement> repeatStatement();
    
    // Expression parsing (operator precedence)
    shared_ptr<Expression> expression();        // OR
    shared_ptr<Expression> logicalAnd();        // AND
    shared_ptr<Expression> comparison();        // >, <, >=, <=, ==, <>
    shared_ptr<Expression> additive();          // +, -
    shared_ptr<Expression> multiplicative();    // *, /, %
    shared_ptr<Expression> unary();             // +, -, NOT
    shared_ptr<Expression> primary();           // literals, identifiers, ()
    
public:
    Parser(vector<Token> t);
    shared_ptr<Program> parse();
};

#endif
```

## Step 3: Implement the Parser

Create `parser.cpp`:

```cpp
#include "parser.h"
#include <iostream>
#include <sstream>

Parser::Parser(vector<Token> t) : tokens(t), current(0) {}

Token Parser::peek() {
    if (current >= tokens.size()) {
        return tokens.back();  // Return EOF
    }
    return tokens[current];
}

Token Parser::peekNext() {
    if (current + 1 >= tokens.size()) {
        return tokens.back();
    }
    return tokens[current + 1];
}

Token Parser::advance() {
    Token prev = peek();
    if (current < tokens.size()) current++;
    return prev;
}

bool Parser::check(TokenType type) {
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::match(vector<TokenType> types) {
    for (auto t : types) {
        if (check(t)) {
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType type, string msg) {
    if (check(type)) {
        return advance();
    }
    error(msg);
    return peek();
}

void Parser::error(string message) {
    cerr << "Parse Error at line " << peek().line 
         << ", column " << peek().column << ": " << message << endl;
    throw runtime_error(message);
}

shared_ptr<Program> Parser::program() {
    auto prog = make_shared<Program>();
    
    // SCRIPT AREA
    consume(TOKEN_SCRIPT, "Expected SCRIPT");
    consume(TOKEN_AREA, "Expected AREA");
    
    // START SCRIPT
    consume(TOKEN_START, "Expected START");
    consume(TOKEN_SCRIPT, "Expected SCRIPT");
    
    // Parse declarations
    declarations();
    while (check(TOKEN_DECLARE)) {
        auto decl = declaration();
        prog->declarations.push_back(decl);
    }
    
    // Parse statements
    statements(prog);
    
    // END SCRIPT
    consume(TOKEN_END, "Expected END");
    consume(TOKEN_SCRIPT, "Expected SCRIPT");
    
    consume(TOKEN_EOF, "Expected EOF");
    
    return prog;
}

void Parser::declarations() {
    // Already consumed in program()
}

shared_ptr<Declaration> Parser::declaration() {
    consume(TOKEN_DECLARE, "Expected DECLARE");
    
    // Get type
    string type;
    if (match(TOKEN_INT)) type = "INT";
    else if (match(TOKEN_CHAR)) type = "CHAR";
    else if (match(TOKEN_BOOL)) type = "BOOL";
    else if (match(TOKEN_FLOAT)) type = "FLOAT";
    else error("Expected INT, CHAR, BOOL, or FLOAT");
    
    auto decl = make_shared<Declaration>(type);
    
    // Parse variables
    do {
        string varName = consume(TOKEN_IDENTIFIER, "Expected variable name").value;
        ExprPtr initialValue = nullptr;
        
        // Check for initialization
        if (match(TOKEN_ASSIGN)) {
            initialValue = expression();
        }
        
        decl->variables.push_back({varName, initialValue});
    } while (match(TOKEN_COMMA));
    
    return decl;
}

void Parser::statements(shared_ptr<Program> prog) {
    while (!check(TOKEN_END) && !check(TOKEN_EOF)) {
        auto stmt = statement();
        if (stmt) {
            prog->statements.push_back(stmt);
        }
    }
}

shared_ptr<Statement> Parser::statement() {
    if (check(TOKEN_DECLARE)) {
        auto decl = declaration();
        return decl;
    }
    else if (check(TOKEN_PRINT)) {
        return printStatement();
    }
    else if (check(TOKEN_SCAN)) {
        return scanStatement();
    }
    else if (check(TOKEN_IF)) {
        return ifStatement();
    }
    else if (check(TOKEN_START) && peekNext().type == TOKEN_FOR) {
        advance(); // consume START
        advance(); // consume FOR
        return forStatement();
    }
    else if (check(TOKEN_REPEAT)) {
        return repeatStatement();
    }
    else if (check(TOKEN_IDENTIFIER)) {
        return assignment();
    }
    else {
        error("Unexpected token: " + peek().value);
        return nullptr;
    }
}

shared_ptr<Assignment> Parser::assignment() {
    string varName = consume(TOKEN_IDENTIFIER, "Expected variable name").value;
    
    // Support chained assignments: x=y=z=5
    vector<string> vars{varName};
    while (check(TOKEN_ASSIGN)) {
        advance();
        if (check(TOKEN_IDENTIFIER)) {
            vars.push_back(consume(TOKEN_IDENTIFIER, "Expected variable name").value);
        } else {
            break;
        }
    }
    
    // Get the value
    ExprPtr value = expression();
    
    // Create assignments from right to left
    shared_ptr<Assignment> result = make_shared<Assignment>(vars.back(), value);
    
    for (int i = vars.size() - 2; i >= 0; i--) {
        auto id = make_shared<Identifier>(vars[i+1]);
        result = make_shared<Assignment>(vars[i], id);
    }
    
    return result;
}

shared_ptr<PrintStatement> Parser::printStatement() {
    consume(TOKEN_PRINT, "Expected PRINT");
    consume(TOKEN_COLON, "Expected :");
    
    auto stmt = make_shared<PrintStatement>();
    
    // Parse print expressions
    do {
        if (match(TOKEN_CONCAT)) continue;  // Skip & at start
        stmt->expressions.push_back(expression());
    } while (match(TOKEN_CONCAT));
    
    return stmt;
}

shared_ptr<ScanStatement> Parser::scanStatement() {
    consume(TOKEN_SCAN, "Expected SCAN");
    consume(TOKEN_COLON, "Expected :");
    
    auto stmt = make_shared<ScanStatement>();
    
    do {
        string varName = consume(TOKEN_IDENTIFIER, "Expected variable name").value;
        stmt->variables.push_back(varName);
    } while (match(TOKEN_COMMA));
    
    return stmt;
}

shared_ptr<IfStatement> Parser::ifStatement() {
    consume(TOKEN_IF, "Expected IF");
    consume(TOKEN_LPAREN, "Expected (");
    auto condition = expression();
    consume(TOKEN_RPAREN, "Expected )");
    
    consume(TOKEN_START, "Expected START");
    consume(TOKEN_IF, "Expected IF");
    
    auto ifStmt = make_shared<IfStatement>(condition);
    
    // Parse if body
    while (!check(TOKEN_END)) {
        if (check(TOKEN_ELSE)) break;
        ifStmt->ifBody.push_back(statement());
    }
    
    consume(TOKEN_END, "Expected END");
    consume(TOKEN_IF, "Expected IF");
    
    // Check for else
    if (match(TOKEN_ELSE)) {
        ifStmt->hasElse = true;
        
        if (check(TOKEN_IF)) {
            advance();
            // ELSE IF - treat as nested if
            consume(TOKEN_LPAREN, "Expected (");
            auto elseCondition = expression();
            consume(TOKEN_RPAREN, "Expected )");
            
            consume(TOKEN_START, "Expected START");
            consume(TOKEN_IF, "Expected IF");
            
            auto elseIfStmt = make_shared<IfStatement>(elseCondition);
            
            while (!check(TOKEN_END)) {
                if (check(TOKEN_ELSE)) break;
                elseIfStmt->ifBody.push_back(statement());
            }
            
            consume(TOKEN_END, "Expected END");
            consume(TOKEN_IF, "Expected IF");
            
            ifStmt->elseBody.push_back(elseIfStmt);
        } else {
            consume(TOKEN_START, "Expected START");
            consume(TOKEN_IF, "Expected IF");
            
            while (!check(TOKEN_END)) {
                ifStmt->elseBody.push_back(statement());
            }
            
            consume(TOKEN_END, "Expected END");
            consume(TOKEN_IF, "Expected IF");
        }
    }
    
    return ifStmt;
}

shared_ptr<ForStatement> Parser::forStatement() {
    auto stmt = make_shared<ForStatement>();
    
    while (!check(TOKEN_END)) {
        stmt->body.push_back(statement());
    }
    
    consume(TOKEN_END, "Expected END");
    consume(TOKEN_FOR, "Expected FOR");
    
    return stmt;
}

shared_ptr<RepeatStatement> Parser::repeatStatement() {
    consume(TOKEN_REPEAT, "Expected REPEAT");
    consume(TOKEN_WHEN, "Expected WHEN");
    consume(TOKEN_LPAREN, "Expected (");
    auto condition = expression();
    consume(TOKEN_RPAREN, "Expected )");
    
    consume(TOKEN_START, "Expected START");
    consume(TOKEN_REPEAT, "Expected REPEAT");
    
    auto stmt = make_shared<RepeatStatement>(condition);
    
    while (!check(TOKEN_END)) {
        stmt->body.push_back(statement());
    }
    
    consume(TOKEN_END, "Expected END");
    consume(TOKEN_REPEAT, "Expected REPEAT");
    
    return stmt;
}

// Expression parsing with operator precedence
shared_ptr<Expression> Parser::expression() {
    return logicalAnd();
}

shared_ptr<Expression> Parser::logicalAnd() {
    auto left = comparison();
    
    while (match(TOKEN_AND)) {
        string op = "AND";
        auto right = comparison();
        left = make_shared<BinaryOp>(op, left, right);
    }
    
    return left;
}

shared_ptr<Expression> Parser::comparison() {
    auto left = additive();
    
    while (true) {
        if (match(TOKEN_GT)) {
            auto right = additive();
            left = make_shared<BinaryOp>(">", left, right);
        } else if (match(TOKEN_LT)) {
            auto right = additive();
            left = make_shared<BinaryOp>("<", left, right);
        } else if (match(TOKEN_GTE)) {
            auto right = additive();
            left = make_shared<BinaryOp>(">=", left, right);
        } else if (match(TOKEN_LTE)) {
            auto right = additive();
            left = make_shared<BinaryOp>("<=", left, right);
        } else if (match(TOKEN_EQ)) {
            auto right = additive();
            left = make_shared<BinaryOp>("==", left, right);
        } else if (match(TOKEN_NEQ)) {
            auto right = additive();
            left = make_shared<BinaryOp>("<>", left, right);
        } else {
            break;
        }
    }
    
    return left;
}

shared_ptr<Expression> Parser::additive() {
    auto left = multiplicative();
    
    while (true) {
        if (match(TOKEN_PLUS)) {
            auto right = multiplicative();
            left = make_shared<BinaryOp>("+", left, right);
        } else if (match(TOKEN_MINUS)) {
            auto right = multiplicative();
            left = make_shared<BinaryOp>("-", left, right);
        } else {
            break;
        }
    }
    
    return left;
}

shared_ptr<Expression> Parser::multiplicative() {
    auto left = unary();
    
    while (true) {
        if (match(TOKEN_MULTIPLY)) {
            auto right = unary();
            left = make_shared<BinaryOp>("*", left, right);
        } else if (match(TOKEN_DIVIDE)) {
            auto right = unary();
            left = make_shared<BinaryOp>("/", left, right);
        } else if (match(TOKEN_MODULO)) {
            auto right = unary();
            left = make_shared<BinaryOp>("%", left, right);
        } else {
            break;
        }
    }
    
    return left;
}

shared_ptr<Expression> Parser::unary() {
    if (match(TOKEN_PLUS)) {
        auto operand = unary();
        return make_shared<UnaryOp>("+", operand);
    }
    
    if (match(TOKEN_MINUS)) {
        auto operand = unary();
        return make_shared<UnaryOp>("-", operand);
    }
    
    if (match(TOKEN_NOT)) {
        auto operand = unary();
        return make_shared<UnaryOp>("NOT", operand);
    }
    
    return primary();
}

shared_ptr<Expression> Parser::primary() {
    if (match(TOKEN_NUMBER)) {
        double value = stod(tokens[current - 1].value);
        bool isFloat = tokens[current - 1].value.find('.') != string::npos;
        return make_shared<NumberLiteral>(value, isFloat);
    }
    
    if (match(TOKEN_STRING)) {
        return make_shared<StringLiteral>(tokens[current - 1].value);
    }
    
    if (match(TOKEN_CHAR_LIT)) {
        return make_shared<CharLiteral>(tokens[current - 1].value[0]);
    }
    
    if (match(TOKEN_TRUE)) {
        return make_shared<BoolLiteral>(true);
    }
    
    if (match(TOKEN_FALSE)) {
        return make_shared<BoolLiteral>(false);
    }
    
    if (match(TOKEN_IDENTIFIER)) {
        return make_shared<Identifier>(tokens[current - 1].value);
    }
    
    if (match(TOKEN_LPAREN)) {
        auto expr = expression();
        consume(TOKEN_RPAREN, "Expected )");
        return expr;
    }
    
    error("Unexpected token: " + peek().value);
    return nullptr;
}

shared_ptr<Program> Parser::parse() {
    try {
        return program();
    } catch (const exception& e) {
        cerr << "Parsing failed: " << e.what() << endl;
        return nullptr;
    }
}
```

## Key Parsing Concepts

1. **Recursive Descent**: Each grammar rule becomes a function
2. **Operator Precedence**: Functions call from lowest to highest precedence
   - Logical AND/OR (lowest)
   - Comparison operators
   - Addition/Subtraction
   - Multiplication/Division/Modulo (highest)
3. **Token Consumption**: Use `match()` to consume expected tokens
4. **Error Recovery**: Report line/column for debugging

## Testing the Parser

Test in `main.cpp`:

```cpp
#include "lexer.h"
#include "parser.h"
#include <iostream>

int main() {
    string code = R"(
SCRIPT AREA
START SCRIPT
DECLARE INT x=5
x = 10
PRINT: x
END SCRIPT
)";

    Lexer lexer(code);
    auto tokens = lexer.tokenize();
    
    Parser parser(tokens);
    auto ast = parser.parse();
    
    if (ast) {
        cout << "Parsing successful!" << endl;
    }
    
    return 0;
}
```

Next: Interpreter Implementation (04_INTERPRETER_TUTORIAL.md)
