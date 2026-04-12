#include "parser.hpp"
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
    vector<string> vars;
    string varName = consume(TOKEN_IDENTIFIER, "Expected variable name").value;
    vars.push_back(varName);
    
    while (check(TOKEN_ASSIGN)) {
        advance();
        if (check(TOKEN_IDENTIFIER)) {
            string nextVar = consume(TOKEN_IDENTIFIER, "Expected variable name").value;
            vars.push_back(nextVar);
        } else {
            break;
        }
    }
    
    // Get the value
    ExprPtr value = expression();
    
    return make_shared<Assignment>(vars, value);
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
    
    if (match(TOKEN_NEWLINE)) {
        return make_shared<StringLiteral>("$");
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