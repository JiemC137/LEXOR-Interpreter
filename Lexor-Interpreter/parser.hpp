#ifndef PARSER_H
#define PARSER_H

#include "token.hpp"
#include "ast.hpp"
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