#ifndef LEXER_H
#define LEXER_H

#include "token.hpp"
#include <string>
#include <vector>
using namespace std;

class Lexer {
private:
    string source;
    int position;      // Current character position
    int line;
    int column;
    char current;      // Current character
    
    // Helper functions
    void advance();                      // Move to next character
    char peek();                         // Look at next character without moving
    void skipWhitespace();               // Skip spaces and tabs (not newlines)
    void skipComment();                  // Skip %% comments
    bool isDigit(char c);
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
    
    // Token creation
    Token readNumber();                  // Read 123 or 45.67
    Token readString();                  // Read "hello"
    Token readCharLiteral();             // Read 'a'
    Token readIdentifierOrKeyword();     // Read variable names or keywords
    TokenType getKeywordType(string s);  // Check if identifier is keyword
    
public:
    Lexer(string sourceCode);
    Token nextToken();                   // Get next token
    vector<Token> tokenize();            // Get all tokens
};

#endif