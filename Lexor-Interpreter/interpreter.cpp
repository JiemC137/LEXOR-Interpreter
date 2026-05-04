#include "interpreter.hpp"
#include <cmath>
#include <iostream>

Interpreter::Interpreter() : loopBreak(false) {}

Value Interpreter::checkTypeCompatibility(string varName, string type, Value value) {
    // Map declared type string to Value::Type and enforce compatibility.
    if (type == "INT") {
        if (value.type == Value::TYPE_INT) return value;
        // Disallow assigning non-numeric or float to INT implicitly
        if (value.type == Value::TYPE_FLOAT) {
            // allow narrowing by truncation
            return Value((int)value.toNumber());
        }
        throw runtime_error("Type error: cannot assign value of different type to INT variable '" + varName + "'");
    }

    if (type == "FLOAT") {
        if (value.type == Value::TYPE_FLOAT) return value;
        if (value.type == Value::TYPE_INT) {
            return Value(value.toNumber());
        }
        throw runtime_error("Type error: cannot assign non-numeric value to FLOAT variable '" + varName + "'");
    }

    if (type == "CHAR") {
        if (value.type == Value::TYPE_CHAR) return value;
        throw runtime_error("Type error: cannot assign value of different type to CHAR variable '" + varName + "'");
    }

    if (type == "BOOL") {
        if (value.type == Value::TYPE_BOOL) return value;
        if (value.type == Value::TYPE_STRING) {
            if (value.stringValue == "TRUE" || value.stringValue == "true") return Value(true);
            if (value.stringValue == "FALSE" || value.stringValue == "false") return Value(false);
            throw runtime_error("Type error: cannot convert string to BOOL for variable '" + varName + "'");
        }
        if (value.type == Value::TYPE_INT || value.type == Value::TYPE_FLOAT) {
            return Value(value.toNumber() != 0);
        }
        if (value.type == Value::TYPE_CHAR) {
            return Value(value.charValue != '\0');
        }
        throw runtime_error("Type error: cannot assign value of different type to BOOL variable '" + varName + "'");
    }

    if (type == "STRING") {
        if (value.type == Value::TYPE_STRING) return value;
        throw runtime_error("Type error: cannot assign non-string value to STRING variable '" + varName + "'");
    }

    // Unknown declared type
    throw runtime_error("Unknown declared type '" + type + "' for variable '" + varName + "'");
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
            // Prevent redeclaration
            if (symbolTable.find(varName) != symbolTable.end()) {
                throw runtime_error("Redeclaration error: variable '" + varName + "' already declared");
            }

            // Enforce declared type compatibility (may convert safely)
            Value storeVal = checkTypeCompatibility(varName, decl->type, value);

            symbolTable[varName] = {decl->type, storeVal};
        }
        return;
    }
    
    // Assignment
    if (auto assign = dynamic_pointer_cast<Assignment>(stmt)) {
        Value value = evaluateExpression(assign->value);
        
        for (auto& v : assign->vars) {
            if (symbolTable.find(v) != symbolTable.end()) {
                string declaredType = symbolTable[v].first;
                Value storeVal = checkTypeCompatibility(v, declaredType, value);
                symbolTable[v].second = storeVal;
            } else {
                throw runtime_error("Assignment error: variable '" + v + "' not declared");
            }
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
                // Enforce compatibility (may adjust numeric types)
                Value storeVal = checkTypeCompatibility(varName, type, val);
                symbolTable[varName].second = storeVal;
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