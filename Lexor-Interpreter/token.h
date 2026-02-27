#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using namespace std;

enum TokenType {
    // Keywords
    TOKEN_SCRIPT,
    TOKEN_AREA,
    TOKEN_START,
    TOKEN_END,
    TOKEN_DECLARE,
    TOKEN_INT,
    TOKEN_CHAR,
    TOKEN_BOOL,
    TOKEN_FLOAT,
    TOKEN_PRINT,
    TOKEN_SCAN,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_REPEAT,
    TOKEN_WHEN,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,
    
    // Literals
    TOKEN_NUMBER,        // 123, 45.67
    TOKEN_STRING,        // "hello"
    TOKEN_CHAR_LIT,      // 'a'
    TOKEN_TRUE,          // "TRUE"
    TOKEN_FALSE,         // "FALSE"
    
    // Identifiers
    TOKEN_IDENTIFIER,    // variable names
    
    // Operators and Punctuation
    TOKEN_PLUS,          // +
    TOKEN_MINUS,         // -
    TOKEN_MULTIPLY,      // *
    TOKEN_DIVIDE,        // /
    TOKEN_MODULO,        // %
    TOKEN_GT,            // >
    TOKEN_LT,            // <
    TOKEN_GTE,           // >=
    TOKEN_LTE,           // <=
    TOKEN_EQ,            // ==
    TOKEN_NEQ,           // <>
    TOKEN_ASSIGN,        // =
    TOKEN_CONCAT,        // &
    TOKEN_NEWLINE,       // $
    TOKEN_LPAREN,        // (
    TOKEN_RPAREN,        // )
    TOKEN_LBRACKET,      // [
    TOKEN_RBRACKET,      // ]
    TOKEN_COLON,         // :
    TOKEN_COMMA,         // ,
    
    // Special
    TOKEN_EOF,           // End of file
    TOKEN_ERROR          // Error token
};

struct Token {
    TokenType type;
    string value;
    int line;
    int column;
    
    Token(TokenType t, string v, int l, int c) 
        : type(t), value(v), line(l), column(c) {}
};

#endif