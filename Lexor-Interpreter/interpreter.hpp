#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.hpp"
#include "value.hpp"
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
    Value checkTypeCompatibility(string varName, string type, Value value);
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