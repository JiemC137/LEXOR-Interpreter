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
    vector<string> vars;
    ExprPtr value;
    
    Assignment(vector<string> v, ExprPtr e) : vars(v), value(e) {}
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